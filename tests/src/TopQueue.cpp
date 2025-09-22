#include "scran_tests/scran_tests.hpp"

#include "topicks/topicks.hpp"
#include "topicks/TopQueue.hpp"

TEST(TopQueue, Larger) {
    topicks::TopQueueOptions<double> options;
    options.keep_ties = false;
    topicks::TopQueue<double, int> tq(3, true, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 2);

    tq.push({ 0.5, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 4 });
    EXPECT_EQ(tq.top(), std::make_pair(0.5, 1));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 0 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4)); // tie with the higher index is of lower priority.
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 8.9, 6 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 0)); // keeps the tie with the lower index.
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 0.9, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 0)); // no effect.
    EXPECT_EQ(tq.size(), 3);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(8.9, 6));
    EXPECT_EQ(tq.size(), 2);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.pop();
    EXPECT_EQ(tq.size(), 0);
}

TEST(TopQueue, LargerTies) {
    topicks::TopQueueOptions<double> options;
    options.keep_ties = true;
    topicks::TopQueue<double, int> tq(3, true, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 2);

    tq.push({ 5.2, 4 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4)); // tie with the higher index is of lower priority.
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 8.9, 6 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4)); // still keeps the tie with the lower index.
    EXPECT_EQ(tq.size(), 4);

    tq.push({ 5.2, 0 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4));
    EXPECT_EQ(tq.size(), 5);

    tq.push({ 5.2, 10 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 10));
    EXPECT_EQ(tq.size(), 6);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4));
    EXPECT_EQ(tq.size(), 5);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 1)); // make sure it deletes from the 
    EXPECT_EQ(tq.size(), 4);

    tq.push({ 8.9, 5 });
    EXPECT_EQ(tq.top(), std::make_pair(8.9, 6));
    EXPECT_EQ(tq.size(), 3);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(8.9, 5));
    EXPECT_EQ(tq.size(), 2);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.pop();
    EXPECT_EQ(tq.size(), 0);
}

TEST(TopQueue, LargerBound) {
    topicks::TopQueueOptions<double> options;
    options.bound = 5;
    topicks::TopQueue<double, int> tq(3, true, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 5.0, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);
}

TEST(TopQueue, LargerBoundClosed) {
    topicks::TopQueueOptions<double> options;
    options.bound = 5;
    options.open_bound = false;
    topicks::TopQueue<double, int> tq(3, true, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(10.1, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 5.0, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(5.0, 3));
    EXPECT_EQ(tq.size(), 2);
}

TEST(TopQueue, Smaller) {
    topicks::TopQueueOptions<double> options;
    options.keep_ties = false;
    topicks::TopQueue<double, int> tq(3, false, options);

    tq.push({ 8.8, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 2);

    tq.push({ 0.5, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 4 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 0 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 0)); // tie with the higher index is of lower priority.
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 0.1, 6 });
    EXPECT_EQ(tq.top(), std::make_pair(0.5, 1));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 0.9, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(0.5, 1)); // no effect.
    EXPECT_EQ(tq.size(), 3);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 6));
    EXPECT_EQ(tq.size(), 2);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 1);

    tq.pop();
    EXPECT_EQ(tq.size(), 0);
}

TEST(TopQueue, SmallerTies) {
    topicks::TopQueueOptions<double> options;
    options.keep_ties = true;
    topicks::TopQueue<double, int> tq(3, false, options);

    tq.push({ 8.8, 2 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 2);

    tq.push({ 0.5, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(8.8, 2));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 4 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 5.2, 0 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 4)); // tie with the higher index is of lower priority.
    EXPECT_EQ(tq.size(), 4);

    tq.push({ 5.2, 10 });
    EXPECT_EQ(tq.top(), std::make_pair(5.2, 10));
    EXPECT_EQ(tq.size(), 5);

    tq.push({ 0.2, 5 });
    EXPECT_EQ(tq.top(), std::make_pair(0.5, 1));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 0.2, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(0.2, 5));
    EXPECT_EQ(tq.size(), 3);

    tq.push({ 0.2, 0 });
    EXPECT_EQ(tq.top(), std::make_pair(0.2, 5));
    EXPECT_EQ(tq.size(), 4);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(0.2, 1));
    EXPECT_EQ(tq.size(), 3);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(0.2, 0));
    EXPECT_EQ(tq.size(), 2);

    tq.pop();
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 1);

    tq.pop();
    EXPECT_EQ(tq.size(), 0);
}

TEST(TopQueue, SmallerBound) {
    topicks::TopQueueOptions<double> options;
    options.bound = 5;
    topicks::TopQueue<double, int> tq(3, false, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.size(), 0);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 5.0, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 1);
}

TEST(TopQueue, SmallerBoundClosed) {
    topicks::TopQueueOptions<double> options;
    options.bound = 5;
    options.open_bound = false;
    topicks::TopQueue<double, int> tq(3, false, options);

    tq.push({ 10.1, 2 });
    EXPECT_EQ(tq.size(), 0);

    tq.push({ 0.1, 3 });
    EXPECT_EQ(tq.top(), std::make_pair(0.1, 3));
    EXPECT_EQ(tq.size(), 1);

    tq.push({ 5.0, 1 });
    EXPECT_EQ(tq.top(), std::make_pair(5.0, 1));
    EXPECT_EQ(tq.size(), 2);
}

class TopQueueTest : public ::testing::TestWithParam<std::tuple<int, int, bool, std::pair<bool, double> > > {};

TEST_P(TopQueueTest, Consistency) {
    auto p = GetParam();
    std::size_t ngenes = std::get<0>(p);
    std::size_t ntop = std::get<1>(p);
    bool larger = std::get<2>(p);
    auto bound = std::get<3>(p);

    auto x = scran_tests::simulate_vector(ngenes, [&]{
        scran_tests::SimulationParameters sparams;
        sparams.lower = 0;
        sparams.upper = 10;
        sparams.seed = ngenes * ntop + 42 + larger;
        return sparams;
    }());

    topicks::PickTopGenesOptions<double> popt;
    topicks::TopQueueOptions<double> qopt;
    if (bound.first) {
        popt.bound = bound.second;
        qopt.bound = bound.second;
    }

    auto expected = topicks::pick_top_genes_index(ngenes, x.data(), ntop, larger, popt);

    topicks::TopQueue<double, int> queue(ntop, larger, qopt);
    for (std::size_t i = 0; i < ngenes; ++i) {
        queue.emplace(x[i], i);
    }

    std::vector<std::size_t> observed; 
    while (!queue.empty()) {
        observed.push_back(queue.top().second); 
        queue.pop();
    }
    std::sort(observed.begin(), observed.end());
    EXPECT_EQ(observed, expected);

    // Also works with a closed bound.
    if (bound.first) {
        auto popt2 = popt;
        popt2.open_bound = false;
        auto expected_closed = topicks::pick_top_genes_index(ngenes, x.data(), ntop, larger, popt2);

        auto qopt2 = qopt;
        qopt2.open_bound = false;

        topicks::TopQueue<double, int> queue_closed(ntop, larger, qopt2);
        for (std::size_t i = 0; i < ngenes; ++i) {
            queue_closed.emplace(x[i], i);
        }

        std::vector<std::size_t> observed_closed; 
        while (!queue_closed.empty()) {
            observed_closed.push_back(queue_closed.top().second); 
            queue_closed.pop();
        }
        std::sort(observed_closed.begin(), observed_closed.end());
        EXPECT_EQ(observed_closed, expected_closed);
    }
}

INSTANTIATE_TEST_SUITE_P(
    TopQueue,
    TopQueueTest,
    ::testing::Combine(
        ::testing::Values(11, 111, 1111), // number of values
        ::testing::Values(5, 50, 500), // number of tops
        ::testing::Values(false, true), // larger or smaller
        ::testing::Values(
            std::make_pair(false, 0.0),
            std::make_pair(true, 1.0),
            std::make_pair(true, 5.0),
            std::make_pair(true, 9.0)
        ) // bounds
    )
);
