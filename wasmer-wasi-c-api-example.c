#include <stdio.h>
#include "rust-build/src/wasmer-runtime-c-api/lib/runtime-c-api/wasmer.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

static int host_print_called = false;

// Host function that will be imported into the Web Assembly Instance
void host_print(wasmer_instance_context_t *ctx, int32_t ptr, int32_t len)
{
    host_print_called = true;
    const wasmer_memory_t *memory = wasmer_instance_context_memory(ctx, 0);
    uint8_t *mem_bytes = wasmer_memory_data(memory);
    printf("PRINTING FROM THE HOST: \"%.*s\"\n", len, mem_bytes + ptr);
}

// Use the last_error API to retrieve error messages
void print_wasmer_error()
{
    int error_len = wasmer_last_error_length();
    printf("Error len: `%d`\n", error_len);
    char *error_str = malloc(error_len);
    wasmer_last_error_message(error_str, error_len);
    printf("Error str: `%s`\n", error_str);
}

int main()
{
    // Create a new func to hold the parameter and signature
    // of our `host_print` host function
    wasmer_value_tag params_sig[] = {WASM_I32, WASM_I32};
    wasmer_value_tag returns_sig[] = {};
    wasmer_import_func_t *func = wasmer_import_func_new((void (*)(void *)) host_print, params_sig, 2, returns_sig, 0);

    // Create module name for our imports
    // represented in bytes for UTF-8 compatability
    const char *module_name = "env";
    wasmer_byte_array module_name_bytes = { .bytes = (const uint8_t *) module_name,
                                            .bytes_len = strlen(module_name) };

    // Define a function import
    const char *import_name = "host_print";
    wasmer_byte_array import_name_bytes = { .bytes = (const uint8_t *) import_name,
                                            .bytes_len = strlen(import_name) };
    wasmer_import_t func_import = { .module_name = module_name_bytes,
                                    .import_name = import_name_bytes,
                                    .tag = WASM_FUNCTION,
                                    .value.func = func };

    // Define a memory import
    const char *import_memory_name = "memory";
    wasmer_byte_array import_memory_name_bytes = { .bytes = (const uint8_t *) import_memory_name,
                                                   .bytes_len = strlen(import_memory_name) };
    wasmer_import_t memory_import = { .module_name = module_name_bytes,
                                      .import_name = import_memory_name_bytes,
                                      .tag = WASM_MEMORY };
    wasmer_memory_t *memory = NULL;
    wasmer_limit_option_t max = { .has_some = true,
                                  .some = 256 };
    wasmer_limits_t descriptor = { .min = 256,
                                   .max = max };

    wasmer_result_t memory_result = wasmer_memory_new(&memory, descriptor);
    if (memory_result != WASMER_OK)
    {
        print_wasmer_error();
    }
    memory_import.value.memory = memory;

    // Define a global import
    const char *import_global_name = "__memory_base";
    wasmer_byte_array import_global_name_bytes = { .bytes = (const uint8_t *) import_global_name,
                                                   .bytes_len = strlen(import_global_name) };
    wasmer_import_t global_import = { .module_name = module_name_bytes,
                                      .import_name = import_global_name_bytes,
                                      .tag = WASM_GLOBAL };

    wasmer_value_t val = { .tag = WASM_I32,
                           .value.I32 = 1024 };
    wasmer_global_t *global = wasmer_global_new(val, false);
    global_import.value.global = global;

    // Get the WASI base imports
    const char *arg1_val = "--help";
    wasmer_byte_array arg = { .bytes = (const uint8_t *) arg1_val,
                              .bytes_len = strlen(arg1_val) };

    const char *env1_val = "COLOR_OUTPUT=1";
    wasmer_byte_array env = { .bytes = (const uint8_t *) env1_val,
                              .bytes_len = strlen(env1_val) };

    wasmer_import_object_t *import_object =
            wasmer_wasi_generate_import_object(&arg, 1, &env, 1, NULL, 0, NULL, 0);
    // Define an array containing our imports
    wasmer_import_t extra_imports[] = {func_import, global_import, memory_import};

    // Add our custom imports to the WASI ImportObject
    wasmer_import_object_extend(import_object, extra_imports, 3);

    // Read the wasm file bytes
    FILE *file = fopen("wasm-wasi-sample-app/wasi_example.wasm", "r");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    uint8_t *bytes = malloc(len);
    fseek(file, 0, SEEK_SET);
    fread(bytes, 1, len, file);
    fclose(file);

    // Creates a WebAssembly Module from wasm bytes
    wasmer_module_t *module = NULL;
    wasmer_compile(&module, bytes, len);

    // Creates a WebAssembly Instance from a Module and ImportObject
    wasmer_instance_t *instance = NULL;
    wasmer_result_t compile_result = wasmer_module_import_instantiate(&instance, module, import_object);
    printf("Compile result:  %d\n", compile_result);

    if (compile_result != WASMER_OK)
    {
        print_wasmer_error();
    }

    assert(compile_result == WASMER_OK);

    // Call the pre-main function that invokes main, "_start".
    // This is necessary to use the WASI filesystem until WASI finishes support for
    // WASI Wasm modules as libraries
    wasmer_value_t params[] = { { .tag = WASM_I32, .value.I32 = 42 } };
    wasmer_value_t result_one;
    wasmer_value_t results[] = {result_one};
    wasmer_result_t call_result = wasmer_instance_call(instance, "_start", params, 0, results, 1);
    printf("Call result:  %d (fn result %d %d)\n", call_result, results[0].tag, results[0].value.I32);
    assert(call_result == WASMER_OK);
    assert(host_print_called);

    // now call the extra function
    call_result = wasmer_instance_call(instance, "print_random_number_of_length", params, 1, results, 1);
    printf("Call result:  %d (fn result %d %d)\n", call_result, results[0].tag, results[0].value.I32);
    assert(call_result == WASMER_OK);

    // Use *_destroy methods to cleanup as specified in the header documentation
    wasmer_import_func_destroy(func);
    wasmer_global_destroy(global);
    wasmer_memory_destroy(memory);
    wasmer_import_object_destroy(import_object);
    wasmer_instance_destroy(instance);

    return 0;
}
