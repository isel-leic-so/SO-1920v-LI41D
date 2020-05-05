#include <stdlib.h>
#include "usynch.h"
#include "uthread_internal.h"

typedef struct wait_block {
	LIST_ENTRY node;
	PUTHREAD thread;
} WAIT_BLOCK, *PWAIT_BLOCK;

void ut_event_wait(PEVENT event) {
	if (!event->signaled) {
		
		WAIT_BLOCK wait_block;
		wait_block.thread = ut_self();
		insertTailList(&(event->waiters), &(wait_block.node));
		
		ut_deactivate();
	}
}

void ut_event_signal_all(PEVENT event) {
	event->signaled = true;
	while (!isListEmpty(&(event->waiters))) {
		PUTHREAD a_blocked_thread =
			container_of(removeHeadList(&(event->waiters)), WAIT_BLOCK, node)->thread;
		ut_activate(a_blocked_thread);
	}
}
