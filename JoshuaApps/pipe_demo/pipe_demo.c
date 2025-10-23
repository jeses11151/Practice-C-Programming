#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define HELLO "Parent wrote HELLO"

static int pipe_fds[2]; // pipe_fds[0] = read end, pipe_fds[1] = write end

int pipe_init()
{
    if (pipe(pipe_fds) < 0) {           //create pipe with 2 descriptors
        perror("pipe creation failed");
        return -1;
    }
    printf("[%s:%s] Successfully initialized pipe\n", __FILE__, __func__);

    return 0;
}

int pipe_write(const void* data, size_t size)
{
    int ret = 0;

    printf("[%s:%s] will write data (%zu bytes):\n", __FILE__, __func__, size);
    printf("%.*s\n", (int)size, (const char*)data);

    ret = write(pipe_fds[1], data, size);
    if (ret < 0) {
        perror("pipe write failed");
    }

    return ret;
}

int pipe_read(void* buffer, size_t size)
{
    int bytes = 0;

    printf("[%s:%s] will read from pipe (up to %zu bytes)\n", __FILE__, __func__, size);

    bytes = read(pipe_fds[0], buffer, size);
    if (bytes > 0) {
        ((char*)buffer)[bytes] = '\0';
        printf("[%s:%s] read data: %s\n", __FILE__, __func__, (char*)buffer);
    } else if (bytes == 0) {
        printf("[%s:%s] pipe read: EOF\n", __FILE__, __func__);
    } else {
        perror("pipe read failed");
    }

    return bytes;
}

void pipe_deinit()
{
    printf("[%s:%s] will close pipe now~~\n", __FILE__, __func__);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

int main()
{
    char buffer[128] = {0};

    if (pipe_init() != 0) {
        fprintf(stderr, "Pipe init failed\n");
        return 1;
    }

    pid_t pid = fork();         //create a child process
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child: read from pipe
        close(pipe_fds[1]); // Close unused write end
        pipe_read(buffer, sizeof(buffer));
        close(pipe_fds[0]); // Close read end after done
        printf("Child got: %s\n", buffer);
        exit(0);
    } else {
        // Parent: write to pipe
        close(pipe_fds[0]); // Close unused read end
        pipe_write(HELLO, strlen(HELLO));
        close(pipe_fds[1]); // Close write end after done
        wait(NULL);         // Wait for child process
    }

    return 0;
}
