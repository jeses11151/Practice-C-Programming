#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define PORT 9091
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    int opt = 1, bytes = 0;
    int loop_counter = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    //Create udp server
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);     
    if(server_fd < 0){
        printf("Failed to create UDP server\n");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        printf("Failed to set socket");
        close(server_fd);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server_addr.sin_port = htons(PORT);

    //Bind to local port
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Failed to bind to port\n");
        close(server_fd);
        return -1;
    }

    while (1) {
        loop_counter++;
        printf("loop number: %d\n", loop_counter);
        bytes = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        if (bytes < BUFFER_SIZE) {
            buffer[bytes] = '\0';
        } else { 
            buffer[BUFFER_SIZE-1] = '\0';
        }

        printf("Received from %s:%d -> %s\n",
                inet_ntoa(client_addr.sin_addr),        //client addr
                ntohs(client_addr.sin_port),            //client port
                buffer);

        sendto(server_fd, buffer, bytes, 0, (struct sockaddr *)&client_addr, client_len);         //sendto() send a message on a socket

        printf("UDP Echo Server is listening on port %d...\n", PORT);
    }

    close(server_fd);
    return 0;
}

