include(FetchContent)
FetchContent_Declare(
  scran_tests
  GIT_REPOSITORY https://github.com/libscran/scran_tests
  GIT_TAG master
)
FetchContent_MakeAvailable(scran_tests)

include(GoogleTest)

option(CODE_COVERAGE "Enable coverage testing" OFF)
set(DO_CODE_COVERAGE OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(DO_CODE_COVERAGE ON)
endif()

macro(decorate_test target)
    target_link_libraries(${target} topicks scran_tests)

    target_compile_options(${target} PRIVATE -Wall -Werror -Wpedantic -Wextra)

    if(DO_CODE_COVERAGE)
        target_compile_options(${target} PRIVATE -O0 -g --coverage)
        target_link_options(${target} PRIVATE --coverage)
    endif()

    gtest_discover_tests(${target})
endmacro()

add_executable(libtest src/pick_top_genes.cpp src/TopQueue.cpp)
decorate_test(libtest)

add_executable(dirtytest src/pick_top_genes.cpp)
decorate_test(dirtytest)
target_compile_definitions(dirtytest PRIVATE "SCRAN_VARIANCES_TEST_INIT=scran_tests::initial_value()")
