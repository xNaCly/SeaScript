#include "map.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

// uses Hashing by multiplication to compute a hash for the given key
// https://en.wikipedia.org/wiki/Golden_ratio
static unsigned int hash_key(SeaScriptMap *m, unsigned int key) {
  return (int)floor(m->table_size * m->golden_ratio * key) % m->table_size;
}

static double load_factor(SeaScriptMap *m) { return m->size / m->table_size; }

SeaScriptMap *SeaScriptMapNew() {
  SeaScriptMap *m = malloc(sizeof(SeaScriptMap));
  m->golden_ratio = (1.0 + sqrt(5)) / 2.0;
  m->size = 0;
  m->table_size = INITIAL_SIZE;
  m->entries = calloc(INITIAL_SIZE, sizeof(SeaScriptMapElement));
  return m;
};

void SeaScriptMapPut(SeaScriptMap *m, int key, void *value) {
  if (m->size == m->table_size) {
    return;
  }
  unsigned int hash = hash_key(m, key);
  m->entries[hash].key = key;
  m->entries[hash].value = value;
  m->size++;
};

SeaScriptResult *SeaScriptMapGet(SeaScriptMap *m, int key) {
  unsigned int hash = hash_key(m, key);
  if (&m->entries[hash] == NULL) {
    return SeaScriptResultNewError("key not found");
  }
  return SeaScriptResultNewSuccess(m->entries[hash].value);
}

void SeaScriptMapRemove(SeaScriptMap *m, int key) {
  unsigned int hash = hash_key(m, key);
  if (&m->entries[hash] == NULL) {
    return;
  }
  m->entries[hash].key = 0;
  m->entries[hash].value = NULL;
  m->size--;
}

void SeaScriptMapFree(SeaScriptMap *m) {
  if (m == NULL) {
    return;
  }
  free(m->entries);
  free(m);
  m = NULL;
};
