#include "map.h"
#include "result.h"
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

// The performance of the hash table deteriorates in relation to the load
// factor, hash table should be resized and rehased if the load factor
// approaches 1. Acceptable values are: a < 0.6, 0.6 < 0.75. Load factor is
// determined by dividing the amount of current elements in the hash table by
// the possible size of the hash table.
static double load_factor(CsMap *m) { return m->size / (double)m->table_size; }

CsMap *CsMapNew() {
  CsMap *m = NULL;
  if (m = malloc(sizeof(CsMap)), m == NULL) {
    fprintf(stderr, "sealib: %s\n", "failed to allocate hash table");
    exit(EXIT_FAILURE);
  }
  m->size = 0;
  m->table_size = INITIAL_SIZE;
  m->entries = NULL;
  if (m->entries = calloc(INITIAL_SIZE, sizeof(CsMapElement)), m == NULL) {
    fprintf(stderr, "sealib: %s\n",
            "failed to allocate underlying buckets for hash table");
    exit(EXIT_FAILURE);
  }

  return m;
};

bool CsMapContains(CsMap *m, const char *key) {
  unsigned int hash = hash_key(m, key);
  CsMapElement entry = m->entries[hash];
  int i = 0;
  while (entry.key != 0 && strcmp(key, entry.key) != 0) {
    if (i >= m->table_size)
      break;
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
    i++;
  }
  if (entry.value == NULL)
    return false;
  return strcmp(key, entry.key) == 0;
}

void CsMapPut(CsMap *m, const char *key, void *value) {
  if (m->size == m->table_size || load_factor(m) > 0.99) {
    fprintf(
        stderr,
        "failed to put into map at key %s - map either full or deteriorated\n",
        key);
    return;
  }
  unsigned int hash = hash_key(m, key);
  CsMapElement entry = m->entries[hash];
  int i = 0;
  while (entry.hasValue && strcmp(key, entry.key) != 0) {
    if (i >= m->table_size)
      break;
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
    i++;
  }
  m->entries[hash].key = key;
  m->entries[hash].value = value;
  m->entries[hash].hasValue = 1;
  m->size++;
};

CsResult *CsMapGet(CsMap *m, const char *key) {
  unsigned int hash = hash_key(m, key);
  CsMapElement entry = m->entries[hash];
  int i = 0;
  while (entry.key != 0 && strcmp(key, entry.key) != 0) {
    if (i >= m->table_size)
      break;
    hash = (hash + 1) % m->table_size;
    entry = m->entries[hash];
    i++;
  }
  if (entry.value == NULL) {
    return CsError("key not found");
  }

  return CsSuccess(m->entries[hash].value);
}

// TODO: fix this
/* void CsMapRemove(CsMap *m, const char *key) { */
/*   unsigned int hash = hash_key(m, key); */
/*   CsMapElement entry = m->entries[hash]; */
/*   int i = 0; */
/*   while (entry.key != 0 && strcmp(key, entry.key) != 0) { */
/*     if (i >= m->table_size) */
/*       break; */
/*     hash = (hash + 1) % m->table_size; */
/*     entry = m->entries[hash]; */
/*     i++; */
/*   } */
/*   if (entry.value == NULL || entry.hasValue == 0) */
/*     return; */
/*   m->entries[hash].key = 0; */
/*   m->entries[hash].value = NULL; */
/*   m->entries[hash].hasValue = 0; */
/*   m->size--; */
/* } */

void CsMapFree(CsMap *m) {
  if (m == NULL) {
    return;
  }
  free(m->entries);
  free(m);
  m = NULL;
};
