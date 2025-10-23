#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "q8_pipe_transfer.h"

static int pipe_fds[2];


int pipe_init()
{    
    int i = 1;
    i = pipe(pipe_fds);
    if (i < 0) {
        perror("pipe creation failed");
        return -1;
    }else{ 
        printf("[%s:%s] Successfully initialized pipe\n", __FILE__, __func__);
    }

    return 0;
}


int pipe_write(const void* data, size_t size)
{
    int ret = 1;

    printf("[%s:%s] will write data (%zu bytes):\n", __FILE__, __func__, size); //%zu = length of byte (C99)
    printf("%.*s\n", (int)size, (const char*)data);

    ret = write(pipe_fds[1], data, size);
    if (ret < 0) {
        perror("pipe write failed");
    }

    return ret;
}

int pipe_read(void * buffer,  size_t size)
{
    printf("[%s:%s] will read from pipe (up to %zu bytes)\n", __FILE__, __func__, size);

    int bytes = read(pipe_fds[0], buffer, size);

    if (bytes > 0) {
        ((char*)buffer)[bytes] = '\0';  
        printf("[%s:%s] read data: %s\n", __FILE__, __func__, (char*)buffer);
    }else if (bytes == 0) {
        printf("[%s:%s] pipe read: EOF\n", __FILE__, __func__);
    }else{
        perror("pipe read failed");
    }
    //return read(pipe_fds[0], buffer, size);

    return bytes;
}

void pipe_deinit()
{
    printf("[%s:%s] will close pipe now~~\n", __FILE__, __func__);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}


//pipe test
// int main(){
//     char buffer[128] = {0};
//     if(pipe_init(NULL) != 0){
//         fprintf(stderr, "Pipe init failed\n");
//         return 1;
//     }

//     pipe_write("Hello pipe", 11);
//     pipe_read(buffer, sizeof(buffer));
//     printf("Got from pipe: %s\n", buffer);

//     pipe_deinit();
//     return 0;
// }