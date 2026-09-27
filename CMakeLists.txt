cmake_minimum_required(VERSION 3.25)

project(winsh VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(winsh
    src/main.cpp
    src/errors.cpp
    src/builtin/builtin.cpp
    src/builtin/cd.cpp
)

target_include_directories(winsh PRIVATE src)

target_compile_features(winsh PRIVATE cxx_std_26)

set_target_properties(winsh PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
