/**
 * @file ThreadPool.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "threadpool.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void deallocThreadTasks(ThreadPool *threadPool);


void execute(void *args)
{
    ThreadInfo *threadInfo = (ThreadInfo *)args;
    ThreadPool *threadPool = threadInfo->threadPool;
    pthread_mutex_t *mutex = threadInfo->mutex;

    while (true)
    {
        pthread_mutex_lock(mutex);
        fprintf(stdout, "thread %d: executing task\n", threadInfo->threadID);

        if (threadPool->itask >= threadPool->ntasks)
        {
            fprintf(stdout, "thread %d: no tasks remaining. Exiting!\n", threadInfo->threadID);
            pthread_mutex_unlock(mutex);
            return;
        }

        Task *task = threadPool->tasks[threadPool->itask++];

        pthread_mutex_unlock(mutex);

        task->func(task->args); // Execute function.
    }
}

ThreadPool *allocThreadPool(unsigned int nthreads)
{
    ThreadPool *threadPool = malloc(sizeof(ThreadPool));

    threadPool->nthreads = nthreads;
    threadPool->threads = malloc(sizeof(pthread_t *) * nthreads);

    threadPool->ntasks = 0;
    threadPool->capacity = 0;
    threadPool->itask = 0;
    threadPool->tasks = NULL;

    return threadPool;
}

void deallocThreadTasks(ThreadPool *threadPool)
{
    if (!threadPool || !threadPool->tasks) return;

    for (int itask = 0; itask < threadPool->capacity; ++itask)
    {
        deallocTask(threadPool->tasks[itask]);
    }

    free(threadPool->tasks);

    threadPool->ntasks = 0;
    threadPool->itask = 0;
    threadPool->capacity = 0;
    threadPool->tasks = NULL;
}

void deallocThreadPool(ThreadPool *threadPool)
{
    if (!threadPool) return;

    deallocThreadTasks(threadPool);

    free(threadPool->threads);
    free(threadPool);
}

void addTask(ThreadPool *threadPool, TaskFunc func, TaskArgs args, size_t argsSize)
{
    if (threadPool->capacity == 0 || threadPool->ntasks >= threadPool->capacity)
    {
        if (threadPool->capacity == 0)
            threadPool->capacity = 1;
        else
            threadPool->capacity <<= 2;

        Task **newPtr = realloc(threadPool->tasks, sizeof(Task *) * threadPool->capacity);
        if (newPtr)
        {
            threadPool->tasks = newPtr;
        }
        else
        {
            fprintf(stderr, "failed to resize thread pool tasks queue!\n");
            exit(EXIT_FAILURE);
        }
    }

    threadPool->tasks[threadPool->ntasks++] = allocTask(func, args, argsSize);
}

void executeTasks(ThreadPool *threadPool)
{
    if (!threadPool || threadPool->ntasks < 1 || threadPool->nthreads < 1)
    {
        return;
    }

    // Resize capacity to fit number of tasks to save memory.
    if (threadPool->capacity > threadPool->ntasks)
    {
        threadPool->capacity = threadPool->ntasks;
        threadPool->tasks = realloc(threadPool->tasks, sizeof(Task *) * threadPool->capacity);
    }

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

    // Remove executed tasks. We can now reuse the thread pool.
    deallocThreadTasks(threadPool);
}
