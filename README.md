# C-API Embedder App Example

This repo showcases how to use the [wasmer-runtime-c-api](https://crates.io/crates/wasmer-runtime/) from C, based on the blogpost: 


See [`wasmer-c-api-example.c`](./wasmer-c-api-example.c) for the example implementation.

The `wasm-sample-app` directory contains an example C wasm app to run in the embedder app.

## Requirements
- CMake
- Emscripten to build the sample app

## Building & Running

```bash
# Building the wasm-sample-app
cd wasm-sample-app && emcc source.c  -Os -s SIDE_MODULE=1 -s EXPORTED_FUNCTIONS="['_hello_wasm']" -o target.wasm

# Build the embedder api
cmake .
make

# Run the executable
./wasmer-c-api-example
```
