#include "../map.h"
#include "../result.h"
#include "test.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void printMap(SeaScriptMap *m) {
  printf("[\n");
  for (int i = 0; i < m->table_size; i++) {
    SeaScriptMapElement e = m->entries[i];
    if (e.hasValue) {
      printf("\t{%s:%f},\n", e.key, *(double *)e.value);
    }
  }
  printf("]\n");
}

SeaScriptMap *createMap() {
  tlog("map creation");
  SeaScriptMap *m = SeaScriptMapNew();
  assert(m != NULL);
  return m;
}

void appendMap(SeaScriptMap *m) {
  tlog("entry putting");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    double *d = malloc(sizeof(double));
    *d = i * i;
    SeaScriptMapPut(m, result, d);
  }
  assert(m->size == 999);
}

void containsMap(SeaScriptMap *m) {
  tlog("entry contain check");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    bool r = SeaScriptMapContains(m, result);
    if (!r) {
      printf("%s\n", result);
    }
    assert(r);
  }
}

void getMap(SeaScriptMap *m) {
  tlog("entry getting");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    double *r = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, result));
    if (*r != i * i) {
      printf("%s: %d*%d != %f\n", result, i, i, *r);
    }
    assert(*r == i * i);
  }
}

/* void deleteMap(SeaScriptMap *m) { */
/*   tlog("entry deletion"); */
/*   for (int i = 1; i < 1000; i++) { */
/*     char *result = malloc(sizeof(char) * 25); */
/*     sprintf(result, "test%d", i); */
/*     double *r = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, result));
 */

/*     free(r); */
/*     SeaScriptMapRemove(m, result); */
/*   } */
/*   assert(m->size == 0); */
/* } */

void freeMap(SeaScriptMap *map) { SeaScriptMapFree(map); }

void example() {
  SeaScriptMap *m = SeaScriptMapNew();
  double *val1 = malloc(sizeof(double));
  *val1 = 25 * 25;
  double *val2 = malloc(sizeof(double));
  *val2 = 50 * 50;
  SeaScriptMapPut(m, "test25", val1);
  SeaScriptMapPut(m, "test50", val2);
  double *r1 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, "test25"));
  double *r2 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, "test50"));
  assert(*r1 == 25 * 25);
  assert(*r2 == 50 * 50);
  printMap(m);
  free(r1);
  free(r2);
  /* SeaScriptMapRemove(m, "test25"); */
  /* SeaScriptMapRemove(m, "test50"); */
}

void testMap() {
  SeaScriptMap *m = createMap();
  appendMap(m);
  containsMap(m);
  getMap(m);
  /* deleteMap(m); */
  freeMap(m);
  example();
}
