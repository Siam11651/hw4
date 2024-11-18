#ifndef HASHSET_H
#define HASHSET_H

#include <cookbook.h>

typedef struct hashset_node {
    RECIPE *recipe;
    struct hashset_node *next;
} HASHSET_NODE;

typedef struct hashset {
    unsigned long long bucket_size;
    HASHSET_NODE **buckets;
} HASHSET;

void hashset_init(HASHSET *hashset, unsigned long long bucket_size);
HASHSET_NODE *hashset_get(HASHSET *hashset, RECIPE *recipe);
int hashset_insert(HASHSET *hashset, RECIPE *recipe);
void hashset_free(HASHSET *hashset);

#endif