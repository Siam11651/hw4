#ifndef RECIPE_QUEUE_H
#define RECIPE_QUEUE_H

#include <cookbook.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct queue_node {
    RECIPE *data;
    struct queue_node *next;
    struct queue_node *parent;
    sem_t semaphore;
    int dep_count;
} QUEUE_NODE;

typedef struct queue {
    QUEUE_NODE *front;
    QUEUE_NODE *back;
    pthread_mutex_t mutex;
} QUEUE;

QUEUE_NODE *make_queue_node(RECIPE *recipe);

void push(QUEUE *queue, QUEUE_NODE *queue_node);

QUEUE_NODE *pop(QUEUE *queue);

#endif