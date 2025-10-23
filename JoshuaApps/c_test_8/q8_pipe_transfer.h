#include <stdio.h>
#include <stdlib.h>

int pipe_init(void);
int pipe_write(const void* data, size_t size);
int pipe_read(void * buffer,  size_t size);
void pipe_deinit(void);