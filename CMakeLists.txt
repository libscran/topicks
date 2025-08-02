cmake_minimum_required(VERSION 3.14)

project(topicker
    VERSION 0.1.0
    DESCRIPTION "Pick top genes for downstream analyses"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Library
add_library(topicker INTERFACE)
add_library(libscran::topicker ALIAS topicker)

target_include_directories(topicker INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/topicker>)
target_compile_features(topicker INTERFACE cxx_std_17)

# Dependencies
option(TOPICKER_FETCH_EXTERN "Automatically fetch topicker's external dependencies." ON)
if(TOPICKER_FETCH_EXTERN)
    add_subdirectory(extern)
else()
    find_package(ltla_sanisizer 0.1.0 CONFIG REQUIRED)
endif()

target_link_libraries(topicker INTERFACE ltla::sanisizer)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(TOPICKER_TESTS "Build topicker's test suite." ON)
else()
    option(TOPICKER_TESTS "Build topicker's test suite." OFF)
endif()

if(TOPICKER_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()

# Install
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/topicker)

install(TARGETS topicker
    EXPORT topickerTargets)

install(EXPORT topickerTargets
    FILE libscran_topickerTargets.cmake
    NAMESPACE libscran::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicker)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topickerConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicker)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topickerConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libscran_topickerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_topickerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_topicker)
