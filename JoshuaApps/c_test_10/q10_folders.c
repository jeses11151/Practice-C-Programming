#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "q8_socket_transfer.h"  // from libcomms.so

#define BUFFER_SIZE 1024

enum ErrorCode {
    ERR_UNKNOWN_COMMAND = 1001,
    ERR_WRITE_FAILED    = 1002,
    ERR_FILE_OPEN_FAIL  = 1003,
    ERR_MKDIR_FAIL      = 1004
};

static FILE *outfile = NULL;
static int receiving_data = 0;

void create_folder(const char *path) {
    if (mkdir(path, 0755) != 0) {
        perror("[SERVER] mkdir failed");
    } else {
        printf("[SERVER] Folder created: %s\n", path);
    }
}

int main() {
    char buffer[BUFFER_SIZE] = {};
    int init_check = socket_init(2); // TCP mode
    if (init_check != 0) {
        perror("Failed to init socket TCP.\n");
        return 1;
    }

    printf("Starting server for folder transfer...\n");
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = socket_read(buffer, sizeof(buffer));
        if (bytes <= 0) continue;

        // Commands
        if (strncmp(buffer, "StartFolder:", 12) == 0) {
            const char *foldername = buffer + 12;
            create_folder(foldername);
            socket_write("FolderReady", strlen("FolderReady"));
        }
        else if (strncmp(buffer, "StartFile:", 10) == 0) {
            const char *filepath = buffer + 10;
            if (outfile) fclose(outfile);
            outfile = fopen(filepath, "wb");
            if (!outfile) {
                char err[64];
                snprintf(err, sizeof(err), "ErrorCode:%d", ERR_FILE_OPEN_FAIL);
                socket_write(err, strlen(err));
                continue;
            }
            receiving_data = 1;
            socket_write("FileReady", strlen("FileReady"));
        }
        else if (strcmp(buffer, "EndFile") == 0) {
            if (outfile) fclose(outfile);
            outfile = NULL;
            receiving_data = 0;
            socket_write("FileFinished", strlen("FileFinished"));
        }
        else if (strcmp(buffer, "EndFolder") == 0) {
            if (outfile) fclose(outfile);
            outfile = NULL;
            receiving_data = 0;
            socket_write("FolderFinished", strlen("FolderFinished"));
            break;
        }
        // Binary data
        else if (receiving_data && outfile) {
            if (fwrite(buffer, 1, bytes, outfile) != (size_t)bytes) {
                char err[32];
                snprintf(err, sizeof(err), "ErrorCode:%d", ERR_WRITE_FAILED);
                socket_write(err, strlen(err));
            }
        }
        else {
            char err[32];
            snprintf(err, sizeof(err), "ErrorCode:%d", ERR_UNKNOWN_COMMAND);
            socket_write(err, strlen(err));
        }
    }

    if (outfile) fclose(outfile);
    socket_deinit();
    return 0;
}
