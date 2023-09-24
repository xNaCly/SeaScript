#include "slice.h"
#include "result.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLICE_MIN_SIZE 8

CsSlice *CsSliceNew(size_t initial_size) {
  CsSlice *s = malloc(sizeof(CsSlice));
  s->elements = calloc(initial_size, sizeof(void));
  s->cap = initial_size < SLICE_MIN_SIZE ? SLICE_MIN_SIZE : initial_size;
  s->len = 0;
  return s;
}

// TODO: i ran into so much issues while trying to implement this without
// creating a new Slice, but i got so many invalid next sizes and corrupted
// tops i gave up, so this is the best you get:
/* static void grow_slice(CsSlice *s) { */
/*   CsSlice *n = CsSliceNew(s->cap * 2); */
/*   for (int i = 0; i < s->len; i++) { */
/*     CsSliceAppend(s, s->elements[i]); */
/*   } */
/*   *s = *n; */
/* } */

void CsSliceAppend(CsSlice *s, void *element) {
  // if we append and the new len is bigger or equal to the size we double the
  // slice, therefore we make a good tradeoff between times we need to grow
  // the slice and the amount of memory we take up
  if (s->len + 1 > s->cap) {
    // TODO: we should grow the array here but i am too retarded to implement
    // it, corrupted tops here, segmentation faults there
    // grow_slice(s);
    return;
  }

  s->elements[s->len] = element;
  s->len++;
}

CsResult *CsSliceGet(CsSlice *s, int index) {
  if (index < 0) {
    return CsError("negative indexing into slice not allowed");
  }
  if (index >= s->cap || index >= s->len) {
    return CsError("index out of bounds");
  }
  return CsSuccess(s->elements[index]);
}

void **CsSliceGetArray(CsSlice *s) { return s->elements; }
