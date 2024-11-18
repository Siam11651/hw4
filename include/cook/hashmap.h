#ifndef HASHMAP_H
#define HASHMAP_H

#include <cookbook.h>
#include <stdlib.h>

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

#endif