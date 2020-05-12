#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define NTHREADS 800
#define INDIVIDUAL_COUNT 100000

pthread_mutex_t lock;
int global_counter = 0;

void * incrementer(void * ptr) {
	for (int i = 0; i < INDIVIDUAL_COUNT; ++i) {
		pthread_mutex_lock(&lock);
		++global_counter;
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main() {
	
	pthread_mutex_init(&lock, NULL);
	
	pthread_t threads[NTHREADS];
	
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_create(&threads[i], NULL, incrementer, NULL);
	}
	
	printf(":: working... ::\n");
	
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	//pthread_mutex_lock(&lock);
	int count = global_counter;
	//pthread_mutex_unlock(&lock);

	printf("Expected result: %d\n", NTHREADS * INDIVIDUAL_COUNT);
	printf("Actual   result: %d\n", count);
	
	pthread_mutex_destroy(&lock);
	return 0;
}