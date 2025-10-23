#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Shared resource
int shared_counter = 0;
int use_mutex = 0; // 0 = race condition mode, 1 = mutex mode

pthread_mutex_t lock;


// Helper: current time in ms
long current_millis() {
    struct timespec ts;

        //time counter
    int millisec = 0;
    // struct timeval tv;
    struct tm *ltime;
    char timestamp[64] = {0};

    //create timestamp
    snprintf(timestamp, sizeof(timestamp), "[%02d:%02d:%02d.%03d]", 
        ltime->tm_hour, 
        ltime->tm_min, 
        ltime->tm_sec, 
        millisec); 

    printf("%d", timestamp);
    clock_gettime(timestamp, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

// Writer Thread
void* writer_thread(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (use_mutex) pthread_mutex_lock(&lock);

        int temp = shared_counter;
        usleep(50000); // Simulate work (50ms delay)
        shared_counter = temp + 1;

        if (use_mutex) pthread_mutex_unlock(&lock);

        printf("[%ld ms] Writer: Incremented counter to %d\n", current_millis(), shared_counter);
        usleep(100000); // Wait before next write
    }
    return NULL;
}

// Reader Thread
void* reader_thread(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (use_mutex) pthread_mutex_lock(&lock);

        int value = shared_counter;

        if (use_mutex) pthread_mutex_unlock(&lock);

        printf("[%ld ms] Reader: Read counter value = %d\n", current_millis(), value);
        usleep(80000); // Wait before next read
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && argv[1][0] == '1') {
        use_mutex = 1; // Enable mutex mode if "1" passed as argument
    }

    printf("Starting program in %s mode\n",
           use_mutex ? "SAFE (mutex)" : "UNSAFE (race condition)");

    pthread_t tid_reader, tid_writer;
    pthread_mutex_init(&lock, NULL);

    long start = current_millis();

    pthread_create(&tid_writer, NULL, writer_thread, NULL);
    pthread_create(&tid_reader, NULL, reader_thread, NULL);

    pthread_join(tid_writer, NULL);
    pthread_join(tid_reader, NULL);

    long end = current_millis();

    printf("\nFinal counter value: %d\n", shared_counter);
    printf("Total execution time: %ld ms\n", end - start);

    pthread_mutex_destroy(&lock);
    return 0;
}
