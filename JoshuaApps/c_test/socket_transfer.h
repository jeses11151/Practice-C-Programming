#include <stdio.h>
#include <stdlib.h>

int socket_init(int socket_mode);
int socket_read(void* buffer, size_t size);
int socket_write(const void* data, size_t size);
void socket_deinit(void);
