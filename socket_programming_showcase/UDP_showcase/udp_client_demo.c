// udp_echo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Init Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 2. Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // 3. Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // 4. Send-receive loop
    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));

        int addr_len = sizeof(server_addr);
        int bytes = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&server_addr, &addr_len);
        buffer[bytes] = '\0';
        printf("Echoed back: %s", buffer);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
