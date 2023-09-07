#include "map.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

// see map.h preamble for hashing algorithm details
static unsigned int hash_key(SeaScriptMap *m, unsigned int key) {
  unsigned int mul = floor(m->table_size * m->golden_ratio * key);
  return mul % m->table_size;
}

// The performance of the hash table deteriorates in relation to the load
// factor, hash table should be resized and rehased if the load factor
// approaches 1. Acceptable values are: a < 0.6, 0.6 < 0.75. Load factor is
// determined by dividing the amount of current elements in the hash table by
// the possible size of the hash table.
static double load_factor(SeaScriptMap *m) {
  return m->size / (double)m->table_size;
}

SeaScriptMap *SeaScriptMapNew() {
  SeaScriptMap *m = malloc(sizeof(SeaScriptMap));
  m->golden_ratio = (1.0 + sqrt(5)) / 2.0;
  m->size = 0;
  m->table_size = INITIAL_SIZE;
  m->entries = calloc(INITIAL_SIZE, sizeof(SeaScriptMapElement));
  return m;
};

bool SeaScriptMapContains(SeaScriptMap *m, unsigned int key) {
  unsigned int hash = hash_key(m, key);
  SeaScriptMapElement entry = m->entries[hash];
  while (entry.hasValue && entry.key != key) {
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
  }
  return entry.key == key && entry.hasValue;
}

void SeaScriptMapPut(SeaScriptMap *m, unsigned int key, void *value) {
  if (m->size == m->table_size) {
    return;
  }
  unsigned int hash = hash_key(m, key);
  SeaScriptMapElement entry = m->entries[hash];
  while (entry.hasValue && entry.key != key) {
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
  }
  m->entries[hash].key = key;
  m->entries[hash].value = value;
  m->entries[hash].hasValue = 1;
  m->size++;
};

SeaScriptResult *SeaScriptMapGet(SeaScriptMap *m, unsigned int key) {
  unsigned int hash = hash_key(m, key);
  if (&m->entries[hash] == NULL) {
    return SeaScriptResultNewError("key not found");
  }
  SeaScriptMapElement entry = m->entries[hash];
  while (entry.hasValue && entry.key != key) {
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
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
  m->entries[hash].hasValue = 0;
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
