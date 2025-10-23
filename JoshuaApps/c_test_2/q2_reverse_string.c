#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_index_swap(const char *original, char *output) 
{
    int len = strlen(original);                     //len = 11
    int i = 0;

    for (i = 0; i < len; i++) {                     //
        output[i] = original[len - 1 - i];          //[11 - 1 - 0] = d(10); [11 - 1 - 1] = l(9)
    }

    output[len] = '\0';
}

void reverse_backward_copy(const char *original, char *output) 
{
    const char *end = original + strlen(original) - 1;          //string pointer end point at end of "hello world"
    char *out = output;

    while (end >= original) {
        *out++ = *end--;
    }

    *out = '\0';
}

void reverse_string(int method, const char *original, char *output) 
{
    switch (method) {
        case 1:
            reverse_index_swap(original, output);
            break;
        case 2:
            reverse_backward_copy(original, output);
            break;
        default:
            printf("Invalid method.\n");
            output[0] = '\0';
    }
}

int main() 
{
    const char *text = "hello world";
    char reversed[100];
    int method;

    printf("Current String = %c\n", *text);
    printf("Select reverse method:\n");
    printf("1. Index-based\n");
    printf("2. Pointer-based\n");
    printf("Choice: ");

    scanf("%d", &method);

    reverse_string(method, text, reversed);

    printf("Reversed: %s\n", reversed);

    FILE *fp = fopen("reverse_result.txt", "w");
    if (!fp) {
        perror("File write error");
        return 1;
    }

    fputs(reversed, fp);
    fclose(fp);

    return 0;
}
