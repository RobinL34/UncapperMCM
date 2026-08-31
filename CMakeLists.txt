cmake_minimum_required(VERSION 3.21)

project(
    UncapperMCM
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(
    ${PROJECT_NAME}
    SOURCES
        src/main.cpp
        src/UncapperAPI.cpp
        src/Settings.cpp
        src/Serialization.cpp
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
        /utf-8
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
        src/PCH.h
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        include
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME "UncapperMCM"
)