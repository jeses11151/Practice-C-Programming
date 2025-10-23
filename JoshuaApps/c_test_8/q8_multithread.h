#include <stdio.h>
#include <stdlib.h>

typedef enum{
    METHOD_NONE   =     0,
    METHOD_PIPE   =     1,
    METHOD_SOCKET =     2,
}transfer_method_t;

typedef struct{
    const char * read_path;
    const char * write_path;
}file_path_t;