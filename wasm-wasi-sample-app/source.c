#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

extern void host_print(char *ptr, int len);

__attribute__((used))
int print_random_number_of_length(int size)
{
        while (size > 0) {
                int digit = random() % 10;
                putchar(digit + '0');
                --size;
        }

        printf("\n");
        // this flush is important due to an issue with wasi-libc
        fflush(stdout);
        return 0;
}

int main(int argc, char* argv[])
{
        printf("The program name is %s\n", argv[0]);
        printf("Found %d arguments\n\n", argc);
        // We have to flush to print the above before the host print due to an issue in wasi-libc
        fflush(stdout);
        char* static_str = "Here's a string from the guest";
        host_print(static_str, strlen(static_str));

        srandom(time(NULL));
}
