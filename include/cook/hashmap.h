#ifndef HASHMAP_H
#define HASHMAP_H

#include <cookbook.h>
#include <cook/hashset.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct traversal_hashmap_node {
    RECIPE *recipe;
    int traversed;
    struct traversal_hashmap_node *next;
} TRAVERSAL_HASHMAP_NODE;

typedef struct traversal_hashmap {
    unsigned long long bucket_size;
    TRAVERSAL_HASHMAP_NODE **buckets;
} TRAVERSAL_HASHMAP;

void traversal_hashmap_init(TRAVERSAL_HASHMAP *traversal_hashmap, unsigned long long bucket_size);
TRAVERSAL_HASHMAP_NODE *traversal_hashmap_get(TRAVERSAL_HASHMAP *traversal_hashmap, RECIPE *recipe);
void traversal_hashmap_insert(TRAVERSAL_HASHMAP *traversal_hashmap, RECIPE *recipe);
void traversal_hashmap_free(TRAVERSAL_HASHMAP *traversal_hashmap);

typedef struct dep_hashmap_node {
    RECIPE *recipe;
    int count;
    sem_t sem;
    HASHSET deps;
    struct dep_hashmap_node *next;
} DEP_HASHMAP_NODE;

typedef struct dep_hashmap {
    unsigned long long bucket_size;
    DEP_HASHMAP_NODE **buckets;
} DEP_HASHMAP;

void dep_hashmap_init(DEP_HASHMAP *dep_hashmap, unsigned long long bucket_size);
DEP_HASHMAP_NODE *dep_hashmap_get(DEP_HASHMAP *dep_hashmap, RECIPE *recipe);
void dep_hashmap_insert(DEP_HASHMAP *dep_hashmap, RECIPE *recipe);
void dep_hashmap_free(DEP_HASHMAP *dep_hashmap);

#endif