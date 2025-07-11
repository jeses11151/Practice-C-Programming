#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <arpa/inet.h>     //linux/unix socket
//#include <sys/socket.h>    //linux/unix socket
#include <winsock2.h>
#include <ws2tcpip.h> // For inet_ntoa and other functions

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //bind the socket to the port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    printf("Server is listening on port %d...\n", PORT);

    // Accept a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == INVALID_SOCKET) {
        fprintf(stderr, "Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //Echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) break;

        printf("Received: %s\n", buffer);
        send(client_fd, buffer, bytes_received, 0);
    }

    //clean up
    printf("Closing connection...\n");
    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();

    return 0;

}