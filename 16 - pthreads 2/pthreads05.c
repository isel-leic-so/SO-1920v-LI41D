#include <stdio.h>
#include <unistd.h>
#include "queue.h"

QUEUE queue;

void * insert(void * ptr) {
	sleep(10);
	int base = (long)ptr;
	for (int i = base; i < base + 10000; ++i) {
		queue_offer(&queue, i);
	}
	return NULL;
}

int main() {
	
	pthread_t threads[100];
	
	queue_init(&queue);

	for (int i = 0; i < 100; ++i) {
		pthread_create(&threads[i], NULL, insert, (void*)(long)((i + 1) * 10000));
	}

	/*
	for (int i = 0; i < 100; ++i) {
		pthread_join(threads[i], NULL);
	}
	*/
	
	for (int i = 0; i < 100*10000; ++i) {
		printf("%d\n", queue_take(&queue));
	}
	
	queue_cleanup(&queue);
	return 0;
}
