#include <cook/hashmap.h>
#include <cook/hash.h>

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

void dep_hashmap_init(DEP_HASHMAP *dep_hashmap, unsigned long long bucket_size) {
    dep_hashmap->bucket_size = bucket_size;
    dep_hashmap->buckets = (DEP_HASHMAP_NODE **)malloc(sizeof(DEP_HASHMAP_NODE *) * bucket_size);

    for (int i = 0; i < dep_hashmap->bucket_size; ++i) {
        dep_hashmap->buckets[i] = NULL;
    }
}

DEP_HASHMAP_NODE *dep_hashmap_get(DEP_HASHMAP *dep_hashmap, RECIPE *recipe) {
    unsigned long long index = hash(recipe->name) % dep_hashmap->bucket_size;
    DEP_HASHMAP_NODE *bucket = dep_hashmap->buckets[index];
    DEP_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return current;
        }
    }

    return NULL;
}

void dep_hashmap_insert(DEP_HASHMAP *dep_hashmap, RECIPE *recipe) {
    unsigned long long index = hash(recipe->name) % dep_hashmap->bucket_size;
    DEP_HASHMAP_NODE *bucket = dep_hashmap->buckets[index];
    DEP_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return;
        }
    }

    DEP_HASHMAP_NODE *new_node = (DEP_HASHMAP_NODE *)malloc(sizeof(DEP_HASHMAP_NODE));
    new_node->next = NULL;
    new_node->count = 0;
    new_node->recipe = recipe;

    hashset_init(&new_node->deps, 1024);
    sem_init(&new_node->sem, 1, 0);

    if (bucket == NULL) {
        bucket = new_node;
    } else {
        new_node->next = bucket;
        bucket = new_node;
    }

    dep_hashmap->buckets[index] = bucket;
}

void dep_hashmap_free(DEP_HASHMAP *dep_hashmap) {
    for (int i = 0; i < dep_hashmap->bucket_size; ++i) {
        if (dep_hashmap->buckets[i] == NULL) {
            continue;
        }

        DEP_HASHMAP_NODE *current = dep_hashmap->buckets[i];

        while (current) {
            DEP_HASHMAP_NODE *next = current->next;

            hashset_free(&current->deps);
            free(current);

            current = next;
        }
    }

    free(dep_hashmap->buckets);
}