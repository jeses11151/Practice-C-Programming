#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_FILES 100
#define MAX_NAME_LEN 256
#define DIR_PATH "/data/test"

long filesize_ftell(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

long filesize_count(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    long count = 0;
    while (fgetc(fp) != EOF) count++;
    fclose(fp);
    return count;
}

long filesize_stat(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) return -1;
    return st.st_size;
}

int list_files(const char *dirpath, char filelist[][MAX_NAME_LEN]) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror("opendir failed");
        return -1;
    }

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (entry->d_type == DT_REG) {
            snprintf(filelist[count], MAX_NAME_LEN, "%s/%s", dirpath, entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}

int main() {
    char files[MAX_FILES][MAX_NAME_LEN];
    int file_count = list_files(DIR_PATH, files);
    if (file_count <= 0) {
        printf("No files found in %s\n", DIR_PATH);
        return 1;
    }

    printf("Files in %s:\n", DIR_PATH);
    for (int i = 0; i < file_count; i++) {
        printf("[%d] %s\n", i + 1, files[i]);
    }

    printf("Select file number to check size: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > file_count) {
        printf("Invalid choice.\n");
        return 1;
    }

    const char *selected_file = files[choice - 1];
    printf("\n--- File: %s ---\n", selected_file);
    printf("Method 1 (ftell)       : %ld bytes\n", filesize_ftell(selected_file));
    printf("Method 2 (byte count)  : %ld bytes\n", filesize_count(selected_file));
    printf("Method 3 (stat)        : %ld bytes\n", filesize_stat(selected_file));

    return 0;
}
