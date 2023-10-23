#include "map.h"
#include "result.h"
#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int FNV_OFFSET_BASIS = 0x811c9dc5;
const unsigned int FNV_PRIME = 0x01000193;

// see map.h preamble for hashing algorithm details
static unsigned int hash_key(CsMap *m, const char *key) {
  int length = strlen(key);
  unsigned int h = FNV_OFFSET_BASIS;
  for (int i = 0; i < length; i++) {
    h ^= (unsigned char)key[i];
    h *= FNV_PRIME;
  }
  return h % m->table_size;
}

CsMap *CsMapNew() {
  CsMap *m = NULL;
  if (m = malloc(sizeof(CsMap)), m == NULL) {
    fprintf(stderr, "failed to malloc map");
    return NULL;
  }
  m->size = 0;
  m->table_size = INITIAL_SIZE;
  m->buckets = (CsSlice **)malloc(sizeof(CsSlice) * INITIAL_SIZE);
  for (int i = 0; i < INITIAL_SIZE; i++) {
    m->buckets[i] = CsSliceNew(8);
  }
  return m;
};

void CsMapPut(CsMap *m, const char *key, void *value) {
  unsigned int hash = hash_key(m, key);
  CsMapElement *e = NULL;
  if (e = malloc(sizeof(CsMapElement)), e == NULL) {
    fprintf(stderr, "failed to allocate map element");
    return;
  }
  e->hasValue = 1;
  e->value = value;
  e->key = key;
  CsSliceAppend(m->buckets[hash], e);
  m->size++;
};

CsResult *CsMapGet(CsMap *m, const char *key) {
  unsigned int hash = hash_key(m, key);
  CsSlice *slice = m->buckets[hash];
  CsMapElement **arr = (CsMapElement **)CsSliceGetArray(slice);
  for (int i = 0; i < slice->len; i++) {
    CsMapElement *t = arr[i];
    if (t->hasValue && strcmp(t->key, key) == 0) {
      return CsSuccess(t->value);
    }
  }
  return CsError("unknown key for map");
}

bool CsMapContains(CsMap *m, const char *key) {
  unsigned int hash = hash_key(m, key);
  CsSlice *slice = m->buckets[hash];
  CsMapElement **arr = (CsMapElement **)CsSliceGetArray(slice);
  for (int i = 0; i < slice->len; i++) {
    CsMapElement *t = arr[i];
    if (t->hasValue && strcmp(t->key, key) == 0) {
      return true;
    }
  }
  return false;
}

void CsMapRemove(CsMap *m, const char *key) {
  unsigned int hash = hash_key(m, key);
  CsSlice *slice = m->buckets[hash];
  CsMapElement **arr = (CsMapElement **)CsSliceGetArray(slice);
  for (int i = 0; i < slice->len; i++) {
    CsMapElement *t = arr[i];
    if (t->hasValue && strcmp(t->key, key) == 0) {
      t->hasValue = 0;
      t->value = 0;
      t->key = 0;
    }
  }
  m->size--;
}

void CsMapFree(CsMap *m) {
  if (m == NULL) {
    return;
  }
  for (int i = 0; i < INITIAL_SIZE; i++) {
    CsSliceFree(m->buckets[i]);
  }
  free(m);
  m = NULL;
};
