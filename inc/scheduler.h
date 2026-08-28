/*
 * Very simple run-to-complete scheduler
 */

#include <pico/stdlib.h>
#include <stdio.h>

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void (*func)(void);
    char * name;
} Task;

// Create a task
#define CREATE_TASK(func_ident) \
    ((Task) {                    \
        .func = (func_ident),    \
        .name = #func_ident,     \
    })

typedef struct {
    Task * tasks;
    size_t task_count;
} Scheduler;


void scheduler_init(Scheduler * scheduler, Task * tasks, size_t task_count) {
    *scheduler = (Scheduler) {
        .tasks = tasks,
        .task_count = task_count,
    };
}

void scheduler_poll(Scheduler * scheduler) {
    for (uint i = 0; i < scheduler->task_count; i++) {
        Task * task = &scheduler->tasks[i];
        (task->func)();
    }
}
