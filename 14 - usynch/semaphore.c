#include <stdlib.h>
#include "usynch.h"
#include "uthread_internal.h"

typedef struct sem_wait_block {
	LIST_ENTRY node;
	PUTHREAD thread;
	unsigned count;
} SEM_WAIT_BLOCK, *PSEM_WAIT_BLOCK;

void ut_sem_wait(PSEMAPHORE sem, unsigned count) {
	if (sem->counter >= count) {
		sem->counter -= count;
	} else {
		SEM_WAIT_BLOCK wait_block;
		wait_block.thread = ut_self();
		wait_block.count = count;
		insertTailList(&(sem->waiters), &(wait_block.node));
		
		ut_deactivate();
	}
}

void ut_sem_post(PSEMAPHORE sem, unsigned count) {
	sem->counter += count;
	
	if (sem->counter > sem->limit) {
		sem->counter = sem->limit;
	}
	
	while (!isListEmpty(&(sem->waiters))) {
		unsigned to_remove = container_of(sem->waiters.flink, SEM_WAIT_BLOCK, node)->count;
		if (sem->counter >= to_remove) {
			sem->counter -= to_remove;
			PUTHREAD a_blocked_thread =
				container_of(removeHeadList(&(sem->waiters)), SEM_WAIT_BLOCK, node)->thread;
			ut_activate(a_blocked_thread);
		}
	}
}
