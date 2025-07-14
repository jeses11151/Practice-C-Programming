#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define METHOD_STRTOK 1
#define METHOD_MANUAL 2

void process_token(const char *token) {
    char *endptr;
    int value = strtol(token, &endptr, 10);
    if (*endptr == '\0') {
        printf("Integer: %d\n", value);
    } else {
        printf("Word: %s\n", token);
    }
}

void split_strtok(const char *input) {
    printf("\n--- Method 1: strtok() ---\n");
    char buffer[200];
    strcpy(buffer, input);  // Save to buffer
    char *token = strtok(buffer, ",");
    while (token != NULL) {
        // Remove leading spaces
        while (*token == ' ') token++;
        process_token(token);
        token = strtok(NULL, ",");
    }
}

void split_manual(const char *input) {
    printf("\n--- Method 2: Manual parsing ---\n");
    char buffer[200];
    strcpy(buffer, input);  // Save to buffer

    char temp[100];
    int j = 0;

    for (int i = 0; ; i++) {
        if (buffer[i] == ',' || buffer[i] == '\0') {
            temp[j] = '\0';
            // Trim leading spaces
            char *trimmed = temp;
            while (*trimmed == ' ') trimmed++;
            process_token(trimmed);
            j = 0;
            if (buffer[i] == '\0') break;
        } else {
            temp[j++] = buffer[i];
        }
    }
}

int main() {
    const char *input = "Embedded, Linux, Software, Engineer, 2025, 06, 02";

    split_strtok(input);
    split_manual(input);

    return 0;
}
