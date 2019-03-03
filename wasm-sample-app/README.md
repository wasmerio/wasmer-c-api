# Hello World

## Requirements
- Emscripten: https://emscripten.org/

## Building
`emcc source.c  -Os -s SIDE_MODULE=1 -s EXPORTED_FUNCTIONS="['_hello_wasm']" -o target.wasm`

## Version
The `target.wasm` file was build with the following version:
```
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 1.38.21
clang version 6.0.1  (emscripten 1.38.21 : 1.38.21)
```