#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUFFER_SIZE 1024
#define MAX_CLIENT 3

int main()
{
    int server_fd, client_fd[MAX_CLIENT] = {};
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int client_count = 0;
    int opt = 1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to initialize TCP socket");
        return -1;
    }

    // Allow reuse of address
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // bind to all interfaces
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept up to MAX_CLIENT clients
    while (client_count < MAX_CLIENT) {
        int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_fd < 0) {
            perror("Accept failed");
            close(server_fd);
            return -1;
        }

        client_fd[client_count++] = new_fd;
        printf("Client %d connected: %s:%d\n",
               client_count,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
    }

    // Close sockets
    for (int i = 0; i < client_count; i++) {
        close(client_fd[i]);
    }
    close(server_fd);

    return 0;
}
