#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_memory = 0;
sem_t semaphore;

void* count_numbers(void* arg)
{
    int process_id = *(int*)arg;
    while(1)
    {   
        sem_wait(&semaphore);
        if(shared_memory >= 1000)
        {   
            sem_post(&semaphore);
            break;
        }
        if(rand() % 2 == 0)
        {
            shared_memory++;
            printf("Process %d: Wrote %d\n", process_id, shared_memory);    
        }   
        sem_post(&semaphore);
        usleep(1000);
    }
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    int process_id1 = 1, process_id2 = 2;
    sem_init(&semaphore, 0, 1);

    pthread_create(&thread1, NULL, count_numbers, &process_id1);
    pthread_create(&thread2, NULL, count_numbers, &process_id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&semaphore);

    printf("Final value: %d\n", shared_memory);

    return 0;
}

