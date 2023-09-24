/*
The slice.h component contains a abstract datatype inspired by golangs
implementation of dynamically growing arrays. It makes heavy use of
CsResult for return types, takes ownership of inserted elements and grows
the underlying array.

Features:

    - performs bound checking for indexing into the underlying array
    - grows and shrinks depending on amount of elements in the underlying array

*/
#ifndef SLICE_H
#define SLICE_H

#include "result.h"
#include <stdint.h>

typedef struct {
  // list of elements
  void **elements;
  // count of elements currenlty in list
  int64_t len;
  // maxium size of Slice
  int64_t cap;
} CsSlice;

// adds an item to the underlying array, if s.len would be bigger than s.cap
// after insertion, doubles the size of the underlying array
void CsSliceAppend(CsSlice *s, void *element);

// returns the given element if 0 <= index < s.len
CsResult *CsSliceGet(CsSlice *s, int index);

// returns pointer to the underlying array
void **CsSliceGetArray(CsSlice *s);

#endif
