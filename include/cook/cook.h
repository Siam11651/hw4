#ifndef COOK_H
#define COOK_H

#include <cookbook.h>
#include <cook/queue.h>

typedef struct run_status {
  RECIPE *recipe;
  pthread_mutex_t start_mtx;
} RUN_STATUS;

void run_steps(STEP *steps, char *input, char *output);

void run_tasks(QUEUE_NODE *node);

int make_work_queue(RECIPE *recipe, QUEUE *queue, QUEUE_NODE *parent);

#endif