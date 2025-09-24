#include "scran_tests/scran_tests.hpp"

#include "topicks/pick_top_genes.hpp"

#include <unordered_set>
#include <cstddef>

class TopicksTest : public ::testing::TestWithParam<std::tuple<int, int, bool, std::pair<bool, double> > > {};

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

template<typename Bool_, typename Stat_>
std::pair<Stat_, Stat_> get_larger_limits(const std::size_t ngenes, const std::vector<Bool_>& keep, const std::vector<Stat_>& x) {
    double min_has = std::numeric_limits<Stat_>::infinity();
    double max_lost = -min_has;
    for (std::size_t o = 0; o < ngenes; ++o) {
        if (keep[o]) {
            min_has = std::min(min_has, x[o]);
        } else {
            max_lost = std::max(max_lost, x[o]);
        }
    }
    return std::make_pair(min_has, max_lost);
}

template<typename Bool_, typename Stat_>
std::pair<Stat_, Stat_> get_smaller_limits(const std::size_t ngenes, const std::vector<Bool_>& keep, const std::vector<Stat_>& x) {
    double min_lost = std::numeric_limits<Stat_>::infinity();
    double max_has = -min_lost;
    for (std::size_t o = 0; o < ngenes; ++o) {
        if (keep[o]) {
            max_has = std::max(max_has, x[o]);
        } else {
            min_lost = std::min(min_lost, x[o]);
        }
    }
    return std::make_pair(max_has, min_lost);
}


TEST_P(TopicksTest, Basic) {
    auto p = GetParam();
    std::size_t ngenes = std::get<0>(p);
    std::size_t ntop = std::get<1>(p);
    bool larger = std::get<2>(p);
    auto bound = std::get<3>(p);

    auto x = scran_tests::simulate_vector(ngenes, [&]{
        scran_tests::SimulationParameters sparams;
        sparams.lower = 0;
        sparams.upper = 10;
        sparams.seed = ngenes * ntop + 42;
        return sparams;
    }());

    topicks::PickTopGenesOptions<double> opt;
    if (bound.first) {
        opt.bound = bound.second;
    }
    auto output = topicks::pick_top_genes<char>(ngenes, x.data(), ntop, larger, opt);

    if (larger) {
        const auto& limits = get_larger_limits(ngenes, output, x);
        const auto min_has = limits.first;
        const auto max_lost = limits.second;
        EXPECT_GT(min_has, max_lost);
        if (bound.first) {
            EXPECT_GT(min_has, bound.second);
        }
    } else {
        const auto& limits = get_smaller_limits(ngenes, output, x);
        const auto max_has = limits.first;
        const auto min_lost = limits.second;
        EXPECT_LT(max_has, min_lost);
        if (bound.first) {
            EXPECT_LT(max_has, bound.second);
        }
    }

    auto nchosen = std::accumulate(output.begin(), output.end(), static_cast<int>(0));
    size_t limit = std::min(ngenes, ntop);
    if (bound.first) {
        EXPECT_LE(nchosen, limit);
    } else {
        EXPECT_EQ(nchosen, limit);
    }

    auto ioutput = topicks::pick_top_genes_index(ngenes, x.data(), ntop, larger, opt);
    compare_bool_with_index(output, ioutput);

    auto ioutput2 = topicks::pick_top_genes_index<int>(ngenes, x.data(), ntop, larger, opt); // check for different integer types.
    EXPECT_EQ(std::vector<size_t>(ioutput2.begin(), ioutput2.end()), ioutput);

    // We probably won't have tied values in 'x', so keeping ties or not shouldn't have an effect.
    {
        auto opt2 = opt;
        opt2.keep_ties = false;
        auto output_nt = topicks::pick_top_genes<char>(ngenes, x.data(), ntop, larger, opt2);
        if (std::unordered_set(x.begin(), x.end()).size() == x.size()) {
            EXPECT_EQ(output, output_nt);
        } else {
            for (std::size_t i = 0; i < ngenes; ++i) {
                if (output[i]) {
                    EXPECT_TRUE(output_nt[i]);
                }
            }
        }
    }

    // Also works with a closed bound.
    if (bound.first) {
        auto opt2 = opt;
        opt2.open_bound = false;
        auto output_closed = topicks::pick_top_genes<char>(ngenes, x.data(), ntop, larger, opt2);

        if (larger) {
            const auto& limits = get_larger_limits(ngenes, output, x);
            const auto min_has = limits.first;
            const auto max_lost = limits.second;
            EXPECT_GT(min_has, max_lost);
            EXPECT_GE(min_has, bound.second);
        } else {
            const auto& limits = get_smaller_limits(ngenes, output, x);
            const auto max_has = limits.first;
            const auto min_lost = limits.second;
            EXPECT_LT(max_has, min_lost);
            EXPECT_LE(max_has, bound.second);
        }
    }
}

TEST_P(TopicksTest, WithNaN) {
    auto p = GetParam();
    std::size_t ngenes = std::get<0>(p);
    std::size_t ntop = std::get<1>(p);
    bool larger = std::get<2>(p);
    auto bound = std::get<3>(p);

    auto x = scran_tests::simulate_vector(ngenes, [&]{
        scran_tests::SimulationParameters sparams;
        sparams.lower = 0;
        sparams.upper = 10;
        sparams.seed = ngenes * ntop + 69;
        return sparams;
    }());

    // Spiking in an NaN in every second value.
    for (std::size_t g = 0; g < ngenes; g += 2) {
        x[g] = std::numeric_limits<double>::quiet_NaN();
    }

    topicks::PickTopGenesOptions<double> opt;
    opt.check_nan = true;
    if (bound.first) {
        opt.bound = bound.second;
    }
    auto output = topicks::pick_top_genes<char>(ngenes, x.data(), ntop, larger, opt);

    std::vector<double> stripped;
    for (std::size_t g = 0; g < ngenes; ++g) {
        if (!std::isnan(x[g])) {
            stripped.push_back(x[g]);
        }
    }
    auto ref = topicks::pick_top_genes<char>(stripped.size(), stripped.data(), ntop, larger, opt);
    std::vector<char> full(ngenes);
    std::size_t counter = 0;
    for (std::size_t g = 0; g < ngenes; ++g) {
        if (!std::isnan(x[g])) {
            full[g] = ref[counter];
            ++counter;
        }
    }
    EXPECT_EQ(output, full);

    auto ioutput = topicks::pick_top_genes_index(ngenes, x.data(), ntop, larger, opt);
    compare_bool_with_index(output, ioutput);

    // We probably won't have tied values in 'x', so keeping ties or not shouldn't have an effect.
    {
        auto opt2 = opt;
        opt2.keep_ties = false;
        auto output_nt = topicks::pick_top_genes<char>(ngenes, x.data(), ntop, larger, opt2);
        if (std::unordered_set(x.begin(), x.end()).size() == x.size()) {
            EXPECT_EQ(output, output_nt);
        } else {
            for (std::size_t i = 0; i < ngenes; ++i) {
                if (output[i]) {
                    EXPECT_TRUE(output_nt[i]);
                }
            }
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    Topicks,
    TopicksTest,
    ::testing::Combine(
        ::testing::Values(11, 111, 1111), // number of values
        ::testing::Values(5, 50, 500), // number of tops
        ::testing::Values(true, false), // larger
        ::testing::Values(
            std::make_pair(false, 0.0),
            std::make_pair(true, 1.0),
            std::make_pair(true, 5.0),
            std::make_pair(true, 9.0)
        ) // bounds
    )
);

TEST(Topicks, TiedLarger) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };

    // Ignoring ties.
    topicks::PickTopGenesOptions<double> opt;
    opt.keep_ties = false;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        std::vector<size_t> expected { 3, 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Introducing a bound.
    opt.bound = 3.3;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        std::vector<size_t> expected { 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties.
    opt.keep_ties = true;
    opt.bound.reset();
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        std::vector<size_t> expected { 3, 5, 6, 7 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties, with a bound.
    opt.bound = 3.3;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        std::vector<size_t> expected { 5, 6 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(3), true, opt);
        compare_bool_with_index(output, ioutput);
    }
}

TEST(Topicks, TiedSmaller) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };

    // Ignoring ties.
    topicks::PickTopGenesOptions<double> opt;
    opt.keep_ties = false;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        std::vector<size_t> expected { 0, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Introducing a bound.
    opt.bound = 1.1;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        std::vector<size_t> expected { 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties.
    opt.keep_ties = true;
    opt.bound.reset();
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        std::vector<size_t> expected { 0, 2, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        compare_bool_with_index(output, ioutput);
    }

    // Keeping all ties with a bound.
    opt.bound = 1.1;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        std::vector<size_t> expected { 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(2), false, opt);
        compare_bool_with_index(output, ioutput);
    }
}

TEST(Topicks, Extremes) {
    std::vector<double> x{ 1.1, 2.2, 1.1, 3.3, 0.0, 4.4, 4.4, 3.3 };
    topicks::PickTopGenesOptions<double> opt;
    auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(0), true, opt);
    EXPECT_EQ(0, std::accumulate(output.begin(), output.end(), 0));

    auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(0), true, opt);
    EXPECT_TRUE(ioutput.empty());
}

TEST(Topicks, ClosedBound) {
    // Checking that the closed bound actually has an effect.
    std::vector<double> x{ 1.1, 2.2, 3.3, 0.0, 4.4 };

    topicks::PickTopGenesOptions<double> opt;
    opt.bound = 2.2;

    // Running this as a control.
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(10), true, opt);
        std::vector<size_t> expected { 2, 4 };
        EXPECT_EQ(ioutput, expected);
    }

    opt.open_bound = false;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(10), true, opt);
        std::vector<size_t> expected { 1, 2, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(10), true, opt);
        compare_bool_with_index(output, ioutput);
    }

    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(4), true, opt);
        std::vector<size_t> expected { 1, 2, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(4), true, opt);
        compare_bool_with_index(output, ioutput);
    }

    opt.keep_ties = false;
    {
        auto ioutput = topicks::pick_top_genes_index(x.size(), x.data(), static_cast<std::size_t>(4), true, opt);
        std::vector<size_t> expected { 1, 2, 4 };
        EXPECT_EQ(ioutput, expected);
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), static_cast<std::size_t>(4), true, opt);
        compare_bool_with_index(output, ioutput);
    }
}

TEST(Topicks, NaNEdgeCases) {
    // Check that we short-circuit correctly if number of genes > top >= number of non-NaNs.
    {
        topicks::PickTopGenesOptions<double> opt;
        opt.check_nan = true;

        auto nan = std::numeric_limits<double>::quiet_NaN();
        std::vector<double> x{ 1.1, nan, 3.3, nan, 4.4 };
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        std::vector<char> expected{ true, false, true, false, true };
        EXPECT_EQ(output, expected);

        auto ioutput = topicks::pick_top_genes_index<int>(x.size(), x.data(), 3, true, opt);
        std::vector<int> iexpected{ 0, 2, 4 };
        EXPECT_EQ(ioutput, iexpected);
    }

    // Coverage for skipping the NaN check altogether if NaNs are not available.
    {
        topicks::PickTopGenesOptions<int> opt;
        opt.check_nan = true;

        std::vector<int> x{ 4, 2, 3, 0, 1 };
        auto output = topicks::pick_top_genes<char>(x.size(), x.data(), 3, true, opt);
        std::vector<char> expected{ true, true, true, false, false };
        EXPECT_EQ(output, expected);
    }
}
