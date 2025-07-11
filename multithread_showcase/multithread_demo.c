// Multithreaded Task Queue Example in C
// Uses pthreads, mutexes, and condition variables to synchronize threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TASK_QUEUE_SIZE 5
#define NUM_WORKERS 3
#define NUM_TASKS 10

// Task definition
typedef struct {
    int task_id;
} Task;

// Task queue structure
typedef struct {
    Task tasks[TASK_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} TaskQueue;

TaskQueue queue;

void enqueue(Task task) {
    pthread_mutex_lock(&queue.lock);
    while (queue.count == TASK_QUEUE_SIZE) {
        pthread_cond_wait(&queue.not_full, &queue.lock);
    }
    queue.tasks[queue.rear] = task;
    queue.rear = (queue.rear + 1) % TASK_QUEUE_SIZE;
    queue.count++;
    pthread_cond_signal(&queue.not_empty);
    pthread_mutex_unlock(&queue.lock);
}

Task dequeue() {
    pthread_mutex_lock(&queue.lock);
    while (queue.count == 0) {
        pthread_cond_wait(&queue.not_empty, &queue.lock);
    }
    Task task = queue.tasks[queue.front];
    queue.front = (queue.front + 1) % TASK_QUEUE_SIZE;
    queue.count--;
    pthread_cond_signal(&queue.not_full);
    pthread_mutex_unlock(&queue.lock);
    return task;
}

void* worker_thread(void* arg) {
    int id = *((int*)arg);
    while (1) {
        Task task = dequeue();
        printf("Worker %d processing task %d\n", id, task.task_id);
        sleep(1); // Simulate task processing
    }
    return NULL;
}

int main() {
    // Initialize queue
    queue.front = queue.rear = queue.count = 0;
    pthread_mutex_init(&queue.lock, NULL);
    pthread_cond_init(&queue.not_empty, NULL);
    pthread_cond_init(&queue.not_full, NULL);

    // Create worker threads
    pthread_t workers[NUM_WORKERS];
    int worker_ids[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        worker_ids[i] = i + 1;
        pthread_create(&workers[i], NULL, worker_thread, &worker_ids[i]);
    }

    // Enqueue tasks
    for (int i = 0; i < NUM_TASKS; i++) {
        Task task = { .task_id = i + 1 };
        enqueue(task);
        printf("Enqueued task %d\n", task.task_id);
        sleep(0.5); // Simulate task arrival
    }

    // Join threads (this example runs infinitely, so this is not reached)
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(workers[i], NULL);
    }

    return 0;
}
