#include <stdio.h>   // Required for all the functions listed
#include <stdlib.h>  // Required for exit()
#include <string.h>  // Required for strlen() and strcmp()

// Define a simple structure to demonstrate fread/fwrite
typedef struct {
    int id;
    char name[20];
    float value;
} DataEntry;

int main() {
    // --- FILE I/O FUNCTIONS ---

    // 1. fopen() - Opens a file for writing
    FILE* file_ptr;
    const char* filename = "example.txt";
    const char* binary_filename = "binary_example.bin";
    int char_read;
    char buffer[100];
    char line_buffer[256];
    int int_val;
    float float_val;
    size_t items_read;
    DataEntry data_out = {101, "Test Item", 123.45f};
    DataEntry data_in;

    printf("--- Demonstrating File I/O Functions ---\n");

    // Open file for writing text
    file_ptr = fopen(filename, "w"); // "w" for write, creates/truncates file
    if (file_ptr == NULL) {
        perror("Error opening example.txt for writing");
        return 1; // Indicate error
    }

    // 2. fprintf() - Writes a formatted string into a file
    fprintf(file_ptr, "Hello, C File I/O!\n");
    fprintf(file_ptr, "This is line %d with a float %.2f.\n", 1, 10.50);

    // 3. fputc() - Writes a character into a file
    fputc('A', file_ptr);
    fputc('\n', file_ptr);

    // 4. fputs() - Writes a string into a file
    fputs("Another string using fputs.\n", file_ptr);

    // 5. fclose() - Closes the file
    if (fclose(file_ptr) == EOF) {
        perror("Error closing example.txt after writing");
        return 1;
    }
    printf("Wrote data to '%s'.\n", filename);

    // Open file for reading text
    file_ptr = fopen(filename, "r"); // "r" for read
    if (file_ptr == NULL) {
        perror("Error opening example.txt for reading");
        return 1;
    }
    printf("\nReading from '%s':\n", filename);

    // 6. fgetc() - Reads a character from a file
    printf("First character using fgetc(): ");
    char_read = fgetc(file_ptr);
    if (char_read != EOF) {
        putchar(char_read); // Display the character
    }
    putchar('\n');

    // 7. fgets() - Reads a line from a file
    // Note: fgetc() moved the indicator, so fgets starts from the second char of first line
    // Let's reset the file pointer for clear demonstration
    fseek(file_ptr, 0, SEEK_SET); // Rewind to the beginning

    printf("First line using fgets(): ");
    if (fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL) {
        printf("%s", line_buffer);
    }

    // 8. fscanf() - Reads formatted data from a file
    // Assuming the second line in the file is "This is line X with a float Y.Y."
    // We already read the first line with fgets, so now we are at the second line
    printf("Reading formatted data using fscanf():\n");
    char temp_str[20]; // To capture "This", "is", "line"
    if (fscanf(file_ptr, "%s %s %s %d %s %s %.2f.", temp_str, temp_str, temp_str, &int_val, temp_str, temp_str, &float_val) == 7) {
        printf("  Read: int_val=%d, float_val=%.2f\n", int_val, float_val);
    } else {
        printf("  Failed to read expected formatted data with fscanf.\n");
        // Clear potential error flag for ferror later
        clearerr(file_ptr);
    }


    // 9. fseek() and ftell() - Move and get position indicator
    long current_pos = ftell(file_ptr);
    printf("Current position after fscanf: %ld bytes\n", current_pos);

    // Move to 10 bytes from the beginning
    fseek(file_ptr, 10, SEEK_SET);
    current_pos = ftell(file_ptr);
    printf("Position after fseek(10, SEEK_SET): %ld bytes\n", current_pos);

    // Read a character at this new position
    char_read = fgetc(file_ptr);
    if (char_read != EOF) {
        printf("Character at position %ld: '%c'\n", current_pos, char_read);
    }

    // Rewind to the beginning
    fseek(file_ptr, 0, SEEK_SET);

    // 10. feof() - Check for end of file
    printf("Checking feof() before reading to end: %s\n", feof(file_ptr) ? "True" : "False");
    printf("Reading character by character until EOF:\n");
    while ((char_read = fgetc(file_ptr)) != EOF) {
        putchar(char_read);
    }
    printf("\n"); // Newline after printing file content

    printf("Checking feof() after reading to end: %s\n", feof(file_ptr) ? "True" : "False");

    // 11. ferror() - Check for errors (let's try to simulate one for demonstration)
    printf("Checking ferror(): %s\n", ferror(file_ptr) ? "True" : "False");
    // (A real error might be trying to write to a read-only file or device)
    // For now, it should be False unless an actual error occurred during operations above.

    // Close the file after reading
    if (fclose(file_ptr) == EOF) {
        perror("Error closing example.txt after reading");
        return 1;
    }
    printf("Closed '%s'.\n", filename);

    // --- Binary File I/O (fread/fwrite) ---
    printf("\n--- Demonstrating Binary File I/O (fread/fwrite) ---\n");

    // Open file for writing binary data
    file_ptr = fopen(binary_filename, "wb"); // "wb" for write binary
    if (file_ptr == NULL) {
        perror("Error opening binary_example.bin for writing");
        return 1;
    }

    // 12. fwrite() - Writes data from a block of memory into a file
    if (fwrite(&data_out, sizeof(DataEntry), 1, file_ptr) != 1) {
        perror("Error writing DataEntry to binary file");
        fclose(file_ptr);
        return 1;
    }
    printf("Wrote DataEntry {id=%d, name='%s', value=%.2f} to '%s'.\n",
           data_out.id, data_out.name, data_out.value, binary_filename);

    fclose(file_ptr);

    // Open file for reading binary data
    file_ptr = fopen(binary_filename, "rb"); // "rb" for read binary
    if (file_ptr == NULL) {
        perror("Error opening binary_example.bin for reading");
        return 1;
    }

    // 13. fread() - Reads data from a file and writes it into a block of memory
    items_read = fread(&data_in, sizeof(DataEntry), 1, file_ptr);
    if (items_read == 1) {
        printf("Read DataEntry {id=%d, name='%s', value=%.2f} from '%s'.\n",
               data_in.id, data_in.name, data_in.value, binary_filename);
    } else {
        printf("Failed to read DataEntry from binary file. Items read: %zu\n", items_read);
    }

    fclose(file_ptr);

    // --- Console I/O Functions ---
    printf("\n--- Demonstrating Console I/O Functions ---\n");

    // 14. printf() - Writes a formatted string to the console
    printf("Hello from printf()! The answer is %d.\n", 42);

    // 15. puts() - Outputs a string to the console (adds newline)
    puts("This is a string from puts().");

    // 16. putchar() - Outputs a single character to the console
    printf("Printing a char using putchar(): ");
    putchar('X');
    putchar('\n');

    // 17. getchar() - Reads one character of user input
    // getc() - The same as fgetc() but often used with stdin/stdout
    // (getchar() is equivalent to getc(stdin))
    printf("Enter a character (then press Enter): ");
    char_read = getchar(); // Reads one character from stdin
    printf("You entered (via getchar()): '%c' (ASCII: %d)\n", char_read, char_read);
    // getchar() often leaves the newline character in the buffer, so subsequent reads might pick it up.
    // Let's clear the input buffer.
    while (getchar() != '\n' && getchar() != EOF); // Consume remaining characters including newline

    // 18. scanf() - Reads formatted data from user input
    printf("Enter an integer and a float (e.g., 10 3.14): ");
    if (scanf("%d %f", &int_val, &float_val) == 2) { // Expecting 2 items
        printf("You entered (via scanf()): int=%d, float=%.2f\n", int_val, float_val);
    } else {
        printf("Failed to read expected integer and float.\n");
        // Clear input buffer in case of bad input
        while (getchar() != '\n' && getchar() != EOF);
    }
    // Clear input buffer after scanf, as it might leave newline
    while (getchar() != '\n' && getchar() != EOF);


    // 19. getc() - Reads a character from an input stream (stdin)
    printf("Enter another character (then press Enter): ");
    char_read = getc(stdin); // Equivalent to getchar() in behavior for stdin
    printf("You entered (via getc(stdin)): '%c' (ASCII: %d)\n", char_read, char_read);
    while (getchar() != '\n' && getchar() != EOF);


    // 20. putc() - Writes a character to an output stream (stdout)
    // (putc() is equivalent to fputc() when stream is stdout)
    printf("Printing a char using putc(char, stdout): ");
    putc('Y', stdout);
    putc('\n', stdout);


    // --- String I/O Functions (Memory-based) ---
    printf("\n--- Demonstrating String I/O Functions (Memory-based) ---\n");
    char str_buffer[256];
    int num1, num2;

    // 21. sprintf() - Writes a formatted string into a char array
    sprintf(str_buffer, "This is an integer: %d and a float: %.1f", 123, 45.6f);
    printf("sprintf() output to buffer: '%s'\n", str_buffer);

    // 22. snprintf() - Memory-safe version of sprintf()
    // It's crucial to provide the buffer size to prevent overflow
    char safe_buffer[99];
    int chars_written = snprintf(safe_buffer, sizeof(safe_buffer), "potential overflow if not for snprintf's size limit: %d", 999);
    printf("snprintf() output to buffer (size %zu): '%s'\n", sizeof(safe_buffer), safe_buffer);
    printf("snprintf() returned characters attempted to write (may be > buffer_size): %d\n", chars_written);
    // If chars_written >= sizeof(safe_buffer), it means the output was truncated.
    if (chars_written >= sizeof(safe_buffer)) {
        printf("  Note: Output was truncated by snprintf.\n");
    }

    // 23. sscanf() - Reads formatted data from a char array
    const char* data_string = "Values: 100 200";
    if (sscanf(data_string, "Values: %d %d", &num1, &num2) == 2) {
        printf("sscanf() read from '%s': num1=%d, num2=%d\n", data_string, num1, num2);
    } else {
        printf("sscanf() failed to parse '%s'.\n", data_string);
    }

    // Clean up created files
    remove(filename);
    remove(binary_filename);
    printf("\nCleaned up '%s' and '%s'.\n", filename, binary_filename);

    return 0; // Indicate success
}