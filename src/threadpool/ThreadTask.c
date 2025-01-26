/**
 * @file ThreadTask.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ThreadTask.h"
#include <string.h>

Task *allocTask(TaskFunc func, TaskArgs args, size_t argsSize)
{
    Task *task = malloc(sizeof(Task));

    // Set next pointer to NULL.
    task->next = NULL;

    // Assume function pointer is safe. Address isn't going to change.
    task->func = func;

    // If args not NULL then we need to copy! Otherwise user will have to keep pointer valid for lifetime of program.
    if (!args)
    {
        task->args = NULL;
    }
    else
    {
        task->args = malloc(argsSize);
        memcpy(task->args, args, argsSize);
    }

    return task;
}


void deallocTask(Task *task)
{
    if (!task) return;

    if (task->args)
    {
        free(task->args);
    }

    free(task);
}