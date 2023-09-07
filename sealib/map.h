/*
The map.h component exposes a very rudimentary hash table. The API makes use of
SeaScriptResult. The hashing is implemented with multiplication and uses the
golden ration for the following hashing formula:

    h(x) = m*A*x % m

    Where
    - x is the key to hash
    - m is the table size
    - A is the golden ratio

While inserting elements into the hash tables bucket, linear probing is
performed if said bucket is already occupied. This can deteriorate the hash
tables performance when putting or getting - mind the load factor for a healthy
table.

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

4. Remove elements from the map:

    free(&r1);
    free(&r2);

    SeaScriptMapRemove(m, 25);
    SeaScriptMapRemove(m, 50);
 */
#ifndef MAP_H
#define MAP_H

#include "result.h"
#include <bits/stdint-intn.h>
#include <math.h>
#include <stdbool.h>
#define INITIAL_SIZE 1024

typedef struct {
  int key;
  void *value;
  int8_t hasValue;
} SeaScriptMapElement;

typedef struct {
  // contains golden ration constant for hashing:
  // https://en.wikipedia.org/wiki/Golden_ratio
  double golden_ratio;
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
void SeaScriptMapPut(SeaScriptMap *map, unsigned int key, void *value);

// checks if an object with the given key is present in the map
bool SeaScriptMapContains(SeaScriptMap *m, unsigned int key);

// get a pointer to a value from the map by key
SeaScriptResult *SeaScriptMapGet(SeaScriptMap *map, unsigned int key);

// remove an object with the given key from the map
void SeaScriptMapRemove(SeaScriptMap *map, int key);

// destroy the map, set point to NULL
void SeaScriptMapFree(SeaScriptMap *map);

#endif
