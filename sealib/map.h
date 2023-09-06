/*
The map.h component exposes a very rudimentary hash table. The API makes use of
SeaScriptResult. The hashing is implemented with multiplication and uses the
golden ration for the following hashing formula:

    h(x) = m*A*x % m

    Where
    - x is the key to hash
    - m is the table size
    - A is the golden ratio

The hash table does currently not contain any linear probing, therefore
collisions are to be expected for more than 100 simultaneously existing
elements in the table.

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
#include <math.h>
#define INITIAL_SIZE 1024

typedef struct {
  int key;
  void *value;
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

// allocates a new map
SeaScriptMap *SeaScriptMapNew();

// insert value for key into map
void SeaScriptMapPut(SeaScriptMap *map, int key, void *value);

// get an pointer to an value from the map via key
SeaScriptResult *SeaScriptMapGet(SeaScriptMap *map, int key);

// remove an object with the given key
void SeaScriptMapRemove(SeaScriptMap *map, int key);

// destroy the map, set point to NULL
void SeaScriptMapFree(SeaScriptMap *map);

#endif
