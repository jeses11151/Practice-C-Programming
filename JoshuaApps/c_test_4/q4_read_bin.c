#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>     
#include <unistd.h>    
#include <sys/stat.h>

#define INC "q4_test_bin.bin"

// char* openfile_and_save_in_buffer(FILE *fp, int filesize)
// {
//     char * buffer = malloc(filesize);

//     if(buffer == NULL){
//         perror("fail to allocate memory");
//         fclose(fp);
//         exit(1);
//     }

//     rewind(fp);
//     fread(buffer, 1, filesize, fp);

//     return buffer;
// }

int print_hex_up(FILE* fp, int filesize)
{
    char * buffer = NULL;
    int i = 0;

    // buffer = openfile_and_save_in_buffer(fp, filesize);
//need to add into buffer first

    buffer = malloc(filesize);
    if (buffer == NULL){
        perror("Failed to allcoated memory\n");
        fclose(fp);
        return 1;
    }

    rewind(fp);
    fread(buffer, 1, filesize, fp);

    printf("Hex (uppercase): \n");
    for(i = 0; i < filesize; i++){
        printf("%X", buffer[i]);
    }

// 0xef -> 0x02x  (   0x   |   %02   |  x  )
//                  prefix   2-digit   hex
// 0xEF -> 0x02X  (   0x   |   %02   |  X  )

// EF
// ef

    printf("\n");
    free(buffer);
    buffer = NULL;
    return 0;
}

int print_hex_low(FILE* fp, int filesize)
{
    // char * buffer = openfile_and_save_in_buffer(fp, filesize);
    char * buffer = NULL;
    int i = 0;

    buffer = malloc(filesize);
    if (buffer == NULL){
        perror("Failed to allocate mempry\n");
        fclose(fp);
        return 1;
    }

    rewind(fp);
    fread(buffer, 1, filesize, fp);

    printf("Hex (lowercase): \n");
    for(i = 0; i < filesize; i++){
        printf("%x", buffer[i]);
    }

    printf("\n");
    free(buffer);
    return 0;
}

int main()
{
    int filesize = 0;
    FILE *fp = fopen(INC, "rb");
    if(!fp){
        perror("file not found");
        exit(1);
    }else{
        printf("Successfully opened file: %s\n", INC);
    }

    fseek(fp, 0L, SEEK_END);            
    filesize = ftell(fp);               
    //rewind(fp);

    if(filesize <= 0){
        perror("file empty");
        fclose(fp);
        exit(1);
    }

    print_hex_up(fp, filesize);
    print_hex_low(fp, filesize);


    fclose(fp);
}
