/*
The map.h component exposes a very rudimentary hash table. The API makes use of
CsResult.

The hash table hashes keys using the FNV-1a algorithm. It's used because its
faster than the previously used multiplication hash algorithm and has a uniform
distribution, which should keep hash colliosions to minimum.

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

Beware, this will change in the feature - seperate chaining will be employed in
the future.

Guide for using this component:

1. Create a new hash table:

    CsMap *m = CsMapNew();

2. Put elements in the map:

    double *val1 = malloc(sizeof(double));
    *val1 = 25 * 25;
    double *val2 = malloc(sizeof(double));
    *val2 = 50 * 50;
    CsMapPut(m, 25, val1);
    CsMapPut(m, 50, val2);

3. Get elements from the map:


    double *r1 = (double *)CsUnwrap(CsMapGet(m, 25));
    double *r2 = (double *)CsUnwrap(CsMapGet(m, 50));

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
} CsMapElement;

typedef struct {
  // elements count
  int size;
  // possible space
  int table_size;
  // buckets
  CsMapElement *entries;
} CsMap;

// allocates a new map, with size of INITIAL_SIZE, performs initial golden
// ration computation
CsMap *CsMapNew();

// copies value, insert copy for key into map, mind the load factor!
void CsMapPut(CsMap *map, const char *key, void *value);

// checks if an object with the given key is present in the map
bool CsMapContains(CsMap *m, const char *key);

// get a pointer to a value from the map by key
CsResult *CsMapGet(CsMap *map, const char *key);

// remove an object with the given key from the map
void CsMapRemove(CsMap *map, const char *key);

// destroy the map, set point to NULL
void CsMapFree(CsMap *map);

#endif
