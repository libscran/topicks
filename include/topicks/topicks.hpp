#ifndef TOPICKER_TOPICKER_HPP
#define TOPICKER_TOPICKER_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <type_traits>

#include "sanisizer/sanisizer.hpp"

/**
 * @file topicks.hpp
 * @brief Choose top genes for downstream analyses.
 */

namespace topicks {

/**
 * @brief Options for `pick_top_genes()`.
 * @tparam Stat_ Numeric type of the statistic for picking top genes.
 */
template<typename Stat_>
struct PickTopGenesOptions {
    /**
     * A absolute bound for the statistic, to be considered when choosing the top genes.
     * A gene will not be picked, even if it is among the top `top` genes, if its statistic is:
     *
     * - equal to or lower than the bound, when `larger = true` and `PickTopGenesOptions::open_bound = true`.
     * - lower than the bound, when `larger = true` and `PickTopGenesOptions::open_bound = false`.
     * - equal to or greater than the bound, when `larger = false` and `PickTopGenesOptions::open_bound = true`.
     * - greater than the bound, when `larger = false` and `PickTopGenesOptions::open_bound = false`.
     *
     * If unset, no absolute bound is applied to the statistic.
     */
    std::optional<Stat_> bound;

    /**
     * Whether `PickTopGenesOptions::bound` is an open interval, i.e., genes with statistics equal to the bound will not be picked. 
     * Only relevant if `PickTopGenesOptions::bound` is set.
     */
    bool open_bound = true;

    /**
     * Whether to keep all genes with statistics that are tied with the `PickTopGenesOptions::top`-th gene.
     * If `false`, ties are arbitrarily broken but the number of retained genes will not be greater than `PickTopGenesOptions::top`.
     */
    bool keep_ties = true;
};

/**
 * @cond
 */
namespace internal {

template<typename Input_>
std::remove_cv_t<std::remove_reference_t<Input_> > I(Input_ x) {
    return x;
}

template<bool keep_index_, typename Index_, typename Stat_, class Output_, class Cmp_>
void filter_genes_by_threshold(const Index_ n, const Stat_* statistic, Output_& output, const Cmp_ cmp, const Stat_ threshold) {
    for (Index_ i = 0; i < n; ++i) {
        const bool ok = cmp(statistic[i], threshold);
        if constexpr(keep_index_) {
            if (ok) {
                output.push_back(i);
            }
        } else {
            output[i] = ok;
        }
    }
}

template<bool keep_index_, typename Index_, typename Stat_, class Output_, class Cmp_>
void select_top_genes_by_threshold(const Index_ top, const Stat_* statistic, Output_& output, const Cmp_ cmp, const Stat_ threshold, const std::vector<Index_>& semi_sorted) {
    Index_ counter = top;
    while (counter > 0) {
        --counter;
        const auto pos = semi_sorted[counter];
        if (cmp(statistic[pos], threshold)) {
            if constexpr(keep_index_) {
                output.push_back(pos);
            } else {
                output[pos] = true;
            }
        }
    }
}

template<bool keep_index_, typename Index_, typename Stat_, typename Top_, class Output_, class CmpNotEqual_, class CmpEqual_>
void pick_top_genes(const Index_ n, const Stat_* statistic, const Top_ top, Output_& output, const CmpNotEqual_ cmpne, const CmpEqual_ cmpeq, const PickTopGenesOptions<Stat_>& options) {
    if (top == 0) {
        if constexpr(keep_index_) {
            ; // no-op, we assume it's already empty.
        } else {
            std::fill_n(output, n, false);
        }
        return;
    }

    if (sanisizer::is_greater_than(top, n)) {
        if (options.bound.has_value()) {
            if (options.open_bound) {
                filter_genes_by_threshold<keep_index_>(n, statistic, output, cmpne, *(options.bound));
            } else {
                filter_genes_by_threshold<keep_index_>(n, statistic, output, cmpeq, *(options.bound));
            }
        } else {
            if constexpr(keep_index_) {
                output.resize(sanisizer::cast<decltype(I(output.size()))>(n));
                std::iota(output.begin(), output.end(), static_cast<Index_>(0));
            } else {
                std::fill_n(output, n, true);
            }
        }
        return;
    }

    auto semi_sorted = sanisizer::create<std::vector<Index_> >(n);
    std::iota(semi_sorted.begin(), semi_sorted.end(), static_cast<Index_>(0));
    const auto cBegin = semi_sorted.begin(), cMid = cBegin + top - 1, cEnd = semi_sorted.end();
    std::nth_element(cBegin, cMid, cEnd, [&](Index_ l, Index_ r) -> bool { 
        auto L = statistic[l], R = statistic[r];
        if (L == R) {
            return l < r; // always favor the earlier index for a stable sort, even if larger = false.
        } else {
            return cmpne(L, R);
        }
    });
    const Stat_ threshold = statistic[*cMid];

    if (options.keep_ties) {
        if (options.bound.has_value()) {
            const auto bound = *(options.bound);

            if (options.open_bound) {
                if (!cmpne(threshold, bound)) {
                    filter_genes_by_threshold<keep_index_>(n, statistic, output, cmpne, *(options.bound));
                    return;
                }
            } else {
                if (!cmpeq(threshold, bound)) {
                    filter_genes_by_threshold<keep_index_>(n, statistic, output, cmpeq, *(options.bound));
                    return;
                }
            }
        }

        filter_genes_by_threshold<keep_index_>(n, statistic, output, cmpeq, threshold);
        return;
    }

    if constexpr(keep_index_) {
        output.reserve(sanisizer::cast<decltype(I(output.size()))>(top));
    } else {
        std::fill_n(output, n, false);
    }

    if (options.bound.has_value()) {
        // cast of 'top' to Index_ is known safe as top <= n by this point.
        if (options.open_bound) {
            select_top_genes_by_threshold<keep_index_>(static_cast<Index_>(top), statistic, output, cmpne, *(options.bound), semi_sorted);
        } else {
            select_top_genes_by_threshold<keep_index_>(static_cast<Index_>(top), statistic, output, cmpeq, *(options.bound), semi_sorted);
        }
    } else {
        if constexpr(keep_index_) {
            output.insert(output.end(), semi_sorted.begin(), semi_sorted.begin() + top);
        } else {
            for (decltype(I(top)) i = 0; i < top; ++i) {
                output[semi_sorted[i]] = true;
            }
        }
    }

    if constexpr(keep_index_) {
        std::sort(output.begin(), output.end());
    }
}

}
/**
 * @endcond
 */

/**
 * @tparam Stat_ Numeric type of the statistic for picking top genes.
 * @tparam Top_ Integer type of the number of top genes.
 * @tparam Bool_ Output boolean type. 
 *
 * @param n Number of genes.
 * @param[in] statistic Pointer to an array of length `n`, containing the statistics with which to rank genes.
 * @param top Number of top genes to choose.
 * Note that the actual number of chosen genes may be: 
 * - smaller than `top`, if the latter is greater than the total number of genes in the dataset. 
 * - smaller than `top`, if `PickTopGenesOptions::bound` is set and `top` is greater than `X`,
 *   where `X` is the number of genes in the dataset with statistics greater than `PickTopGenesOptions::bound`
 *   (or less than the bound, if `PickTopGenesOptions::larger = false`).
 * - larger than `top`, if `PickTopGenesOptions::keep_ties = true` and there are tied statistics at the `top`-th chosen gene.
 * @param larger Whether the top genes are defined as those with larger statistics.
 * @param[out] output Pointer to an array of length `n`. 
 * On output, the `i`-th element will be `true` if gene `i` is one of the top genes and `false` otherwise.
 * @param options Further options.
 */
template<typename Stat_, typename Top_, typename Bool_>
void pick_top_genes(const std::size_t n, const Stat_* const statistic, const Top_ top, const bool larger, Bool_* const output, const PickTopGenesOptions<Stat_>& options) {
    if (larger) {
        internal::pick_top_genes<false>(
            n, 
            statistic, 
            top,
            output, 
            [](Stat_ l, Stat_ r) -> bool { return l > r; },
            [](Stat_ l, Stat_ r) -> bool { return l >= r; },
            options
        );
    } else {
        internal::pick_top_genes<false>(
            n, 
            statistic, 
            top,
            output, 
            [](Stat_ l, Stat_ r) -> bool { return l < r; },
            [](Stat_ l, Stat_ r) -> bool { return l <= r; },
            options
        );
    }
}

/**
 * @tparam Bool_ Output boolean type. 
 * @tparam Stat_ Numeric type of the statistic for picking top genes.
 * @tparam Top_ Integer type of the number of top genes.
 *
 * @param n Number of genes.
 * @param[in] statistic Pointer to an array of length `n`, containing the statistics with which to rank genes.
 * @param top Number of top genes to choose, see the `pick_top_genes()` overload for more details.
 * @param larger Whether the top genes are defined as those with larger statistics.
 * @param options Further options.
 *
 * @return A vector of booleans of length `n`, indicating whether each gene is to be retained.
 */
template<typename Bool_, typename Stat_, typename Top_>
std::vector<Bool_> pick_top_genes(const std::size_t n, const Stat_* const statistic, const Top_ top, const bool larger, const PickTopGenesOptions<Stat_>& options) {
    auto output = sanisizer::create<std::vector<Bool_> >(n
#ifdef SCRAN_VARIANCES_TEST_INIT
        , SCRAN_VARIANCES_TEST_INIT
#endif
    );
    pick_top_genes(n, statistic, top, larger, output.data(), options);
    return output;
}

/**
 * @tparam Index_ Integer type of the output indices.
 * @tparam Stat_ Numeric type of the statistic for picking top genes.
 * @tparam Top_ Integer type of the number of top genes.
 *
 * @param n Number of genes.
 * @param[in] statistic Pointer to an array of length `n` containing the statistics with which to rank genes.
 * @param top Number of top genes to choose, see the `pick_top_genes()` overload for more details.
 * @param larger Whether the top genes are defined as those with larger statistics.
 * @param options Further options.
 *
 * @return Vector of sorted and unique indices for the chosen genes.
 * All indices are guaranteed to be non-negative and less than `n`.
 */
template<typename Index_, typename Stat_, typename Top_>
std::vector<Index_> pick_top_genes_index(const Index_ n, const Stat_* const statistic, const Top_ top, const bool larger, const PickTopGenesOptions<Stat_>& options) {
    std::vector<Index_> output;
    if (larger) {
        internal::pick_top_genes<true>(
            n, 
            statistic, 
            top,
            output,
            [](Stat_ l, Stat_ r) -> bool { return l > r; },
            [](Stat_ l, Stat_ r) -> bool { return l >= r; },
            options
        );
    } else {
        internal::pick_top_genes<true>(
            n, 
            statistic, 
            top,
            output,
            [](Stat_ l, Stat_ r) -> bool { return l < r; },
            [](Stat_ l, Stat_ r) -> bool { return l <= r; },
            options
        );
    }
    return output;
}

}

#endif
