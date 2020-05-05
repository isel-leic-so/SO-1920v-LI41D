#ifndef USYNCH_H
#define USYNCH_H

#include <stdbool.h>
#include "list.h"

// EVENT

typedef struct event {
	bool signaled;
	LIST_ENTRY waiters;
} EVENT, *PEVENT;

__attribute__((always_inline))
inline void ut_event_init(PEVENT event) {
	event->signaled = false;
	initializeListHead(&(event->waiters));
}

void ut_event_wait(PEVENT event);

void ut_event_signal_all(PEVENT event);


// SEMAPHORE

typedef struct semaphore {
	unsigned int limit;    // maximum allowed number of permits
	unsigned int counter;  // current number of permits
	LIST_ENTRY waiters;
} SEMAPHORE, *PSEMAPHORE;

__attribute__((always_inline))
inline void ut_sem_init(PSEMAPHORE sem, unsigned initial_permits, unsigned limit) {
	sem->limit = limit;
	sem->counter = initial_permits;
	initializeListHead(&(sem->waiters));
}

void ut_sem_wait(PSEMAPHORE sem, unsigned count);

void ut_sem_post(PSEMAPHORE sem, unsigned count);

#endif // USYNCH_H
