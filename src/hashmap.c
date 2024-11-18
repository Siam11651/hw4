#include <hashmap.h>

void recipe_run_hashmap_init(RECIPE_RUN_HASHMAP *recipe_run_hashmap, unsigned long long bucket_size) {
    recipe_run_hashmap->bucket_size = bucket_size;
    recipe_run_hashmap->buckets = (RECIPE_RUN_HASHMAP_NODE **)malloc(sizeof(RECIPE_RUN_HASHMAP_NODE *) * bucket_size);

    for (int i = 0; i < recipe_run_hashmap->bucket_size; ++i) {
        recipe_run_hashmap->buckets[i] = NULL;
    }
}

RECIPE_RUN_HASHMAP_NODE *recipe_run_hashmap_get(RECIPE_RUN_HASHMAP *recipe_run_hashmap, RECIPE *recipe) {
    unsigned long long index = (unsigned long long)recipe % recipe_run_hashmap->bucket_size;
    RECIPE_RUN_HASHMAP_NODE *bucket = recipe_run_hashmap->buckets[index];
    RECIPE_RUN_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return current;
        }
    }

    return NULL;
}

void recipe_run_hashmap_insert(RECIPE_RUN_HASHMAP *recipe_run_hashmap, RECIPE *recipe) {
    unsigned long long index = (unsigned long long)recipe % recipe_run_hashmap->bucket_size;
    RECIPE_RUN_HASHMAP_NODE *bucket = recipe_run_hashmap->buckets[index];
    RECIPE_RUN_HASHMAP_NODE *current = bucket;

    while (current) {
        if (current->recipe == recipe) {
            return;
        }
    }

    RECIPE_RUN_HASHMAP_NODE *new_node = (RECIPE_RUN_HASHMAP_NODE *)malloc(sizeof(RECIPE_RUN_HASHMAP_NODE));
    new_node->next = NULL;
    new_node->recipe = recipe;

    pthread_mutex_init(&new_node->recipe_mtx, NULL);

    if (bucket == NULL) {
        bucket = new_node;
    } else {
        new_node->next = bucket;
        bucket = new_node;
    }

    recipe_run_hashmap->buckets[index] = bucket;
}

void recipe_run_hashmap_free(RECIPE_RUN_HASHMAP *recipe_run_hashmap) {
    for (int i = 0; i < recipe_run_hashmap->bucket_size; ++i) {
        if (recipe_run_hashmap->buckets[i] == NULL) {
            continue;
        }

        RECIPE_RUN_HASHMAP_NODE *current = recipe_run_hashmap->buckets[i];

        while (current) {
            RECIPE_RUN_HASHMAP_NODE *next = current->next;

            pthread_mutex_destroy(&current->recipe_mtx);
            free(current);

            current = next;
        }
    }

    free(recipe_run_hashmap->buckets);
}