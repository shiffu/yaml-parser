cmake_minimum_required(VERSION 3.17)
project(parser VERSION 1.0.0)

add_executable(parser)
target_precompile_headers(parser PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h>")

target_sources(parser
    PRIVATE
    main.cpp
)

target_include_directories(parser
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_features(parser PRIVATE cxx_std_17)

target_link_libraries(parser
    PRIVATE
)
