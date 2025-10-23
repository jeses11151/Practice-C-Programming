#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/home/joshua/tmp/mysocket"
#define BUFFER_SIZE 128

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure socket address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send message
    const char *msg = "Hello from client!";
    write(client_fd, msg, strlen(msg));

    // Read reply
    memset(buffer, 0, BUFFER_SIZE);
    if (read(client_fd, buffer, BUFFER_SIZE) > 0) {
        printf("Server replied: %s\n", buffer);
    }

    close(client_fd);
    return 0;
}
