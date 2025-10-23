#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void save_to_buffer(char *destination, const char *source) 
{
    //check memory space allocation
    //setup memory space 
    //check pointers & paremeters are legit
    int len = 0;
    char *dest_space = NULL;

    if(source == NULL){
        perror("Fail to detect source");
        exit(1);
    }

    len = strlen(source);
    dest_space = malloc(len + 1);
    
    if(dest_space == NULL){
        perror("Fail to allocate memory for destination");
        exit(1);
    }

    strcpy(destination, source);
}

void insert_method1(const char *original, const char *insert, int position, char *output) 
{
    int i = 0; 
    int j = 0;
    
    for (i = 0; i < position && original[i] != '\0'; i++) {
        output[i] = original[i];
    }
    for (j = 0; insert[j] != '\0'; j++, i++) {
        output[i] = insert[j];
    }
    for (j = position; original[j] != '\0'; j++, i++) {
        output[i] = original[j];
    }

    output[i] = '\0';
}

void insert_method2(const char *original, const char *insert, int position, char *output) 
{
    char temp1[100];
    char temp2[100];
    char temp_out[100];

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
    char left[100];
    char right[100];
    char temp_out[100];

    strncpy(left, original, position);
    left[position] = '\0';

    strcpy(right, original + position);
    snprintf(temp_out, sizeof(temp_out), "%s%s%s", left, insert, right);
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
    fputs("\n", fp);

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

int main() 
{
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
            insert_method1(original, insert, strlen("Embedded"), result);
            printf("Used Method 1: insert_copy_with_pointer	 copy\n");
            break;
        case 2:
            insert_method2(original, insert, strlen("Embedded"), result);
            printf("Used Method 2: strncpy + strcat\n");
            break;
        case 3:
            insert_method3(original, insert, strlen("Embedded"), result);
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
