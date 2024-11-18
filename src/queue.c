#include <cook/queue.h>

QUEUE_NODE *make_queue_node(RECIPE *recipe) {
    QUEUE_NODE *new_node = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
    new_node->data = recipe;
    new_node->next = NULL;
    new_node->parent = NULL;
    new_node->dep_count = 0;

    return new_node;
}

void push(QUEUE *queue, QUEUE_NODE *queue_node) {
    pthread_mutex_lock(&queue->mutex);

    queue_node->next = NULL;

    if (queue->back) {
        queue->back->next = queue_node;
        queue->back = queue_node;
    } else {
        queue->front = queue->back = queue_node;
    }

    pthread_mutex_unlock(&queue->mutex);
}

QUEUE_NODE *pop(QUEUE *queue) {
    pthread_mutex_lock(&queue->mutex);

    QUEUE_NODE *to_return = NULL;

    if (queue->front) {
        to_return = queue->front;
        queue->front = queue->front->next;

        if (queue->front == NULL) {
            queue->back = NULL;
        }
    }

    pthread_mutex_unlock(&queue->mutex);

    return to_return;
}