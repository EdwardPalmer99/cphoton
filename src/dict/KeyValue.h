/**
 * @file KeyValue.h
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

// A container used in a dictionary.
typedef struct keyValue_t
{
    char *key;
    void *value;
} KeyValue;


// Create a new container copying the key but not the value.
KeyValue *allocKeyValue(const char *key, void *value);

// Free memory allocated for container.
void deallocKeyValue(KeyValue *bucket);
