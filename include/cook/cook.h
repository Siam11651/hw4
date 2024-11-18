#ifndef COOK_H
#define COOK_H

#include <cookbook.h>
#include <cook/queue.h>
#include <cook/hashmap.h>

void run_steps(STEP *steps, char *input, char *output);
void run_tasks(QUEUE_NODE *node);
int make_work_queue(RECIPE *recipe, QUEUE *queue, TRAVERSAL_HASHMAP *traversal_hashmap);

#endif