#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "192.168.0.201"   
#define SERVER_PORT 9091
#define BUFFER_SIZE 1024

int main() 
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = "Hello from client!";
    socklen_t addr_len = sizeof(server_addr);

    //Create socket
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("Failed to create socket");
        return -1;
    }

    //Server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);                   //inet_pton() convert  IPv4 and IPv6 addresses from text to binaryform

    //Send message to server
    if (sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len) < 0) {              //sendto() send a message on a socket
        perror("sendto failed");
        close(client_fd);
        return -1;
    }

    printf("Message sent to server: %s\n", buffer);

    //Receive reply (optional)
    memset(buffer, 0, BUFFER_SIZE);
    if (recvfrom(client_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len) < 0) {
        perror("recvfrom failed");
    } else {
        printf("Server replied: %s\n", buffer);
    }

    close(client_fd);
    return 0;
}
