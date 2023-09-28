#include "slice.h"
#include "result.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLICE_MIN_SIZE 8

CsSlice *CsSliceNew(size_t initial_size) {
  CsSlice *s = NULL;
  if (s = malloc(sizeof(CsSlice)), s == NULL) {
    fprintf(stderr, "sealib: failed to allocate a slice");
    exit(EXIT_FAILURE);
  }
  s->elements = NULL;
  if (s->elements = malloc(initial_size * sizeof(void *)),
      s->elements == NULL) {
    fprintf(stderr,
            "sealib: failed to allocate the underlying array of size %zu\n",
            initial_size);
    exit(EXIT_FAILURE);
  }
  s->cap = initial_size < SLICE_MIN_SIZE ? SLICE_MIN_SIZE : initial_size;
  s->len = 0;
  return s;
}

// duplicates the cap of the slice and the allocated space of the underlying
// array
static void grow_slice(CsSlice *s) {
  void **t = NULL;
  if (t = malloc(s->cap * 2 * sizeof(void *)), t == NULL) {
    fprintf(stderr,
            "sealib: failed to grow the slice from %zu to %zu (allocation "
            "failed)\n",
            s->cap, s->cap * 2);
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < s->len; i++) {
    t[i] = s->elements[i];
  }
  free(s->elements);
  s->elements = t;
  s->cap *= 2;
}

void CsSliceAppend(CsSlice *s, void *element) {
  // if we append and the new len is bigger or equal to the size we double the
  // slice, therefore we make a good tradeoff between times we need to grow
  // the slice and the amount of memory we take up
  if (s->len + 1 > s->cap) {
    grow_slice(s);
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

void CsSliceFree(CsSlice *s) {
  free(s->elements);
  free(s);
  s = NULL;
}
