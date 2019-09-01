cmake_minimum_required (VERSION 2.6)
project (WasmerCApiExample)
add_executable(wasmer-c-api-example wasmer-c-api-example.c)

include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/rust-build)
ExternalProject_Add(
        wasmer-runtime-c-api
        DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build
        GIT_REPOSITORY https://github.com/wasmerio/wasmer.git
        GIT_TAG master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cargo build -p wasmer-runtime-c-api
        COMMAND cargo build -p wasmer-runtime-c-api
        BINARY_DIR "${CMAKE_SOURCE_DIR}/rust-build/src/wasmer-runtime-c-api/"
        INSTALL_COMMAND ""
        LOG_BUILD ON)
add_dependencies(wasmer-c-api-example wasmer-runtime-c-api)

find_library(
        WASMER_LIB NAMES libwasmer_runtime_c_api.dylib libwasmer_runtime_c_api.so wasmer_runtime_c_api.dll
        PATHS ${CMAKE_SOURCE_DIR}/rust-build/src/wasmer-runtime-c-api/target/debug/
)

if(NOT WASMER_LIB)
    message(FATAL_ERROR "wasmer library not found")
endif()

target_link_libraries(wasmer-c-api-example general ${WASMER_LIB})
