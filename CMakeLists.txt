cmake_minimum_required(VERSION 3.14)

project(topicks
    VERSION 0.2.2
    DESCRIPTION "Pick top genes for downstream analyses"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Library
add_library(topicks INTERFACE)
add_library(libscran::topicks ALIAS topicks)

target_include_directories(topicks INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/topicks>)
target_compile_features(topicks INTERFACE cxx_std_17)

# Dependencies
option(TOPICKER_FETCH_EXTERN "Automatically fetch topicks's external dependencies." ON)
if(TOPICKER_FETCH_EXTERN)
    add_subdirectory(extern)
else()
    find_package(ltla_sanisizer 0.1.2 CONFIG REQUIRED)
endif()

target_link_libraries(topicks INTERFACE ltla::sanisizer)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(TOPICKER_TESTS "Build topicks's test suite." ON)
else()
    option(TOPICKER_TESTS "Build topicks's test suite." OFF)
endif()

if(TOPICKER_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()

# Install
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/topicks)

install(TARGETS topicks
    EXPORT topicksTargets)

install(EXPORT topicksTargets
    FILE libscran_topicksTargets.cmake
    NAMESPACE libscran::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicks)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topicksConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicks)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topicksConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libscran_topicksConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topicksConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicks)
