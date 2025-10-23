#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int buffer = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//simple function
void * count_to_ten(void* arg) 
{
    int counter = 0;
    int i = 0;

    for(i = 0; i <= 10; i ++){
        printf("Thread 1 Counter: %d \n", counter);
        ++counter;

        pthread_mutex_lock(&lock);
        while (counter == 5 && buffer != 9){                //lock to check buffer
            pthread_cond_wait(&cond, &lock);
            printf("Check A unluck wait\n");
            // ++counter;
        }
        pthread_mutex_unlock(&lock);
        
        printf("unlocked thread 1\n");
        sleep(1);
    }
}

void * count_to_twenty(void* arg)
{
    int counter = 0;
    int i = 0;

    for(i = 0; i <= 20; i ++){
        printf("Thread 2 Counter: %d \n", counter);
        ++counter;

        pthread_mutex_lock(&lock);              //lock to check buffer
        while (counter == 5 && buffer != 9){
            pthread_cond_wait(&cond, &lock);
            printf("Check B unluck wait\n");
            // ++counter;
        }
        pthread_mutex_unlock(&lock);

        printf("unlock thread 2\n");
        sleep(1);
    }
}

void * trigger_finger(void* arg)
{
    int input = 0;

    pthread_mutex_trylock(&lock);
    // pthread_mutex_lock(&lock);
    while (buffer != 9){
        printf("This is Thread 3:\n");
        printf("Please write 9 to continue counting\n");
        scanf("%d", &input);
        buffer = input;
    }

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    // pthread_cond_signal(&cond);
    
    sleep(1);

}

void ipc_trailer()
{
    int ipc_trailer = 0;
    int write_fd = 0;
    char my_message[] = "Hello Multithreading";

    ipc_trailer = mkfifo("/home/joshua/tmp/myfifo", 0666);  //create fifo and rteturn fd to "ipc_trailer"
    if (ipc_trailer != 0){
        perror("mkfifo failed");
        return;
    } else {
        printf("FIFO created at /home/joshua/tmp/fifospec\n");
    }

    write_fd = write(ipc_trailer, my_message, sizeof(my_message));     
    if (write_fd == -1){
        perror("Failed to write to FIFO");
    }
    //write(ipc_trailer, "/0", 1);

    return;
}

void thread_join_finish()
{
    int ipc_trailer = 0;
    int read_fd = 0;
    char readpath[] =  "/home/joshua/tmp/myfifo";
    char buffer[] = {0};
    int i = 0;

    printf("reading fifo after join thread\n");

    ipc_trailer = read(read_fd, buffer, sizeof(buffer)); 
    for (i = 0; i < sizeof(buffer) - 1; i++)
        printf("%c", buffer[i]);

    return;
}


int main() 
{
    pthread_t mythread_1, mythread_2, mythread_3;

    pthread_create(&mythread_1, NULL, count_to_ten, NULL);
    pthread_create(&mythread_2, NULL, count_to_twenty, NULL);
    pthread_create(&mythread_3, NULL, trigger_finger, NULL);

    //function outside of threads
    ipc_trailer();
    

    pthread_join(mythread_1, NULL);
    pthread_join(mythread_2, NULL);
    pthread_join(mythread_3, NULL);

    //function must wait thread to finish
    thread_join_finish();

    return 0;
}