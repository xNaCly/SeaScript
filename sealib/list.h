/*
The list.h component exposes a rudimentary linked list commonly used for
dynamically sized arrays. It makes heavy use of SeaScriptResult for return
types.

Guide for using this sealib component:

1. Create a new linked list:

  SeaScriptList *list = SeaScriptListNew();

2. Insert elements:

  SeaScriptListAppend(list, 1);
  SeaScriptListAppend(list, 2);
  SeaScriptListAppend(list, 3);
  SeaScriptListAppend(list, 4);

3. Pop the last element from the list:

    SeaScriptResult *res = SeaScriptListPop(list);
    double *resDouble = (double *)SeaScriptResultUnwrap(res);

    // remember to free the resulting value after you're done using it
    free(resDouble)

4. Print the rest of the list:

    for(int i = 0; i < list->size; i++) {
        SeaScriptResult r = SeaScriptListGet(list, i)
        double *res = (double *)SeaScriptResultUnwrap(r);
        printf("%f\n", *res);
        free(res);
    }

5. Free all nodes and the list itself:

    SeaScriptListFree(list);
*/
#ifndef LIST_H
#define LIST_H
#include "result.h"
#include <stdint.h>

typedef struct SeaScriptListNode {
  void *value;
  struct SeaScriptListNode *next;
} SeaScriptListNode;

typedef struct {
  SeaScriptListNode *first;
  SeaScriptListNode *last;
  int64_t size;
} SeaScriptList;

// allocates a new list
SeaScriptList *SeaScriptListNew();

// appends element to the end of the list
void SeaScriptListAppend(SeaScriptList *list, void *element);

// removes the last element of the list and returns its value in a Result
SeaScriptResult *SeaScriptListPop(SeaScriptList *list);

// gets the element at the index, Result contains an error if index < 0 or
// index > SeaScriptList->size
SeaScriptResult *SeaScriptListGet(SeaScriptList *list, int index);

// destroys all nodes in the list, the list itself, points list to NULL
void SeaScriptListFree(SeaScriptList *list);

#endif
