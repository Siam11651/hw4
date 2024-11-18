#ifndef HASHMAP_H
#define HASHMAP_H

#include <cookbook.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct recipe_run_hashmap_node {
    RECIPE *recipe;
    pthread_mutex_t recipe_mtx;
    struct recipe_run_hashmap_node *next;
} RECIPE_RUN_HASHMAP_NODE;

typedef struct recipe_run_hashmap {
    unsigned long long bucket_size;
    RECIPE_RUN_HASHMAP_NODE **buckets;
} RECIPE_RUN_HASHMAP;

void recipe_run_hashmap_init(RECIPE_RUN_HASHMAP *recipe_run_hashmap, unsigned long long bucket_size);
RECIPE_RUN_HASHMAP_NODE *recipe_run_hashmap_get(RECIPE_RUN_HASHMAP *recipe_run_hashmap, RECIPE *recipe);
void recipe_run_hashmap_insert(RECIPE_RUN_HASHMAP *recipe_run_hashmap, RECIPE *recipe);
void recipe_run_hashmap_free(RECIPE_RUN_HASHMAP *recipe_run_hashmap);

#endif