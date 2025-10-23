#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENT 5

int main() {
    int server_fd, client_fd[MAX_CLIENT] = {};
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int client_count = 0;
    int counter = 0;
    int opt = 1;
    int i = 0;

    for (counter = 0; counter < MAX_CLIENT; counter++) {
        client_fd[counter] = -1;
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind the socket to the port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept connection
    while (client_count < MAX_CLIENT)
    {
        int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_fd < 0) {
            perror("Accept failed");
            close(server_fd);
            return 1;
        }

        client_fd[client_count++] = new_fd;
        printf
            (
                "Client %d connected: %s:%d\n", 
                client_count,
                inet_ntoa(client_addr.sin_addr), 
                ntohs(client_addr.sin_port)
            );
    }

    //prompt every 30s
    while (1) 
    {
        const char * prompt = "Server : Please Respond (type 'end' to exit)\n";
        
        for(i = 0; i < MAX_CLIENT; i++)
        {
            if (client_fd[i] == -1)
            {
                continue;
            }
            
            //send prompt
            if (send(client_fd[i], prompt, strlen(prompt), 0) < 0)
            {
                perror("Send Failed");
                close(client_fd[i]);
                client_fd[i] = -1;
                continue;
            }
            
            //receive response
            memset(buffer, 0, BUFFER_SIZE);
            int bytes = recv(client_fd[i], buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
            if (bytes > 0)
            {
                buffer[bytes] = '\0';
                printf("Client %d replied: %s", i + 1, buffer);

                if (strcasecmp(buffer, "end\n") == 0 || strcasecmp(buffer, "end\r\n") == 0)
                {
                    printf("Client %d request to end. Closing Conn\n", i + 1);
                    close(client_fd[i]);
                    client_fd[i] = -1;
                }
            }
        }
        sleep(30);
    }


    // Clean up
    for (i = 0; i < MAX_CLIENT; i++)
    {
        if (client_fd[i] > 0)
            close(client_fd[i]);
    }

    close(server_fd);

    return 0;
}
