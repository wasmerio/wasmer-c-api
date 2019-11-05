# WASI Sample App

## Requirements
- Install [wasienv] (instructions in its README.md) or manually set up Clang with a WASI sysroot

## Building
```sh
# we must pass extra linker flags because we're extending the normal WASI ABI
wasicc program.c -Oz -o program.wasm -Wl,--allow-undefined -Wl,--export-all
```

[wasienv]: https://github.com/wasienv/wasienv
