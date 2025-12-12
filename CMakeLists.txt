cmake_minimum_required(VERSION 3.16)
project(nog VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(fmt REQUIRED)
find_package(tomlplusplus REQUIRED)

# Fetch llhttp for HTTP parsing
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    llhttp
    URL https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.3.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(llhttp)

# Nog compiler library
add_library(nog_lib
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/typechecker/typechecker.cpp
    src/typechecker/strings.cpp
    src/codegen/codegen.cpp
    src/project/project.cpp
    src/project/module.cpp
    src/docgen/docgen.cpp
    src/stdlib/http.cpp
)
target_link_libraries(nog_lib fmt::fmt tomlplusplus::tomlplusplus)
target_include_directories(nog_lib PUBLIC ${llhttp_SOURCE_DIR}/include)

# Nog compiler executable
add_executable(nog src/main.cpp)
target_link_libraries(nog nog_lib)

# Nog HTTP runtime library (linked by generated programs)
add_library(nog_http STATIC src/runtime/http/http.cpp)
target_include_directories(nog_http PUBLIC
    ${CMAKE_SOURCE_DIR}/src/runtime/http
    ${llhttp_SOURCE_DIR}/include
)
target_link_libraries(nog_http llhttp_static)
set_target_properties(nog_http PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Copy runtime headers and libraries to build directory for development
add_custom_command(TARGET nog_http POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/nog
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/runtime/http/http.hpp
        ${CMAKE_BINARY_DIR}/include/nog/http.hpp
    COMMAND ${CMAKE_COMMAND} -E copy
        ${llhttp_SOURCE_DIR}/include/llhttp.h
        ${CMAKE_BINARY_DIR}/include/llhttp.h
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:llhttp_static>
        ${CMAKE_BINARY_DIR}/lib/
    COMMENT "Copying runtime headers and libraries"
)
