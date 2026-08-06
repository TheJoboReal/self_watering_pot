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
    uint64_t frequency_us;
    uint64_t timer_us;
} Task;

// Create a task with frequency in microseconds
#define CREATE_TASK_US(func_ident, frequency_us_val) \
    ((Task) {                                        \
        .func = (func_ident),                        \
        .name = #func_ident,                         \
        .frequency_us = (frequency_us_val),          \
        .timer_us     = (frequency_us_val),          \
    })

// Create a task with frequency in milliseconds
#define CREATE_TASK(func_ident, frequency_ms) \
    CREATE_TASK_US(func_ident, (frequency_ms) * 1000)

typedef struct {
    Task *   tasks;
    size_t   task_count;
    uint64_t last_poll_time_us;
} Scheduler;


void scheduler_init(Scheduler * scheduler, Task * tasks, size_t task_count) {
    *scheduler = (Scheduler) {
        .tasks = tasks,
        .task_count = task_count,
        .last_poll_time_us = 0,
    };
}

void scheduler_poll(Scheduler * scheduler) {
    uint64_t time_us = time_us_64();

    // Check for invalid `last_poll_time` field
    if (scheduler->last_poll_time_us > time_us) {
        scheduler->last_poll_time_us = time_us; return;
    }

    uint64_t dt_us = time_us - scheduler->last_poll_time_us;

    for (uint i = 0; i < scheduler->task_count; i++) {
        Task * task = &scheduler->tasks[i];

        if (task->timer_us < dt_us) {
            // Call the task if the timer has run out
            (task->func)();
            task->timer_us += task->frequency_us;
        } else {
            // Decrement the timer
            task->timer_us -= dt_us;
        }
    }

    scheduler->last_poll_time_us = time_us;
}
