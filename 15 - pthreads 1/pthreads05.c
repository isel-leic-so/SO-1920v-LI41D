#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "list.h"

typedef struct queue {
	pthread_mutex_t lock;
	pthread_cond_t waiters;
	LIST_ENTRY head;
	unsigned   size;
} QUEUE, *PQUEUE;

typedef struct queue_node {
	int value;
	LIST_ENTRY entry;
} QUEUE_NODE, *PQUEUE_NODE;

void queue_init(PQUEUE queue) {
	pthread_mutex_init(&(queue->lock), NULL);
	pthread_cond_init(&(queue->waiters), NULL);
	initializeListHead(&(queue->head));
	queue->size = 0;
}

void queue_cleanup(PQUEUE queue) {
	pthread_cond_destroy(&(queue->waiters));
	pthread_mutex_destroy(&(queue->lock));
}

void queue_offer(PQUEUE queue, int val) {
	PQUEUE_NODE node = (PQUEUE_NODE)malloc(sizeof (QUEUE_NODE));
	node->value = val;
	pthread_mutex_lock(&(queue->lock));
	{
		insertTailList(&(queue->head), &(node->entry));
		queue->size += 1;
		pthread_cond_signal(&(queue->waiters));
	}
	pthread_mutex_unlock(&(queue->lock));
}

int queue_take(PQUEUE queue) {
	PLIST_ENTRY entry = NULL;

	pthread_mutex_lock(&(queue->lock));
	{
		while (isListEmpty(&(queue->head))) {
			pthread_cond_wait(&(queue->waiters), &(queue->lock));
		}
		queue->size -= 1;
		entry = removeHeadList(&(queue->head));
	}
	pthread_mutex_unlock(&(queue->lock));
	
	if (entry != NULL) {
		PQUEUE_NODE node = container_of(entry, QUEUE_NODE, entry);
		int val = node->value;
		free(node);
		return val;
	}
	return -1;
}

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
