#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define NTHREADS 800
#define INDIVIDUAL_COUNT 100000

int global_counter = 0;

void * incrementer(void * ptr) {
	for (int i = 0; i < INDIVIDUAL_COUNT; ++i) {
		//++global_counter;
		__sync_fetch_and_add(&global_counter, 1);
	}
	return NULL;
}

int main() {
	
	pthread_t threads[NTHREADS];
	
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_create(&threads[i], NULL, incrementer, NULL);
	}
	
	printf(":: working... ::\n");
	
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_join(threads[i], NULL);
	}
	
	printf("Expected result: %d\n", NTHREADS * INDIVIDUAL_COUNT);
	printf("Actual   result: %d\n", global_counter);
	
	return 0;
}