#include "../map.h"
#include "../result.h"
#include "test.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* static void printMap(CsMap *m) { */
/*   printf("[\n"); */
/*   for (int i = 0; i < m->table_size; i++) { */
/*     CsMapElement e = m->entries[i]; */
/*     if (e.hasValue) { */
/*       printf("\t{%s:%f},\n", e.key, *(double *)e.value); */
/*     } */
/*   } */
/*   printf("]\n"); */
/* } */

CsMap *createMap() {
  tlog("map creation");
  CsMap *m = CsMapNew();
  assert(m != NULL);
  return m;
}

void appendMap(CsMap *m) {
  tlog("entry putting");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    double *d = malloc(sizeof(double));
    *d = i * i;
    CsMapPut(m, result, d);
  }
  assert(m->size == 999);
}

void containsMap(CsMap *m) {
  tlog("entry contain check");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    bool r = CsMapContains(m, result);
    if (!r) {
      printf("%s\n", result);
    }
    assert(r);
  }
}

void getMap(CsMap *m) {
  tlog("entry getting");
  for (int i = 1; i < 1000; i++) {
    char *result = malloc(sizeof(char) * 25);
    sprintf(result, "test%d", i);
    double *r = (double *)CsUnwrap(CsMapGet(m, result));
    if (*r != i * i) {
      printf("%s: %d*%d != %f\n", result, i, i, *r);
    }
    assert(*r == i * i);
  }
}

/* void deleteMap(CsMap *m) { */
/*   tlog("entry deletion"); */
/*   for (int i = 1; i < 1000; i++) { */
/*     char *result = malloc(sizeof(char) * 25); */
/*     sprintf(result, "test%d", i); */
/*     double *r = (double *)CsResultUnwrap(CsMapGet(m, result));
 */

/*     free(r); */
/*     CsMapRemove(m, result); */
/*   } */
/*   assert(m->size == 0); */
/* } */

void freeMap(CsMap *map) { CsMapFree(map); }

void example() {
  CsMap *m = CsMapNew();
  double *val1 = malloc(sizeof(double));
  *val1 = 25 * 25;
  double *val2 = malloc(sizeof(double));
  *val2 = 50 * 50;
  CsMapPut(m, "test25", val1);
  CsMapPut(m, "test50", val2);
  double *r1 = (double *)CsUnwrap(CsMapGet(m, "test25"));
  double *r2 = (double *)CsUnwrap(CsMapGet(m, "test50"));
  assert(*r1 == 25 * 25);
  assert(*r2 == 50 * 50);
  free(r1);
  free(r2);
  /* CsMapRemove(m, "test25"); */
  /* CsMapRemove(m, "test50"); */
}

void testMap() {
  CsMap *m = createMap();
  appendMap(m);
  containsMap(m);
  getMap(m);
  /* deleteMap(m); */
  freeMap(m);
  example();
}
