#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* data = (struct thread_data *) thread_param;

    // Convert wait times to microseconds and sleep
    usleep(data->wait_to_obtain_ms * 1000);

    // Try to lock the mutex
    if (pthread_mutex_lock(data->mutex) != 0) {
        ERROR_LOG("Failed to obtain mutex\n");
        data->thread_complete_success = false;
        return data;
    }

    // Hold the mutex for wait_to_release_ms
    usleep(data->wait_to_release_ms * 1000);

    // Release the mutex
    if (pthread_mutex_unlock(data->mutex) != 0) {
        ERROR_LOG("Failed to release mutex\n");
        data->thread_complete_success = false;
        return data;
    }

    data->thread_complete_success = true;
    return data;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    // Allocate memory for thread_data
    struct thread_data* data = malloc(sizeof(struct thread_data));
    if (data == NULL) {
        ERROR_LOG("Failed to allocate memory for thread_data\n");
        return false;
    }

    // Initialize the thread data
    data->thread_complete_success = false;
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;

    // Create the thread
    int rc = pthread_create(thread, NULL, threadfunc, (void *)data);
    if (rc != 0) {
        ERROR_LOG("Failed to create thread: %d\n", rc);
        free(data);
        return false;
    }

    return true;
}
