#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "q8_multithread.h"
#include "q8_pipe_transfer.h"
#include "q8_socket_transfer.h"


#define READFILE_PATH "/data/c_test/"
#define READFILE "/data/c_test/readme_ctest.txt"
#define WRITEFILE_PATH "/data/c_test/writetopath/"
#define WRITEFILE "/data/c_test/writetopath/writeme_ctest.txt"

transfer_method_t g_method = METHOD_NONE;

int check_dir() 
{
    struct stat st = {0};

    if (stat(READFILE_PATH, &st) == -1)
    {
        mkdir(READFILE_PATH, 0755);  // rwxr-xr-x (linux only)
        fprintf(stderr, "Path does not exist.\n READ FILE DOES NOT EXIST\n");
        system("touch /data/c_test/readme_ctest.txt");
        system("echo 'read file for C-Test.' > /data/c_test/readme_ctest.txt");
        fprintf(stderr, "Created Path.\n Created read file\n");
        return 0;
    }

    if (stat(WRITEFILE_PATH, &st) == -1)
    {
        mkdir(WRITEFILE_PATH, 0755);  // rwxr-xr-x (linux only)
        fprintf(stderr, "Path does not exist.\n");
        system("touch /data/c_test/writetopath/writeme_ctest.txt");
        return 0;
    }

    return 0;
}

void* read_thread_func(void* arg)
{
    file_path_t *user_path = (file_path_t*) arg;
    FILE * readfile = NULL;

    char buffer[256] = {0};

    printf("read_thread created\n");

    //readfile = fopen(READFILE, "r");
    readfile = fopen(user_path->read_path, "r");
    if(readfile == NULL)
    {
        perror("Failed to open readfile\n");
        return NULL;
    }

    if(g_method == METHOD_PIPE)
    {
        //save file output to some shared memory
        fread(buffer, sizeof(char), sizeof(buffer), readfile);
        fclose(readfile);
        pipe_write(buffer, strlen(buffer));
    }
    else if(g_method == METHOD_SOCKET)
    {
        fread(buffer, sizeof(char), sizeof(buffer), readfile);
        fclose(readfile);
        socket_write(buffer, strlen(buffer));
    }
    
    return NULL;
}

void* write_thread_func(void* arg)
{
    file_path_t *user_path = (file_path_t*) arg;
    FILE * writefile = NULL;

    char buffer[256] = {0};

    printf("write_thread created\n");

    //recieve file from reader
    if(g_method == METHOD_PIPE)
    {
        pipe_read(buffer, sizeof(buffer));
        printf("Read from buffer to readfile: %s", buffer);
    }
    else if(g_method == METHOD_SOCKET)
    {
        socket_read(buffer, sizeof(buffer));
        printf("Read from buffer to readfile: %s", buffer);
    }

    writefile = fopen(user_path->write_path, "a+");

    if(writefile == NULL)
    {
        perror("Failed to open writefile\n");
        return NULL;
    }

    fwrite(buffer, sizeof(char), strlen(buffer), writefile);
    fclose(writefile);

    pipe_deinit();
    socket_deinit();

    return NULL;
}

int check_method(const char *method)
{
    if (strcmp(method, "pipe") == 0) 
    {
        g_method = METHOD_PIPE;
        pipe_init();
    } 
    else if (strcmp(method, "socket") == 0) 
    {
        g_method = METHOD_SOCKET;
        socket_init(1);
    } 
    else 
    {
        fprintf(stderr, "Unknown method '%s'\n", method);
        g_method = METHOD_NONE;
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[])
{
    pthread_t reader_thread;
    pthread_t writer_thread;
    file_path_t file_paths;

    int check_thread = 1;
    int check_args = 1;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file_path> <output_file_path> <method>\n", argv[0]);
        return 1;
    }

    file_paths.read_path = argv[1];
    file_paths.write_path = argv[2]; 

    check_dir();

    if (check_method(argv[3]) != 0){
        fprintf(stderr, "Supported methods: pipe | socket\n");
        return 1;
    }

    check_thread = pthread_create(&reader_thread, NULL, read_thread_func, &file_paths);

    if (check_thread != 0)
    {
        perror("Failed to create Read Thread, Unable to read File\n");
        exit(1);
    }
    else
    {
        printf("Reader Thread successfully created\n");
        check_thread = 1;
    }
    
    check_thread = pthread_create(&writer_thread, NULL, write_thread_func, &file_paths);
    if(check_thread != 0)
    {
        perror("Failed to create Write THread, Unable to write to File.\n");
        exit(1);
    }
    else
    {
        printf("Writer Thread successfully created\n");
        check_thread = 1;
    }

    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);
    
    return 0;
}