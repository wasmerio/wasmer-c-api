extern void print_str(char *ptr, int len);

int hello_wasm()
{
    char *str = "Hello, World!";
    print_str(str, 13);
    return 0;
}