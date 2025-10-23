#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "q8_socket_transfer.h"  // from libcomms.so
//#include "libcomms.so"

#define BUFFER_SIZE 1024

enum ErrorCode {
    ERR_UNKNOWN_COMMAND = 1001,
    ERR_WRITE_FAILED    = 1002,
    ERR_FILE_OPEN_FAIL  = 1003
};

int main() {
    char buffer[BUFFER_SIZE] = {};
    FILE *outfile = NULL;
    int receiving_data = 0;
    int i = 0;
    int init_check = 1;

    init_check = socket_init(2); //socket_tcp_mode
    if (init_check != 0)
    {
        perror("Failed to init socket TCP. \n exit program\n");
        return 1;
    }

    printf("Starting wait client loop\n");
    while (1) 
    {   
        printf("entering loop. times: %d \n", ++i);

        memset(buffer, 0, sizeof(buffer));
        
        int bytes = socket_read(buffer, sizeof(buffer) - 1);
        if (bytes <= 0) continue;

        buffer[bytes] = '\0';
        printf("[DEBUG] Raw buffer (%d bytes): '%s'\n", bytes, buffer);

        // buffer[strcspn(buffer, "\r\n")] = 0;

        printf("[SERVER] Received: '%s'\n", buffer);

        // Handle Start:<filename>
        if (strncmp(buffer, "Start:", 6) == 0) {
            const char *filepath = buffer + 6; //start after "start:"

            printf("[SERVER] Received: '%s'\n", buffer);

            if (outfile) 
                fclose(outfile);  // close previous if any

            printf("%s", filepath);
            outfile = fopen(filepath, "w");
            if (!outfile) {
                char err[64];
                snprintf(err, sizeof(err), "ErrorCode:%d", ERR_FILE_OPEN_FAIL);
                socket_write(err, strlen(err));
                //用完文件要关
                continue;
            }

            printf("[SERVER] Opened file: %s\n", filepath);
            socket_write(filepath, strlen(filepath));   //server reply

            receiving_data = 1;

            socket_write("Ready", strlen("Ready"));
        } 
        // Handle End
        else if (strcmp(buffer, "End") == 0) {
            if (outfile) 
                fclose(outfile);
            outfile = NULL;
            receiving_data = 0;

            socket_write("Finished", strlen("Finished"));
            break;
        } 
        // Data
        else if (receiving_data && outfile) {
            if (fputs(buffer, outfile) == EOF) {//尽量用 write / read，fput 可以被覆盖到
                char err[32];
                snprintf(err, sizeof(err), "ErrorCode:%d", ERR_WRITE_FAILED);
                socket_write(err, strlen(err));
            }
        } 
        // Unknown command
        else {
            char err[32];
            snprintf(err, sizeof(err), "ErrorCode:%d", ERR_UNKNOWN_COMMAND);
            socket_write(err, strlen(err));
        }
    }

    if (outfile) 
        fclose(outfile);

    socket_deinit();
    return 0;
}
