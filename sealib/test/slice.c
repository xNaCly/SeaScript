#include "../slice.h"
#include "test.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

CsSlice *createSlice() { return CsSliceNew(0); }
void appendSlice(CsSlice *s) {
  for (int i = 0; i < 100; i++) {
    int *j = (int *)malloc(sizeof(int));
    *j = i;
    CsSliceAppend(s, j);
  }
}

void getSlice(CsSlice *s) {
  for (size_t i = 0; i < 100; i++) {
    int r = *(int *)CsUnwrap(CsSliceGet(s, i));
    assert(r == i);
  }
}

void testSlice() {
  CsSlice *s = createSlice();
  appendSlice(s);
  getSlice(s);
}
