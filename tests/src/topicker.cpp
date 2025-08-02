#include "scran_tests/scran_tests.hpp"

#include "topicker/topicker.hpp"

#include <unordered_set>
#include <cstddef>

class TopickerTest : public ::testing::TestWithParam<std::tuple<int, int, std::pair<bool, double> > > {};

template<typename Bool_, typename Index_>
static void compare_bool_with_index(const std::vector<Bool_>& asbool, const std::vector<Index_>& asindex) {
    std::vector<Index_> compared;
    for (size_t i = 0; i < asbool.size(); ++i) {
        if (asbool[i]) {
            compared.push_back(i);
        }
    }
    EXPECT_EQ(compared, asindex);
}

TEST_P(TopickerTest, Larger) {
    auto p = GetParam();
    std::size_t ngenes = std::get<0>(p);
    std::size_t ntop = std::get<1>(p);
    auto bound = std::get<2>(p);

    auto x = scran_tests::simulate_vector(ngenes, [&]{
        scran_tests::SimulationParameters sparams;
        sparams.lower = 0;
        sparams.upper = 10;
        sparams.seed = ngenes * ntop + 42;
        return sparams;
    }());

    topicker::PickTopGenesOptions<double> opt;
    if (bound.first) {
        opt.bound = bound.second;
    }

    auto output = topicker::pick_top_genes<char>(ngenes, x.data(), ntop, true, opt);

    double min_has = 100, max_lost = -100;
    for (size_t o = 0; o < ngenes; ++o) {
        if (output[o]) {
            min_has = std::min(min_has, x[o]);
        } else {
            max_lost = std::max(max_lost, x[o]);
        }
    }
    EXPECT_GT(min_has, max_lost);
    if (bound.first) {
        EXPECT_GT(min_has, bound.second);
    }

    auto nchosen = std::accumulate(output.begin(), output.end(), static_cast<int>(0));
    size_t limit = std::min(ngenes, ntop);
    if (bound.first) {
        EXPECT_LE(nchosen, limit);
    } else {
        EXPECT_EQ(nchosen, limit);
    }

    auto ioutput = topicker::pick_top_genes_index(ngenes, x.data(), ntop, true, opt);
    compare_bool_with_index(output, ioutput);

    auto ioutput2 = topicker::pick_top_genes_index<int>(ngenes, x.data(), ntop, true, opt); // check for different integer types.
    EXPECT_EQ(std::vector<size_t>(ioutput2.begin(), ioutput2.end()), ioutput);

    opt.keep_ties = false; // this should have no effect, assuming we don't have tied values in 'x'.
    auto output_nt = topicker::pick_top_genes<char>(ngenes, x.data(), ntop, true, opt);
    if (std::unordered_set(x.begin(), x.end()).size() == x.size()) {
        EXPECT_EQ(output, output_nt);
    }
}

TEST_P(TopickerTest, Smaller) {
    auto p = GetParam();
    size_t ngenes = std::get<0>(p);
    size_t ntop = std::get<1>(p);
    auto bound = std::get<2>(p);

    auto x = scran_tests::simulate_vector(ngenes, [&]{
        scran_tests::SimulationParameters sparams;
        sparams.lower = 0;
        sparams.upper = 10;
        sparams.seed = ngenes * ntop + 69;
        return sparams;
    }());

    topicker::PickTopGenesOptions<double> opt;
    if (bound.first) {
        opt.bound = bound.second;
    }

    auto output = topicker::pick_top_genes<char>(ngenes, x.data(), ntop, false, opt);

    double max_has = -100, min_lost = 100;
    for (size_t o = 0; o < ngenes; ++o) {
        if (output[o]) {
            max_has = std::max(max_has, x[o]);
        } else {
            min_lost = std::min(min_lost, x[o]);
        }
    }
    EXPECT_LT(max_has, min_lost);
    if (bound.first) {
        EXPECT_LT(max_has, bound.second);
    }

    auto nchosen = std::accumulate(output.begin(), output.end(), static_cast<int>(0));
    size_t limit = std::min(ngenes, ntop);
    if (bound.first) {
        EXPECT_LE(nchosen, limit);
    } else {
        EXPECT_EQ(nchosen, limit);
    }

    auto ioutput = topicker::pick_top_genes_index(ngenes, x.data(), ntop, false, opt);
    compare_bool_with_index(output, ioutput);

    opt.keep_ties = false; // this should have no effect, assuming we don't have tied values in 'x'.
    auto output_nt = topicker::pick_top_genes<char>(ngenes, x.data(), ntop, false, opt);
    if (std::unordered_set(x.begin(), x.end()).size() == x.size()) {
        EXPECT_EQ(output, output_nt);
    }
}

INSTANTIATE_TEST_SUITE_P(
    Topicker,
    TopickerTest,
    ::testing::Combine(
        ::testing::Values(11, 111, 1111), // number of values
        ::testing::Values(5, 50, 500), // number of tops
        ::testing::Values(
            std::make_pair(false, 0.0),
            std::make_pair(true, 1.0),
            std::make_pair(true, 5.0),
            std::make_pair(true, 9.0)
        ) // bounds
    )
);

TEST(Topicker, TiedLarger) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };

    // Ignoring ties.
    topicker::PickTopGenesOptions<double> opt;
    opt.keep_ties = false;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 3, true, opt);
        std::vector<size_t> expected { 3, 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Introducing a bound.
    opt.bound = 3.3;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 3, true, opt);
        std::vector<size_t> expected { 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties.
    opt.keep_ties = true;
    opt.bound.reset();
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 3, true, opt);
        std::vector<size_t> expected { 3, 5, 6, 7 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties, with a bound.
    opt.bound = 3.3;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 3, true, opt);
        std::vector<size_t> expected { 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        compare_bool_with_index(output, ioutput);
    }
}

TEST(Topicker, TiedSmaller) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };

    // Ignoring ties.
    topicker::PickTopGenesOptions<double> opt;
    opt.keep_ties = false;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 2, false, opt);
        std::vector<size_t> expected { 0, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 2, false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Introducing a bound.
    opt.bound = 1.1;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 2, false, opt);
        std::vector<size_t> expected { 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 2, false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties.
    opt.keep_ties = true;
    opt.bound.reset();
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 2, false, opt);
        std::vector<size_t> expected { 0, 2, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 2, false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties with a bound.
    opt.bound = 1.1;
    {
        auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 2, false, opt);
        std::vector<size_t> expected { 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 2, false, opt);
        compare_bool_with_index(output, ioutput);
    }
}

TEST(Topicker, Extremes) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };
    topicker::PickTopGenesOptions<double> opt;
    auto output = topicker::pick_top_genes<char>(x.size(), x.data(), 0, true, opt);
    EXPECT_EQ(0, std::accumulate(output.begin(), output.end(), 0));

    auto ioutput = topicker::pick_top_genes_index(x.size(), x.data(), 0, true, opt);
    EXPECT_TRUE(ioutput.empty());
}
