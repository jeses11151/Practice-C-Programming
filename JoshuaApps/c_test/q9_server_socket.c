#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#define PORT 9090
#define BUFFER_SIZE 1024
#define MAX_CLIENT 5

void trim_string(char *str);

int main() 
{
    int server_fd; 
    int client_fd = -1;
    // int client_fd[MAX_CLIENT];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char *dest_path = {0};
    int client_count = 0;
    int counter = 0;
    int opt = 1;
    int i = 0;
    int bytes = 0;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
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

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if(client_fd < 0)
    {
        perror("Accept Failed\n");  
        close(server_fd);           
        return 1;
    }

    printf("Client Connected: %s:%d\n",
            inet_ntoa(client_addr.sin_addr),  //convert IP addr to string
            ntohs(client_addr.sin_port)       //Convert network byte order to host order
        );

//============================= Start File Transfer =====================================
    
    memset(buffer, 0, sizeof(buffer));

    bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
    {
        printf("Failed to read Command\n");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    buffer[bytes] = '\0';               //set last as NULL
    trim_string(buffer);
    if (strncmp(buffer, "Start:", 6) == 0) {
        // const char *filepath = buffer + 6; //start after "start:"
        dest_path = buffer + 6;
        trim_string(dest_path);

        printf("Will write file to: %s\n", dest_path);

        if (send(client_fd, "Ready\n", strlen("Ready\n"), 0) < 0) 
        {
            perror("Send Ready failed");
        }

        printf("[SERVER] Received: '%s'\n", buffer);

        FILE *received_file = fopen(dest_path, "w");
        if (!received_file)
        {
            perror("Cannot open file to write buffer\n");
            return -1;
        }
        else
        {
            while (1) 
            {
                memset(buffer, 0, sizeof(buffer));
                bytes = recv(client_fd, buffer, sizeof(buffer), 0);
                if (bytes <= 0)
                {
                    break;
                }
                buffer [bytes] = '\0';
                trim_string(buffer);
                if (strcasecmp(buffer, "End") == 0)
                {
                    break;
                }
                fprintf(received_file, "%s\n", buffer);
            }
            fclose(received_file);
        }

        if (send(client_fd, "Finished\n", strlen("Finished\n"), 0) < 0)
        {
            perror("Failed to notify client\n");
        }
    } 
    else 
    {
        fprintf(stderr, "Unexpected command : %s\n", buffer);
    }

//====================================== try to get usr prompt every 30s ==============================================
    
    while (1) 
    {
        const char * prompt = "Server : Please Respond (type 'end' to exit)\n";
        if (send(client_fd, prompt, strlen(prompt), 0) < 0)
        {
            perror("Failed to promtp client fore response");
            break;
        }

        fd_set rfds;
        struct timeval tv;
        int sel = 0;
        FD_ZERO(&rfds);
        FD_SET(client_fd, &rfds);
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        sel = select(client_fd + 1, &rfds, NULL, NULL, &tv);            //check client_fd for data every 30s
        if (sel > 0 && FD_ISSET(client_fd, &rfds))
        {
            memset(buffer, 0, sizeof(buffer));
            bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0)
            {
                printf("Client Disconnected\n");
                break;
            }

            buffer[bytes] = '\0';
            trim_string(buffer);
            if (strcasecmp(buffer, "end") == 0)
            {
                printf("Client Requested End.\n Closing Connection\n");
                break;
            }
        }
        else if (sel == 0)
        {
            //timeout
            continue;
        }
        else 
        {
            perror("select error\n");
            break;
        }

        close(client_fd);
        close(server_fd);

        return 0;
    }
}

void trim_string(char *str)
{
    size_t len = strlen(str);

    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r'))
    {
        str[--len] = '\0';
    }
    
}