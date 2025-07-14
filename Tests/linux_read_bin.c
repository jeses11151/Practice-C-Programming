#include <stdio.h>
#include <stdlib.h>

void print_hex_uppercase(const unsigned char *buffer, size_t size) {
    printf("Hex (Uppercase):\n");
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);  // %02X = 2-digit uppercase hex
    }
    printf("\n");
}

void print_hex_lowercase(const unsigned char *buffer, size_t size) {
    printf("Hex (Lowercase):\n");
    for (size_t i = 0; i < size; i++) {
        printf("%02x", buffer[i]);  // %02x = 2-digit lowercase hex
    }
    printf("\n");
}

int main() {
    const char *filename = "test.bin";
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open failed");
        return 1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size <= 0) {
        printf("Empty or invalid file.\n");
        fclose(fp);
        return 1;
    }

    // Allocate buffer
    unsigned char *buffer = malloc(size);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    // Read file into buffer
    fread(buffer, 1, size, fp);
    fclose(fp);

    // Print results
    print_hex_uppercase(buffer, size);
    print_hex_lowercase(buffer, size);

    free(buffer);
    return 0;
}
