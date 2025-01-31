/**
 * @file Dictionary.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Dictionary.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHBITS 16
#define HASHSIZE (1U << HASHBITS)
#define MAX_HASHVALUE (HASHSIZE - 1)
#define HASH_CAPACITY (1U << (HASHBITS - 1)) // For 0.5 load factor.

static uint32_t hashKey(const char *str);

// Dictionary structure.
struct dict_t
{
    long numInstalled;
    KeyValue **buckets;
};


bool dictInstall(Dict *dict, const char *key, void *value)
{
    if (!dict || dict->numInstalled >= HASH_CAPACITY || !key)
    {
        return false;
    }

    KeyValue **buckets = dict->buckets;
    if (!buckets) return false;

    for (uint32_t hash = hashKey(key);; hash = (hash + 1) % HASHSIZE)
    {
        if (!buckets[hash])
        {
            buckets[hash] = allocKeyValue(key, value);
            (++dict->numInstalled);
            return true;
        }
    }

    return false;
}


void *dictLookup(Dict *dict, const char *key)
{
    if (!dict || dict->numInstalled < 1 || !key)
    {
        return NULL;
    }

    KeyValue **buckets = dict->buckets;
    if (!buckets) return NULL;

    for (uint32_t hash = hashKey(key);; hash = (hash + 1) % HASHSIZE)
    {
        KeyValue *bucket = buckets[hash];

        if (!bucket)
        {
            return NULL;
        }
        else if (bucket && strcmp(bucket->key, key) == 0)
        {
            return bucket->value;
        }
    }
}


Dict *allocDict(void)
{
    Dict *dict = malloc(sizeof(Dict));
    if (!dict) return NULL;

    dict->numInstalled = 0;
    dict->buckets = calloc(HASHSIZE, sizeof(KeyValue));
    if (!dict->buckets)
    {
        free(dict);
        return NULL;
    }

    return dict;
}


void deallocDict(Dict *dict)
{
    if (!dict) return;

    if (dict->buckets)
    {
        for (int i = 0; i < HASHSIZE; ++i)
        {
            deallocKeyValue(dict->buckets[i]);
        }

        free(dict->buckets);
    }

    free(dict);
}


static uint32_t hashKey(const char *str)
{
    uint32_t hash = 5381;

    for (char *c = (char *)str; *c; c++)
    {
        hash = (hash * 31) + (uint8_t)*c;
    }

    return (hash & MAX_HASHVALUE);
}
