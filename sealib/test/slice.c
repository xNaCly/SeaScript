#include "../slice.h"
#include "test.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void testSlice() {
  CsSlice *s = CsSliceNew(0);
  for (int i = 0; i < 100; i++) {
    int *j = (int *)malloc(sizeof(int));
    *j = i;
    CsSliceAppend(s, j);
  }

  for (size_t i = 0; i < 100; i++) {
    int r = *(int *)CsUnwrap(CsSliceGet(s, i));
    printf("%d\n", r);
  }
}
