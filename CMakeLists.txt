cmake_minimum_required(VERSION 3.16)
project(nog VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(fmt REQUIRED)
find_package(tomlplusplus REQUIRED)

add_library(nog_lib
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/typechecker/typechecker.cpp
    src/typechecker/strings.cpp
    src/codegen/codegen.cpp
    src/project/project.cpp
    src/project/module.cpp
)
target_link_libraries(nog_lib fmt::fmt tomlplusplus::tomlplusplus)

add_executable(nog src/main.cpp)
target_link_libraries(nog nog_lib)
