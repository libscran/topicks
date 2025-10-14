# Pick top genes

![Unit tests](https://github.com/libscran/topicks/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/libscran/topicks/actions/workflows/doxygenate.yaml/badge.svg)
[![Codecov](https://codecov.io/gh/libscran/topicks/graph/badge.svg?token=JWV0I4WJX2)](https://codecov.io/gh/libscran/topicks)

## Overview

The **topicks** library implements a `pick_top_genes()` function to pick the top genes based on some statistic.
The idea is to use this to choose highly variable genes based on their variances (e.g., from [**scran_variances**](https://github.com/libscran/scran_variances)),
or for picking the best markers based on a differential expression statistic (e.g., from [**scran_markers**](https://github.com/libscran/scran_markers)).
This functionality is surprisingly complex when we need to consider ties, absolute bounds, and whether to return a boolean filter or an array of indices. 
We also implement the `TopQueue` class, a tie- and bound-aware priority queue for retaining the top genes.

## Quick start

We can obtain an array of booleans indicating whether each gene was picked based on its `stats`:

```cpp
#include "topicks/topicks.hpp"

std::vector<double> stats(100); // vector of per-gene statistics.
topicks::PickTopGenesOptions<double> opts;
auto filter = topicks::pick_top_genes(
    stats.size(),
    stats.data(),
    10, // number of top genes to pick.
    true, // whether to pick genes with the largest 'stats'.
    opts
);
```

Alternatively we can obtain an array of integer indices:

```cpp
auto idx = topicks::pick_top_genes_index(stats.size(), stats.data(), 10, true, opts);
```

By default, ties at the selection boundary are retained so the actual number of chosen genes may be greater than what was requested.
This can be disabled via the `PickTopGenesOptions` options:

```cpp
opt.keep_ties = false;
```

We can also set an absolute bound on the statistic, e.g., to ensure that we never select marker genes with log-fold changes below some threshold:

```cpp
opt.bound = 0.5;
```

Perhaps we don't have an entire array of statistics, and we are only computing each gene's statistics as needed.
We can use the `TopQueue` class to choose the top genes in a running manner:

```cpp
topicks::TopQueueOptions<double> qopts;
qopts.bound = 0.6;
qopts.keep_ties = true;
qopts.check_nan = true;

// Picking the top 10 genes with the largest statistics.
topicks::TopQueue<double, int> running_best(10, true, qopts);
for (int g = 0; g < ngenes; ++g) {
    running_best.emplace(compute_some_statistic(g), g);
}

// Same result as pick_top_genes_index(), sans the ordering.
std::vector<int> best_final;
while (!running_best.empty()) {
    const auto& best = running_best.top();
    best_final.push_back(best.second);
    running_best.pop();
}
```

Check out the [reference documentation](https://libscran.github.io/topicks) for more details.

## Building projects

### CMake with `FetchContent`

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  topicks
  GIT_REPOSITORY https://github.com/libscran/topicks
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(topicks)
```

Then you can link to **topicks** to make the headers available during compilation:

```cmake
# For executables:
target_link_libraries(myexe libscran::topicks)

# For libaries
target_link_libraries(mylib INTERFACE libscran::topicks)
```

### CMake with `find_package()`

```cmake
find_package(libscran_topicks CONFIG REQUIRED)
target_link_libraries(mylib INTERFACE libscran::topicks)
```

To install the library, use:

```sh
mkdir build && cd build
cmake .. -DTOPICKER_TESTS=OFF
cmake --build . --target install
```

By default, this will use `FetchContent` to fetch all external dependencies.
If you want to install them manually, use `-DTOPICKER_FETCH_EXTERN=OFF`.
See the tags in [`extern/CMakeLists.txt`](extern/CMakeLists.txt) to find compatible versions of each dependency.

### Manual

If you're not using CMake, the simple approach is to just copy the files in `include/` - either directly or with Git submodules -
and include their path during compilation with, e.g., GCC's `-I`.
This requires the external dependencies listed in [`extern/CMakeLists.txt`](extern/CMakeLists.txt). 
