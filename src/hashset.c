#include <cook/hashset.h>
#include <cook/hash.h>
#include <stdlib.h>

void hashset_init(HASHSET *hashset, unsigned long long bucket_size) {
    hashset->bucket_size = bucket_size;
    hashset->buckets = (HASHSET_NODE **)malloc(sizeof(HASHSET_NODE *) * bucket_size);

    for (int i = 0; i < hashset->bucket_size; ++i) {
        hashset->buckets[i] = NULL;
    }
}

HASHSET_NODE *hashset_get(HASHSET *hashset, RECIPE *recipe) {
    unsigned long long index = hash(recipe->name) % hashset->bucket_size;
    HASHSET_NODE *bucket = hashset->buckets[index];
    HASHSET_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return current;
        }
    }

    return NULL;
}

int hashset_insert(HASHSET *hashset, RECIPE *recipe) {
    unsigned long long index = hash(recipe->name) % hashset->bucket_size;
    HASHSET_NODE *bucket = hashset->buckets[index];
    HASHSET_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return 1;
        }
    }

    HASHSET_NODE *new_node = (HASHSET_NODE *)malloc(sizeof(HASHSET_NODE));
    new_node->next = NULL;
    new_node->recipe = recipe;

    if (bucket == NULL) {
        bucket = new_node;
    } else {
        new_node->next = bucket;
        bucket = new_node;
    }

    hashset->buckets[index] = bucket;

    return 0;
}

void hashset_free(HASHSET *hashset) {
    for (int i = 0; i < hashset->bucket_size; ++i) {
        if (hashset->buckets[i] == NULL) {
            continue;
        }

        HASHSET_NODE *current = hashset->buckets[i];

        while (current) {
            HASHSET_NODE *next = current->next;

            free(current);

            current = next;
        }
    }

    free(hashset->buckets);
}