#include "../map.h"
#include "../result.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

SeaScriptMap *createMap() {
  tlog("map creation");
  SeaScriptMap *m = SeaScriptMapNew();
  assert(m != NULL);
  return m;
}

void appendMap(SeaScriptMap *m) {
  tlog("entry putting");
  for (int i = 1; i < 100; i++) {
    double *val = malloc(sizeof(double));
    *val = i * i;
    SeaScriptMapPut(m, i, val);
  }
  assert(m->size == 99);
}

void getMap(SeaScriptMap *m) {
  tlog("entry getting");
  for (int i = 1; i < 100; i++) {
    double r = *(double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, i));
    assert(r == i * i);
  }
}

void deleteMap(SeaScriptMap *m) {
  tlog("entry deletion");
  for (int i = 1; i < 100; i++) {
    double *r = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, i));
    free(r);
    SeaScriptMapRemove(m, i);
  }
  assert(m->size == 0);
}

void freeMap(SeaScriptMap *map) { SeaScriptMapFree(map); }

void example() {
  SeaScriptMap *m = SeaScriptMapNew();
  double *val1 = malloc(sizeof(double));
  *val1 = 25 * 25;
  double *val2 = malloc(sizeof(double));
  *val2 = 50 * 50;
  SeaScriptMapPut(m, 25, val1);
  SeaScriptMapPut(m, 50, val2);
  double *r1 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, 25));
  double *r2 = (double *)SeaScriptResultUnwrap(SeaScriptMapGet(m, 50));
  assert(*r1 == 25 * 25);
  assert(*r2 == 50 * 50);
  free(r1);
  free(r2);
  SeaScriptMapRemove(m, 25);
  SeaScriptMapRemove(m, 50);
}

void testMap() {
  SeaScriptMap *m = createMap();
  appendMap(m);
  getMap(m);
  deleteMap(m);
  freeMap(m);
  example();
}
