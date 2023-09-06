#include "../list.h"
#include "../result.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

SeaScriptList *createList() {
  tlog("list creation");
  SeaScriptList *l = SeaScriptListNew();
  assert(l != NULL);
  return l;
}

void appendList(SeaScriptList *l) {
  tlog("element add");
  assert(l != NULL);
  int lastSize = 0;
  for (int i = 0; i < 100; i++) {
    int *el = malloc(sizeof(int));
    *el = i;
    SeaScriptListAppend(l, el);
    assert(lastSize < l->size);
    lastSize++;
  }
}

void removeFromList(SeaScriptList *l) {
  tlog("element del");
  assert(l != NULL);
  for (int i = 99; i != 0; i--) {
    SeaScriptResult *r = SeaScriptListPop(l);
    int *val = (int *)r->value;
    assert(!r->hasError);
    assert(*val == i);
    free(val);
    free(r);
  }
}

void getFromList(SeaScriptList *l) {
  tlog("element get");
  assert(l != NULL);
  for (int i = 0; i < 100; i++) {
    assert(*(int *)SeaScriptResultUnwrap(SeaScriptListGet(l, i)) == i);
  }
}

void freeList(SeaScriptList *l) {
  tlog("list free");
  assert(l != NULL);
  SeaScriptListFree(l);
}

void testList() {
  SeaScriptList *l = createList();
  appendList(l);
  getFromList(l);
  removeFromList(l);
  freeList(l);
}
