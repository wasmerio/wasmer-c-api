# C-API Embedder App Example

This repo showcases how to use the [wasmer-runtime-c-api](https://crates.io/crates/wasmer-runtime/) from C, based on the blogpost: 


See [`wasmer-c-api-example.c`](./wasmer-c-api-example.c) for the example implementation.

The `wasm-sample-app` directory contains an example rust wasm app to run in the embedder app.

## Requirements
- CMake
- Rust target `wasm32-unknown-unknown` - install using `rustup target add wasm32-unknown-unknown`

## Building & Running

```bash
# Building the wasm-sample-app
cd wasm-sample-app && cargo build --release && cd ..

# Build the embedder api
cmake .
make

# Run the executable
./wasmer-c-api-example
```
