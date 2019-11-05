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
        putchar('\n');

        return 0;
}

int main(int argc, char* argv[])
{
        printf("The program name is %s\n", argv[0]);
        printf("Found %d arguments\n\n", argc);
        char* static_str = "Here's a string from the guest";
        host_print(static_str, strlen(static_str));

        srandom(time(NULL));
}
