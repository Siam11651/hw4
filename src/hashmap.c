#include <cook/hashmap.h>

void traversal_hashmap_init(TRAVERSAL_HASHMAP *traversal_hashmap, unsigned long long bucket_size) {
    traversal_hashmap->bucket_size = bucket_size;
    traversal_hashmap->buckets = (TRAVERSAL_HASHMAP_NODE **)malloc(sizeof(TRAVERSAL_HASHMAP_NODE *) * bucket_size);

    for (int i = 0; i < traversal_hashmap->bucket_size; ++i) {
        traversal_hashmap->buckets[i] = NULL;
    }
}

TRAVERSAL_HASHMAP_NODE *traversal_hashmap_get(TRAVERSAL_HASHMAP *traversal_hashmap, RECIPE *recipe) {
    unsigned long long index = (unsigned long long)recipe % traversal_hashmap->bucket_size;
    TRAVERSAL_HASHMAP_NODE *bucket = traversal_hashmap->buckets[index];
    TRAVERSAL_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return current;
        }
    }

    return NULL;
}

void traversal_hashmap_insert(TRAVERSAL_HASHMAP *traversal_hashmap, RECIPE *recipe) {
    unsigned long long index = (unsigned long long)recipe % traversal_hashmap->bucket_size;
    TRAVERSAL_HASHMAP_NODE *bucket = traversal_hashmap->buckets[index];
    TRAVERSAL_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return;
        }
    }

    TRAVERSAL_HASHMAP_NODE *new_node = (TRAVERSAL_HASHMAP_NODE *)malloc(sizeof(TRAVERSAL_HASHMAP_NODE));
    new_node->next = NULL;
    new_node->traversed = 0;
    new_node->recipe = recipe;

    if (bucket == NULL) {
        bucket = new_node;
    } else {
        new_node->next = bucket;
        bucket = new_node;
    }

    traversal_hashmap->buckets[index] = bucket;
}

void traversal_hashmap_free(TRAVERSAL_HASHMAP *traversal_hashmap) {
    for (int i = 0; i < traversal_hashmap->bucket_size; ++i) {
        if (traversal_hashmap->buckets[i] == NULL) {
            continue;
        }

        TRAVERSAL_HASHMAP_NODE *current = traversal_hashmap->buckets[i];

        while (current) {
            TRAVERSAL_HASHMAP_NODE *next = current->next;

            free(current);

            current = next;
        }
    }

    free(traversal_hashmap->buckets);
}