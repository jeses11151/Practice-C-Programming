#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("AF_INET not supported");
        return 1;
    }
    printf("AF_INET supported\n");
    return 0;
}