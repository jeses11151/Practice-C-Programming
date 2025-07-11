#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    //initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 2. Create socket

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    // 3. Bind to local port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // all interfaces
    server_addr.sin_port = htons(PORT);
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("UDP Echo Server is listening on port %d...\n", PORT);

    // 4. Receive & echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recvfrom(sock, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&client_addr, &client_len);

        buffer[bytes] = '\0';
        printf("Received from %s:%d -> %s",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        sendto(sock, buffer, bytes, 0,
            (struct sockaddr*)&client_addr, client_len);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}