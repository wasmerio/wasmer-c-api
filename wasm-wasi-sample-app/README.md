# WASI Sample App

## Requirements
- describe WASI clang installation process here

## Building
```sh
clang source.c -Os -o wasi_example.wasm --target=wasm32-wasi -Wl,--allow-undefined -Wl,--export-all
```
