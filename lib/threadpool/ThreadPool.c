/**
 * @file ThreadPool.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "threadpool/ThreadPool.h"
#include "logger/Logger.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


void execute(void *args)
{
    ThreadInfo *threadInfo = (ThreadInfo *)args;
    ThreadPool *threadPool = threadInfo->threadPool;
    pthread_mutex_t *mutex = threadInfo->mutex;

    while (true)
    {
        pthread_mutex_lock(mutex);
        // fprintf(stdout, "thread %d: executing task\n", threadInfo->threadID);

        if (!threadPool->task)
        {
            // fprintf(stdout, "thread %d: no tasks remaining. Exiting!\n", threadInfo->threadID);
            pthread_mutex_unlock(mutex);
            return;
        }

        Task *task = threadPool->task;
        threadPool->task = threadPool->task->next;

        pthread_mutex_unlock(mutex);

        task->func(task->args); // Execute function.

        // Task completed. Dealloc memory.
        deallocTask(task);
    }
}

ThreadPool *allocThreadPool(unsigned int nthreads)
{
    ThreadPool *threadPool = malloc(sizeof(ThreadPool));

    threadPool->nthreads = nthreads;
    threadPool->threads = malloc(sizeof(pthread_t *) * nthreads);
    threadPool->base = NULL;
    threadPool->task = NULL;

    return threadPool;
}


void deallocThreadPool(ThreadPool *threadPool)
{
    if (!threadPool) return;

    if (threadPool->base) // Tasks were not executed. Cleanup.
    {
        Task *ptr = threadPool->base;
        while (ptr)
        {
            Task *next = ptr->next;
            deallocTask(ptr);
            ptr = next;
        }
    }

    free(threadPool->threads);
    free(threadPool);
}

void addTask(ThreadPool *threadPool, TaskFunc func, TaskArgs args, size_t argsSize)
{
    if (!threadPool->base)
    {
        threadPool->base = allocTask(func, args, argsSize);
        threadPool->task = threadPool->base;
    }
    else
    {
        threadPool->task->next = allocTask(func, args, argsSize);
        threadPool->task = threadPool->task->next;
    }
}

void executeTasks(ThreadPool *threadPool)
{
    if (!threadPool || !threadPool->base || threadPool->nthreads < 1)
    {
        Logger(LogLevelFailed, "Invalid arguments");
        abort();
    }

    // Set thread pool pointer to base.
    threadPool->task = threadPool->base;

    ThreadInfo threadInfo[threadPool->nthreads];

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    for (int ithread = 0; ithread < threadPool->nthreads; ++ithread)
    {
        ThreadInfo *infoForThread = (threadInfo + ithread);
        infoForThread->threadID = (ithread + 1);
        infoForThread->threadPool = threadPool;
        infoForThread->mutex = &mutex;

        pthread_create(threadPool->threads + ithread, NULL, (void *)&(execute), (void *)infoForThread);
    }

    for (int ithread = 0; ithread < threadPool->nthreads; ++ithread)
    {
        pthread_join(threadPool->threads[ithread], NULL);
    }

    pthread_mutex_destroy(&mutex);

    // Tasks executed and deallocted.
    threadPool->task = threadPool->base = NULL;
}
