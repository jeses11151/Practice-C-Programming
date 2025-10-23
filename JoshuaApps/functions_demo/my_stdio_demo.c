#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/file.h>
#include <time.h>

#define TEXTFILE "/home/joshua/tmp/mytextfile.txt"
#define COPYFILE "/home/joshua/tmp/mycopyfile.txt"
#define NUMBERFILE "/home/joshua/tmp/mynumfile.txt"
#define MEMSIZE 256

int myopenfile(char passchar)
{
    FILE * myfileptr = NULL;
    int actfile = 0;

    myfileptr = fopen(TEXTFILE, "r+");
    if(myfileptr == NULL){
        perror("Failed to open file:");
        return -1;
    }

    actfile = fputs("A string to put into file\n", myfileptr);
    if(actfile < 0){
        perror("Failed to puts character into .txt file\n");
        fclose(myfileptr);
        return -1;
    }

    actfile = fprintf(myfileptr, "2nd string to fprintf into file.\n");
    if(ferror(myfileptr)){
        perror("Error writing ti file using fprintf()");
        fclose(myfileptr);
        return -1;
    }

    fputc('\n', myfileptr);
    if(ferror(myfileptr)){
        perror("Error writing NULL Terminator to file");
        fclose(myfileptr);
        return -1;
    }

    fclose(myfileptr);
}


char * myreadfile(size_t * out_size)
{
    FILE * myreadptr = NULL;
    char * readbuffer;
    size_t nread = MEMSIZE;
    int actfile = 0;

    readbuffer = malloc(MEMSIZE);
    if(readbuffer == NULL){
        perror("Failed to allocate memory:");
        return NULL; 
    }

    myreadptr = fopen(TEXTFILE, "r");
    if(myreadptr == NULL){
        perror("Failed to open file:");
        free(readbuffer);
        return NULL;
    }

    nread = fread(readbuffer, 1, MEMSIZE - 1, myreadptr);
    if(ferror(myreadptr)){
        perror("Error Reading File\n");
        fclose(myreadptr);
        return NULL;
    }

    readbuffer[nread] = '\0';
    printf("File Contents: \n %s \n", readbuffer);

    fclose(myreadptr);
    * out_size = nread;
    return readbuffer;
}

int write_memblock_to_file(const char *readbuffer, size_t nread)
{
    char * new_block = NULL;
    FILE * dest = NULL;
    char buffer[MEMSIZE];


    new_block = malloc(nread);
    if(new_block == NULL){
        perror("Failed to allocalte memory block:");
        return -1;
    }
    memset(new_block, 0, nread);

    if(memcpy(new_block, readbuffer, nread) == NULL){
        perror("Failed to perform memcpy:");
        free(new_block);
        return -1;
    }

    if(memcmp(new_block, readbuffer, nread) != 0){
        perror("new_block != readbuffer");
        printf("new_block and readbuffer is not equal");
        printf("Failed to copy from readbuffer");
        free(new_block);
        return -1;
    }

    dest = fopen("/home/joshua/tmp/mycopyfile.txt", "w");
    if (dest == NULL) {
        perror("Failed to open destination file");
        free(new_block);
        return -1;
    }

    // Write memory block to file
    if (fwrite(new_block, 1, nread, dest) != nread) {
        perror("Failed to write to destination file");
        fclose(dest);
        free(new_block);
        return -1;
    }

    fclose(dest);
    printf("Enter confirmation message (wrote %zu bytes): ", nread);
    fscanf("%s", buffer); 
    printf("You entered: %s\n", buffer);    
    free(new_block);

    return 0;
}

int mymemorycopy()
{
    FILE * myfile = NULL;
    int random = 0;
    bool is_even = TRUE;    //TRUE = even, FALSE = odd

    random = srand();
    if(random %2 == 0){
        printf("is even\n");
        is_even = TRUE;
    }else{
        printf("is odd\n");
        is_even = FALSE;
    }

    if(is_even = TRUE){
        myfile = fopen(COPYFILE, "r");
    }else{
        myfile = fopen(TEXTFILE, "r")
    }
    
    if(myfile = NULL){
        perror("Failed to open file:");
        return -1;
    }
    

}

int main() 
{
    char userchar1 = '\0';
    int userint1 = 0;
    size_t nread = MEMSIZE;
    char * buf;

    printf("--- C Standard Library Function Examples ---\n\n");

    printf("Please type in a character:\n");
    scanf("%c", &userchar1);
    while(getchar()!='\n' && getchar() != EOF);     //errorchecking

    if(isdigit(userchar1)){
        userint1 = myopenfile(userchar1);
        printf("User entered integer: %d\n", userint1);
        buf = myreadfile(&nread);
        if (buf) {
            write_memblock_to_file(buf, nread);
            free(buf);
        }
    }else{
        printf("user entered character: %c\n", userchar1);
        putc(userchar1+1, stdout);
        putchar(userchar1+5);
        exit(1);
    }

    return 0; 
}