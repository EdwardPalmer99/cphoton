/**
 * @file KeyValue.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "dict/KeyValue.h"
#include <stdlib.h>
#include <string.h>

KeyValue *allocKeyValue(const char *key, void *value)
{
    if (!key || !value) return NULL;

    KeyValue *bucket = malloc(sizeof(KeyValue));
    bucket->key = strdup(key); // Copy key.
    bucket->value = value;     // NB: dictionary does not assume ownership!

    return bucket;
}


void deallocKeyValue(KeyValue *bucket)
{
    if (!bucket) return;

    if (bucket->key) free(bucket->key);
    free(bucket);
}