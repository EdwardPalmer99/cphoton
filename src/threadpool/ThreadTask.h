/**
 * @file ThreadTask.h
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdlib.h>

typedef void (*TaskFunc)(void *);
typedef void *TaskArgs;

typedef struct task_t
{
    TaskFunc func;
    TaskArgs args;
} Task;


// Allocate a new thread task with a function and a pointer to the function arguments. This pointer can be NULL. The
// arguments will be copied and stored with the task.
Task *allocTask(TaskFunc func, TaskArgs args, size_t argsSize);

// Free memory allocated to a task instance.
void deallocTask(Task *task);
