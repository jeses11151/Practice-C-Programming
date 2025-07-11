#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // For inet_ntoa and other functions

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    //Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //Connect to the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Convert IP address from string to binary form

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Connected to server %s on port %d\n", SERVER_IP, PORT);

    //communication loop
    while (1) {
        printf("Enter message to send (or 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        if (strcmp(buffer, "exit") == 0) {
            break; // Exit the loop if user types 'exit'
        }

        // Send message to server
        if (send(sockfd, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            fprintf(stderr, "Send failed. Error Code: %d\n", WSAGetLastError());
            break;
        }

        // Receive response from server
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            fprintf(stderr, "Receive failed. Error Code: %d\n", WSAGetLastError());
            break;
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received string
        printf("Server response: %s\n", buffer);
    }

    // Clean up
    closesocket(sockfd);
    WSACleanup();
    printf("Connection closed.\n");
    return 0;
}