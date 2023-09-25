/*
The slice.h component contains an abstract datatype inspired by golangs
implementation of dynamically growing arrays. It makes heavy use of
CsResult for return types, does not take ownership of inserted elements and
grows the underlying array as required.

Features:

- performs bound checking for indexing into the underlying array
- grows the array depending on required amount of elements in the underlying
  array

Guide:

1. Create a new slice:

    CsSlice *s = CsSliceNew(0);

2. Append items to the slice:

    for (int i = 0; i < 100; i++) {
        int *j = (int *)malloc(sizeof(int));
        *j = i;
        CsSliceAppend(s, j);
    }

3. Access elements in the slice


    for (size_t i = 0; i < 100; i++) {
        int r = *(int *)CsUnwrap(CsSliceGet(s, i));
        printf("%d\n", r);
    }

4. Free the slice:

    CsSliceFree(s)
*/
#ifndef SLICE_H
#define SLICE_H

#include "result.h"
#include <stddef.h>

typedef struct CsSlice {
  // list of elements
  void **elements;
  // count of elements currenlty in list
  size_t len;
  // maxium size of Slice
  size_t cap;
} CsSlice;

// creates and returns a new slice, if initial_size is less than
// SLICE_MIN_SIZE, initial_size gets set to SLICE_MIN_SIZE.
CsSlice *CsSliceNew(size_t initial_size);

// inserts element at the given index, if s.len would be bigger than s.cap
// after insertion, doubles the size of the underlying array
void CsSliceAppend(CsSlice *s, void *element);

// returns the given element if 0 <= index < s.len
CsResult *CsSliceGet(CsSlice *s, int index);

// returns pointer to the underlying array
void **CsSliceGetArray(CsSlice *s);

// frees the allocated memory region for the given Slice, sets the
// pointer to point to NULL
void CsSliceFree(CsSlice *s);

#endif
