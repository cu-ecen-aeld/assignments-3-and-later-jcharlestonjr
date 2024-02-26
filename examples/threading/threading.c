#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<pthread.h>
#include<time.h>
#include<sys/types.h>

#define NUM_THREADS 2
// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)
struct thread_data thread_param; 
void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* threadargs = (struct thread_data *) thread_param;
    int rc = 1;
    usleep(threadargs->wait);
    while(rc){
    	rc = pthread_mutex_lock(threadargs->mutex);
	usleep(threadargs->wait);
    }
    if(rc == 0){
    	usleep(threadargs->release);
    	rc = pthread_mutex_unlock(threadargs->mutex);
    		threadargs->thread_complete_success = true;
    }
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

        struct thread_data *test = malloc(sizeof(*test));
	test->mutex = mutex;
	test->wait =wait_to_obtain_ms*1000;
	test->release =wait_to_release_ms*1000;
	test->thread_complete_success = false;
	int i = pthread_create(thread, (void *)0, threadfunc,test);
	if(i==0){
		return true;
	}
	return false;
}

