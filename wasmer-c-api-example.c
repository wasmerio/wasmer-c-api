#include <stdio.h>
#include "rust-build/src/wasmer-runtime-c-api/lib/runtime-c-api/wasmer.h"
#include <assert.h>
#include <stdint.h>

static print_str_called = false;

// Host function that will be imported into the Web Assembly Instance
void print_str(wasmer_instance_context_t *ctx, int32_t ptr, int32_t len)
{
    print_str_called = true;
    wasmer_memory_t *memory = wasmer_instance_context_memory(ctx, 0);
    uint32_t mem_len = wasmer_memory_length(memory);
    uint8_t *mem_bytes = wasmer_memory_data(memory);
    printf("%.*s", mem_len, mem_bytes);
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
    // of our `print_str` host function
    wasmer_value_tag params_sig[] = {WASM_I32, WASM_I32};
    wasmer_value_tag returns_sig[] = {};
    wasmer_func_t *func = wasmer_func_new(print_str, params_sig, 2, returns_sig, 0);

    // Create module and import names for our import
    // represented in bytes for UTF-8 compatability
    char *module_name = "env";
    wasmer_byte_array module_name_bytes;
    module_name_bytes.bytes = module_name;
    module_name_bytes.bytes_len = strlen(module_name);
    char *import_name = "print_str";
    wasmer_byte_array import_name_bytes;
    import_name_bytes.bytes = import_name;
    import_name_bytes.bytes_len = strlen(import_name);

    // Define an array containing our import
    wasmer_import_t import;
    import.module_name = module_name_bytes;
    import.import_name = import_name_bytes;
    import.tag = WASM_FUNCTION;
    import.value.func = func;
    wasmer_import_t imports[] = {import};

    // Read the wasm file bytes
    FILE *file = fopen("wasm-sample-app/target/wasm32-unknown-unknown/release/wasm_sample_app.wasm", "r");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    uint8_t *bytes = malloc(len);
    fseek(file, 0, SEEK_SET);
    fread(bytes, 1, len, file);
    fclose(file);

    // Creates a WebAssembly Instance from wasm bytes and imports
    wasmer_instance_t *instance = NULL;
    wasmer_result_t compile_result = wasmer_instantiate(&instance, bytes, len, imports, 1);
    printf("Compile result:  %d\n", compile_result);
    if (compile_result != WASMER_OK)
    {
        print_wasmer_error();
    }
    assert(compile_result == WASMER_OK);

    // Call the exported "hello_wasm" function of our instance
    wasmer_value_t params[] = {};
    wasmer_value_t results[] = {};
    wasmer_result_t call_result = wasmer_instance_call(instance, "hello_wasm", params, 0, results, 0);
    printf("Call result:  %d\n", call_result);
    assert(call_result == WASMER_OK);
    assert(print_str_called);

    // Use *_destroy methods to cleanup as specified in the header documentation
    printf("Destroy instance\n");
    wasmer_instance_destroy(instance);

    return 0;
}
