/*
The slice.h component contains a abstract datatype inspired by golangs
implementation of dynamically growing arrays. It makes heavy use of SeaScriptResult for return
types, takes ownership of inserted elements and grows the underlying array.

Slices are intended to replace list.h
*/
#ifndef SLICE_H
#define SLICE_H

#include <stdint.h>

typedef struct {
    void** elements; // list of elements
    int64_t len;
    int64_t cap;
} SeaScriptSlice;

#endif
