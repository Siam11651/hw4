#include <cook/cook.h>
#include <cook/queue.h>
#include <cook/hashmap.h>
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

DEP_HASHMAP dep_hashmap;
sem_t chef_semaphore;

typedef struct chef_routine_params {
    QUEUE_NODE *queue_node;
    int lock_status;
} CHEF_ROUTINE_PARAMS;

void *chef_routine(void *node_ptr) {
    run_tasks((QUEUE_NODE *)node_ptr, &dep_hashmap);
    sem_post(&chef_semaphore);
    free((QUEUE_NODE *)node_ptr);

    return NULL;
}

void malformed_args() {
    fprintf(stderr, "Malformed arguments\n");
    exit(1);
}

int main(int argc, char *argv[], char **envp) {
    char *main_recipe_name = NULL;
    int num_chefs = sysconf(_SC_NPROCESSORS_CONF) - 1;
    char *cookbook = "cookbook.ckb";

    if (num_chefs <= 0) {
        num_chefs = 1;
    }

    int parsed_c = 0;
    int parsed_f = 0;
    int state = 0;

    for (int i = 1; i < argc; ++i) {
        if (state == 0) {
            if (strncmp("-c", argv[i], 2) == 0) {
                if (parsed_c) {
                    malformed_args();
                } else {
                    state = 1;
                    parsed_c = 1;
                }
            } else if (strncmp("-f", argv[i], 2) == 0) {
                if(parsed_f) {
                    malformed_args();
                } else {
                    state = 2;
                    parsed_f = 1;
                }
            } else {
                if (main_recipe_name == NULL) {
                    main_recipe_name = argv[i];
                } else {
                    malformed_args();
                }
            }
        } else if (state == 1) {
            num_chefs = atoi(argv[i]);
            state = 0;
        } else if(state == 2) {
            cookbook = argv[i];
            state = 0;
        }
    }

    if (main_recipe_name == NULL) {
        fprintf(stderr, "No main recipe specified\n");
        exit(1);
    }

    COOKBOOK *cbp;
    int err = 0;
    FILE *in;

    sem_init(&chef_semaphore, 1, num_chefs);

    if ((in = fopen(cookbook, "r")) == NULL) {
        fprintf(stderr, "Can't open cookbook '%s': %s\n", cookbook, strerror(errno));
        exit(1);
    }

    cbp = parse_cookbook(in, &err);

    if (err) {
        fprintf(stderr, "Error parsing cookbook '%s'\n", cookbook);
        exit(1);
    }

    fclose(in);

    RECIPE *main_recipe = NULL;

    {
        RECIPE *current = cbp->recipes;

        while (current) {
            if (current->name && strcmp(main_recipe_name, current->name) == 0) {
                main_recipe = current;

                break;
            }

            current = current->next;
        }
    }

    if (main_recipe == NULL) {
        fprintf(stderr, "Could not find recipe '%s' in cookbook '%s'\n", main_recipe_name, cookbook);
        exit(1);
    }

    QUEUE work_queue = {NULL, NULL};

    dep_hashmap_init(&dep_hashmap, 1024);
    pthread_mutex_init(&work_queue.mutex, NULL);
    int work_count = make_work_queue(main_recipe, &work_queue, &dep_hashmap);
    pthread_t *work_threads = (pthread_t *)malloc(sizeof(pthread_t) * work_count);

    for(int i = 0; i < work_count; ++i) {
        work_threads[i] = 0;
    }

    int work_index = 0;

    while (1) {
        sem_wait(&chef_semaphore);

        QUEUE_NODE *front = pop(&work_queue);

        if (front == NULL) {
            break;
        }

        DEP_HASHMAP_NODE *node = dep_hashmap_get(&dep_hashmap, front->data);

        for(int i = 0; i < node->count; ++i) {
            sem_wait(&node->sem);
        }

        pthread_create(&work_threads[work_index], NULL, chef_routine, (void *)front);

        ++work_index;
    }

    for (int i = 0; i < work_count; ++i) {
        if(work_threads[i]) {
            pthread_join(work_threads[i], NULL);
        }
    }

    free(work_threads);

    exit(0);
}
