/**
 * @file ThreadPool.h
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "threadpool/ThreadTask.h"
#include <pthread.h>

// TODO: - use linked list.
typedef struct threadPool_t
{
    unsigned int nthreads;
    pthread_t *threads;

    Task *base;
    Task *task;
} ThreadPool;

typedef struct threadInfo_t
{
    int threadID;
    ThreadPool *threadPool;
    pthread_mutex_t *mutex;
} ThreadInfo;

// Creates a new thread pool with nthreads.
ThreadPool *allocThreadPool(unsigned int nthreads);

// Frees memory allocated for thread pool.
void deallocThreadPool(ThreadPool *threadPool);

// Adds a new task to an existing thread pool.
void addTask(ThreadPool *threadPool, TaskFunc func, TaskArgs args, size_t argsSize);

// Execute all tasks in thread pool. Once completed, tasks will be removed. The thread pool can then be reused.
void executeTasks(ThreadPool *threadPool);