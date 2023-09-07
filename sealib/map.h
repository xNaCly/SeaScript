/*
The map.h component exposes a very rudimentary hash table. The API makes use of
SeaScriptResult.

The hash table hashes keys using the FNV-1a algorithm. It's used because its
faster than the previously used multiplication hash algorithm.

    hash = FNV_OFFSET_BASIS

    for byte in bytes
        hash ^= byte
        hash *= FNV_PRIME

Both FNV_PRIME and FNV_OFFSET_BASIS were taken from:
https://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function#FNV-1a_hash .

While inserting elements into the hash tables bucket, linear
probing is performed if said bucket is already occupied. This can deteriorate
the hash tables performance when putting or getting - mind the load factor for
a healthy table.

Guide for using this component:

1. Create a new hash table:

    SeaScriptMap *m = SeaScriptMapNew();

2. Put elements in the map:

    double *val1 = malloc(sizeof(double));
    *val1 = 25 * 25;
    double *val2 = malloc(sizeof(double));
    *val2 = 50 * 50;
    SeaScriptMapPut(m, 25, val1);
    SeaScriptMapPut(m, 50, val2);

3. Get elements from the map:


    double *r1 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, 25));
    double *r2 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, 50));

    assert(*r1 == 25 * 25);
    assert(*r1 == 50 * 50);

 */
#ifndef MAP_H
#define MAP_H

#include "result.h"
#include <stdbool.h>
#define INITIAL_SIZE 1024

typedef struct {
  const char *key;
  void *value;
  char hasValue;
} SeaScriptMapElement;

typedef struct {
  // elements count
  int size;
  // possible space
  int table_size;
  // buckets
  SeaScriptMapElement *entries;
} SeaScriptMap;

// allocates a new map, with size of INITIAL_SIZE, performs initial golden
// ration computation
SeaScriptMap *SeaScriptMapNew();

// copies value, insert copy for key into map, mind the load factor!
void SeaScriptMapPut(SeaScriptMap *map, const char *key, void *value);

// checks if an object with the given key is present in the map
bool SeaScriptMapContains(SeaScriptMap *m, const char *key);

// get a pointer to a value from the map by key
SeaScriptResult *SeaScriptMapGet(SeaScriptMap *map, const char *key);

// remove an object with the given key from the map
void SeaScriptMapRemove(SeaScriptMap *map, const char *key);

// destroy the map, set point to NULL
void SeaScriptMapFree(SeaScriptMap *map);

#endif
