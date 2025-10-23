#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void save_to_buffer(char *destination, const char *source) 
{
    strcpy(destination, source);
}

void insert_method1(const char *original, const char *insert, int position, char *output) 
{
    int i = 0;
    for (; i < position && original[i] != '\0'; i++) {
        output[i] = original[i];
    }
    for (int j = 0; insert[j] != '\0'; j++, i++) {
        output[i] = insert[j];
    }
    for (int j = position; original[j] != '\0'; j++, i++) {
        output[i] = original[j];
    }
    output[i] = '\0';
}

void insert_method2(const char *original, const char *insert, int position, char *output) 
{
    char temp1[100], temp2[100], temp_out[100];
    strncpy(temp1, original, position);
    temp1[position] = '\0';
    strcpy(temp2, original + position);
    strcpy(temp_out, temp1);
    strcat(temp_out, insert);
    strcat(temp_out, temp2);
    save_to_buffer(output, temp_out);
}

void insert_method3(const char *original, const char *insert, int position, char *output) 
{
    char left[100], right[100], temp_out[100];
    strncpy(left, original, position);
    left[position] = '\0';
    strcpy(right, original + position);
    sprintf(temp_out, "%s%s%s", left, insert, right);
    save_to_buffer(output, temp_out);
}

void save_to_file(const char *filename, const char *data) 
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to write to file");
        exit(1);
    }
    fputs(data, fp);
    fclose(fp);
}

int check_dir(const char *dirpath) 
{
    struct stat st = {0};
    if (stat(dirpath, &st) == -1) {
        return mkdir(dirpath, 0755);  // rwxr-xr-x (linux only)
    }
    return 0;
}

int main() {
    const char *original = "Embedded Software Engineer";
    const char *insert = " Linux";
    char result[200];
    int method;

    printf("Select insertion method:\n");
    printf("1. insert_copy_with_pointer	 copy (index-based)\n");
    printf("2. String functions (strncpy + strcat)\n");
    printf("3. Formatted printing (sprintf)\n");
    printf("Enter method number: ");
    scanf("%d", &method);

    switch (method) {
        case 1:
            insert_method1(original, insert, 8, result);
            printf("Used Method 1: insert_copy_with_pointer	 copy\n");
            break;
        case 2:
            insert_method2(original, insert, 8, result);
            printf("Used Method 2: strncpy + strcat\n");
            break;
        case 3:
            insert_method3(original, insert, 8, result);
            printf("Used Method 3: sprintf\n");
            break;
        default:
            printf("Invalid method.\n");
            return 1;
    }

    printf("Result: %s\n", result);
    check_dir("/data/test");
    save_to_file("/data/test/insert_linux.txt", result);
    printf("Saved to insert_linux.txt\n");

    return 0;
}
