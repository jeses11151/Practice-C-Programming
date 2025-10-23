#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "q8_socket_transfer.h"

#ifndef SOCKET_MODE_EN

#define SOCKET_MODE_EN

#define SOCKET_PAIR 1
#define SOCKET_TCP 2

#endif

/*------------------------Function Declaration-----------------------*/
int socketpair_init();
int socketnet_init();
/*-------------------------------------------------------------------*/

static int sock_fds[2] = {0};
static int tcp_sock_fds = -1;
static int tcp_client_fds = -1;
static int socket_mode_flag = 0; 

int socket_init(int socket_mode)
{
    int recv_size = 65536;                  //64kb
    int sv[2];

    socket_mode_flag = socket_mode;
    if (socket_mode_flag == SOCKET_PAIR)
    {
        socketpair_init();
    }
    else if (socket_mode_flag == SOCKET_TCP)
    {
        socket_net_init();
    }
    
    return 0;
}

int socketpair_init()
{
    int recv_size = 65536;                  //64kb
    int sv[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_fds) == -1)
    {
        perror("Socketpair Failed\n");
        return -1;
    }
    else
    {
        printf("[%s:%s]Successfullt Init SOCKER PAIR\n", __FILE__, __func__);
    }

    return 0;
}

int socket_net_init()
{
    struct sockaddr_in addr;
    int opt = 1;

    tcp_sock_fds = socket(AF_INET, SOCK_STREAM, 0);                                     //AF_INET = ipv4      sock_stream = tcp
    if (tcp_sock_fds < 0)
    {
        perror("Socket TCP Failed\n");
        return -1;
    }
    else
    {
        setsockopt(tcp_sock_fds, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8080);

        if (bind(tcp_sock_fds, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            perror("TCP bind failed");
            return -1;
        }

        if (listen(tcp_sock_fds, 5) < 0)
        {
            perror("TCP listen failed");
            return -1;
        }

        printf("[%s:%s] Listening on TCP port 8080...\n", __FILE__, __func__);

        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        tcp_client_fds = accept(tcp_sock_fds, (struct sockaddr*)&client, &len);
        if (tcp_client_fds < 0)
        {
            perror("TCP accept failed");
            return -1;
        }

        printf("[%s:%s] Accepted TCP connection from %s:%d\n", __FILE__, __func__,
               inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
    }

    return 0;

}

// int socket_write(fd, const void* data, size_t size)

int socket_write(const void* data, size_t size)
{
    int ret = 1;

    //int fd = (socket_mode_flag == SOCKET_PAIR) ? sock_fds[1] : tcp_client_fds;
    int fd = 0;
    if (socket_mode_flag == SOCKET_PAIR)
    {
        fd = sock_fds[0];
    }
    else
    {
        fd = tcp_client_fds;
    }

    printf("[%s:%s] will write data (%zu bytes): \n", __FILE__, __func__, size); //%zu = length of byte (C99)
    printf("%.*s\n", (int)size, (const char*)data);

    ret = write(fd, data, size);
    if(ret <= 0){
        perror("SOCKET write failed");
    }
    
    return ret;
}

int socket_read(void* buffer, size_t size)
{
    //int fd = (socket_mode_flag == SOCKET_PAIR) ? sock_fds[0] : tcp_client_fds;
    int fd = 0;
    int bytes = 0;

    printf("[%s:%s] will read from Socket (up to %zu bytes)\n", __FILE__, __func__, size);

    if (socket_mode_flag == SOCKET_PAIR)
    {
        fd = sock_fds[0];
    }
    else
    {
        fd = tcp_client_fds;
    }

    bytes = read(fd, buffer, size);
    if (bytes > 0)
    {
        ((char*)buffer)[bytes] = '\0';
        printf("[%s:%s] read data: %s\n", __FILE__, __func__, (char*)buffer);
    }
    else if (bytes == 0)
    {
        printf("[%s:%s] socket read: EOF\n", __FILE__, __func__);
    }
    else 
    {
        perror("socket read failed");
    }
    //return read(sock_fds[0], buffer, size);
    return bytes;
}

void socket_deinit()
{
    printf("[%s:%s] will close socket now~~\n", __FILE__, __func__);

    if (socket_mode_flag == SOCKET_PAIR)
    {
        close(sock_fds[0]);
        close(sock_fds[1]);
    }
    else if (socket_mode_flag == SOCKET_TCP)
    {
        close(tcp_client_fds);
        close(tcp_sock_fds);
    }
}


//socket test
// int main(){
//     char buffer[128] = {0};
//     socket_init();

//     socket_write("Hello Socket", 13);
//     socket_read(buffer, sizeof(buffer));
//     printf("Got: %s\n", buffer);

//     socket_deinit();
//     return 0;
// }