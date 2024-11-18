#include <cook/cook.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void run_steps(STEP *steps, char *input, char *output) {
    STEP *step = steps;
    STEP *prev_step = NULL;
    int prev_pipe_ends[] = {-1, -1};
    int fork_id = -1;
    int child_count = 0;

    while (step) {
        STEP *next_step = step->next;
        int pipe_ends[] = {-1, -1};

        if (next_step) {
            pipe(pipe_ends);
        }

        fork_id = fork();

        if (fork_id == 0) {
            if (prev_step == NULL && input) {
                int input_fd = open(input, O_RDONLY);

                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            } else if (prev_pipe_ends[0] != -1) {
                close(prev_pipe_ends[1]);
                dup2(prev_pipe_ends[0], STDIN_FILENO);
                close(prev_pipe_ends[0]);
            }

            if (next_step == NULL && output) {
                int output_fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            } else if (pipe_ends[1] != -1) {
                close(pipe_ends[0]);
                dup2(pipe_ends[1], STDOUT_FILENO);
                close(pipe_ends[1]);
            }

            execv("./util/generic_step", step->words);
        } else {
            ++child_count;

            close(prev_pipe_ends[0]);
            close(prev_pipe_ends[1]);

            prev_pipe_ends[0] = pipe_ends[0];
            prev_pipe_ends[1] = pipe_ends[1];
        }

        step = next_step;
    }

    while (child_count) {
        wait(NULL);

        --child_count;
    }
}

void run_tasks(QUEUE_NODE *node) {
    TASK *task = node->data->tasks;

    while (task) {
        run_steps(task->steps, task->input_file, task->output_file);

        task = task->next;
    }
}

int make_work_queue(RECIPE *recipe, QUEUE *queue, TRAVERSAL_HASHMAP *traversal_hashmap) {
    {
        TRAVERSAL_HASHMAP_NODE *node = traversal_hashmap_get(traversal_hashmap, recipe);

        if(node) {
            return 0;
        }
    }

    RECIPE_LINK *current_dependency = recipe->this_depends_on;
    QUEUE_NODE *new_node = make_queue_node(recipe);
    int size = 1;

    while (current_dependency) {
        if (current_dependency->recipe) {
            size += make_work_queue(current_dependency->recipe, queue, traversal_hashmap);
        }

        current_dependency = current_dependency->next;
    }

    push(queue, new_node);

    return size;
}