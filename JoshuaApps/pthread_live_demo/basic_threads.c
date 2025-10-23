#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main()
{
    int check = -1;     //init check as fail
    ptherad_t thread_id[2];
    int *arg;

          //pthread_create ( thread ID, Attributes, your_function, Arguments )
    check = pthread_create ( thread_id,     NULL,      print_num,    &arg    );
    if (check != 0)
    {
        perror("Failed to create new thread\n");
        return -1;
    }
    else 
    {
        printf("Successfully created thread\n" );
    }
}

void* print_num(void* p)
{
    printf("PRINT_NUM: 12345\n");

    pthread_exit(&p);
}