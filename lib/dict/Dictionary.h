/**
 * @file Dictionary.h
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "dict/KeyValue.h"
#include <stdbool.h>

// Forward declarations to hide implementation.
struct dict_t;
typedef struct dict_t Dict;

// Returns an empty dictionary.
Dict *allocDict(void);

// Frees a dictionary.
void deallocDict(Dict *dict);

// Looksup an item in the dictionary.
void *dictLookup(Dict *dict, const char *key);

// Installs key, value-pair in the dictionary. The key is copied but not the value!
bool dictInstall(Dict *dict, const char *key, void *value);
