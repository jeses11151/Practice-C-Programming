#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>     
#include <unistd.h>    
#include <sys/stat.h>

#define DIR_PATH "/data/test"
#define INC "q3_test_file.txt"

long method_1(char *filename)
{                        //fseek
    int size = 0;
    FILE *fp = fopen(filename, "r");\
    if (fp == NULL) {
        printf("File not found");
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);            //fp set to eof
    size = ftell(fp);               //

    fclose(fp);

    printf("Size of the file is %d bytes \n", size);
    exit(1);
}

long method_2(char *filename)
{              //POSIX(linux)
    struct stat st = {0};
    int size = 0;

    stat(filename, &st);                    
    size = st.st_size;

    printf("size of file is %d bytes\n", size);

    return 0;
}

long method_3(char *filename)
{
    struct stat buf;
    int size = 0;
    int fd = 0;
    
    fd = open(filename, O_RDONLY);
    fstat(fd, &buf);
    size = buf.st_size;

    printf("size of file is %d bytes\n", size);

    return 0;
}   

void main()
{
    int method;
    char filename[] = INC;

    printf("Check File Size: \n");
    printf("Method 1: \n");
    printf("Method 2: \n");
    printf("Method 3: \n");

    scanf("%d", &method);

    switch (method)
    {
    case 1 :
        method_1(filename);
        break;
    case 2:
        method_2(filename);
        break;
    case 3:
        method_3(filename);
        break;
    default:
        printf("Invalid Selection. Exit Program\n");
        break;
    } 
}