#ifndef MAP_H
#define MAP_H

#include "result.h"
#define INITIAL_SIZE 1024

typedef struct {
  int key;
  void *value;
} SeaScriptMapElement;

typedef struct {
  int size;
  int table_size;
  SeaScriptMapElement *entries;
} SeaScriptMap;

// allocate a new map
SeaScriptMap *SeaScriptMapNew();
// insert value for key into map
void SeaScriptMapPut(SeaScriptMap *map, int key, void *value);
// get an object from the map via key
SeaScriptResult *SeaScriptMapGet(SeaScriptMap *map, int key);
// remove an object with the given key
void SeaScriptMapRemove(SeaScriptMap *map, int key);
// destroy the map, set point to NULL
void SeaScriptMapFree(SeaScriptMap *map);

#endif
