#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() 
{
    int myfifo = 0;

    //create fifo at /temp/myfifo with r
    myfifo = mkfifo("/home/joshua/tmp/myfifo", 0666);  //create
    if (myfifo != 0){
        perror("mkfifo failed");
    } else {
        printf("FIFO created at /home/joshua/tmp/fifospec\n");
    }
}
