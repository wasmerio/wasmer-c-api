# C-API Embedder App Example

[![Actions Status](https://github.com/wasmerio/wasmer-c-api/workflows/wasmer-c-api%20build/badge.svg)](https://github.com/wasmerio/wasmer-c-api/actions)

This repo showcases how to use the [wasmer-runtime-c-api](https://crates.io/crates/wasmer-runtime-c-api/) from C.

The documentation containing installation instructions, and additional examples can be found at the [Wasmer C/C++ Integration Documentation](https://docs.wasmer.io/runtime/c-integration/runtime-c-integration-installation).

See [`wasmer-c-api-example.c`](./wasmer-c-api-example.c) and [`wasmer-wasi-c-api-example.c`](./wasmer-wasi-c-api-example.c) for the example implementations.

The `wasm-sample-app` and `wasmer-wasi-c-api-example` directories contain example C wasm apps to run in the embedder app.

## Requirements
- CMake
- Emscripten to build the sample app

## Building & Running

```bash
# Building the wasm-sample-app
cd wasm-sample-app && emcc source.c  -Os -s SIDE_MODULE=1 -s EXPORTED_FUNCTIONS="['_hello_wasm']" -o target.wasm

# Building the wasm-wasi-sample-app
cd wasmer-wasi-c-api-example && wasicc program.c -Oz -o program.wasm -Wl,--allow-undefined -Wl,--export-all

# Build the embedder api
cmake .
make

# Run the executables
./wasmer-c-api-example
./wasmer-wasi-c-api-example
```

## Reference API Documentation

The Reference API for the Wasmer C API can be found [here](https://wasmerio.github.io/wasmer/c/runtime-c-api/).
