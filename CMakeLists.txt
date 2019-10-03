cmake_minimum_required (VERSION 2.6)
project (WasmerCApiExample)
add_executable(wasmer-c-api-example wasmer-c-api-example.c)
add_executable(wasmer-wasi-c-api-example wasmer-wasi-c-api-example.c)

include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/rust-build)
ExternalProject_Add(
        wasmer-runtime-c-api
        DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build
        GIT_REPOSITORY https://github.com/wasmerio/wasmer.git
        GIT_TAG origin/feature/wasi-in-c-api
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cargo build -p wasmer-runtime-c-api
        COMMAND cargo build -p wasmer-runtime-c-api
        BINARY_DIR "${CMAKE_SOURCE_DIR}/rust-build/src/wasmer-runtime-c-api/"
        INSTALL_COMMAND ""
        LOG_BUILD ON)
add_dependencies(wasmer-c-api-example wasmer-runtime-c-api)

set(WASMER_LIB "${CMAKE_SOURCE_DIR}/rust-build/src/wasmer-runtime-c-api/target/debug/libwasmer_runtime_c_api${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(NOT WASMER_LIB)
    message(FATAL_ERROR "wasmer library not found")
endif()

target_link_libraries(wasmer-c-api-example general ${WASMER_LIB})
target_link_libraries(wasmer-wasi-c-api-example general ${WASMER_LIB})
