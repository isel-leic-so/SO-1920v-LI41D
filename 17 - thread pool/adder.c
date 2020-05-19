#include <stdio.h>

#include <limits.h>
#include <assert.h>
#include "chrono.h"

#include "tpool.h"
#include "countdownlatch.h"

#define NITERS 50
#define NVALS 5000000
#define NTHREADS 4

tpool_t tpool;

int vals[NVALS];

static void init_vals() {
	for(int i = 0; i < NVALS; ++i)
		vals[i] = 1;
}

long seq_adder(int vals[], size_t nvals) {
	long total = 0;
	for (size_t i = 0; i < nvals; ++i)
		total += vals[i];
	return total;
}

/*
typedef struct sum_task {
	cdlatch_t * latch;
	int * vals;
	size_t nvals;
	size_t first;
	long result;
} sum_task_t;

void sum_task_func(void * arg) {
	sum_task_t * stask = (sum_task_t *)arg;
	long total = 0;
	for (size_t i = stask->first; i < stask->nvals; i += NTHREADS) {
		total += stask->vals[i];
	}
	stask->result = total;
	cdlatch_count_down(stask->latch);
}

long par_adder(int vals[], size_t nvals) {
	cdlatch_t latch;
	cdlatch_init(&latch, NTHREADS);

	sum_task_t stasks[NTHREADS];
	for (int i = 0; i < NTHREADS; ++i) {
		stasks[i].latch = &latch;
		stasks[i].vals  = vals;
		stasks[i].nvals = nvals;
		stasks[i].first = i;
		
		tp_submit(&tpool, sum_task_func, &stasks[i]);
	}

	cdlatch_await(&latch);

	long total = 0;
	for (int i = 0; i < NTHREADS; ++i) {
		total += stasks[i].result;
	}
	
	return total;
}
*/

typedef struct sum_task {
	cdlatch_t * latch;
	int * begin;
	int * end;
	long result;
} sum_task_t;

void sum_task_func(void * arg) {
	sum_task_t * stask = (sum_task_t *)arg;
	long total = 0;
	for (int * p = stask->begin; p < stask->end; ++p) {
		total += *p;
	}
	stask->result = total;
	cdlatch_count_down(stask->latch);
}

long par_adder(int vals[], size_t nvals) {
	cdlatch_t latch;
	cdlatch_init(&latch, NTHREADS);

	sum_task_t stasks[NTHREADS];
	size_t region_size = (nvals + (NTHREADS - 1)) / NTHREADS;
	
	for (int i = 0; i < NTHREADS; ++i) {
		int * begin = &vals[i * region_size];
		int * end = (begin + region_size) > &vals[nvals] ?
			&vals[nvals] :
			(begin + region_size);
		
		stasks[i].latch = &latch;
		stasks[i].begin = begin;
		stasks[i].end = end;
		
		tp_submit(&tpool, sum_task_func, &stasks[i]);
	}

	cdlatch_await(&latch);

	long total = 0;
	for (int i = 0; i < NTHREADS; ++i) {
		total += stasks[i].result;
	}
	
	return total;
}

void test(char *name,
		long (*sum_func)(int vals[], size_t nvals), 
		int vals[], 
		int nvals, 
		long expected) {
	
	unsigned long micros, less_micros = ULONG_MAX;
	long res;
	for(int i = 0; i < NITERS; ++i) {
		chrono_t chrono = chrono_start();
		{
			res = sum_func(vals, nvals);
		}
		micros = chrono_micros(chrono);

		if (micros < less_micros) 
			less_micros = micros;

		assert(expected == res);
	}
	
	printf("%s gives a result of %ld in %lu micros.\n", 
			name, res, less_micros);
}

int main() {
	tp_init(&tpool, NTHREADS);
	
	init_vals();
	 
	test("sequential adder", seq_adder, vals, NVALS, NVALS);
	test("parallel adder", par_adder, vals, NVALS, NVALS);
	
	//tp_destroy(&tpool);
	return 0;
}