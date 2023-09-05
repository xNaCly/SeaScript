#include "list.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

SeaScriptList *SeaScriptListNew() {
  SeaScriptList *l = NULL;
  l = malloc(sizeof(SeaScriptList));
  l->first = NULL;
  l->last = NULL;
  l->size = 0;
  return l;
}

// recursively frees nodes and sets them to NULL
static void freeNode(SeaScriptListNode *node) {
  if (node == NULL)
    return;
  if (node->next != NULL) {
    freeNode(node->next);
  }
  free(node);
  node = NULL;
}

static SeaScriptListNode *nodeLookup(SeaScriptListNode *n, int counter) {
  if (n == NULL) {
    return NULL;
  }
  if (counter != 0) {
    if (n->next == NULL) {
      return NULL;
    }
    return nodeLookup(n->next, counter - 1);
  }
  return n;
}

SeaScriptResult *SeaScriptListGet(SeaScriptList *list, int index) {
  if (index < 0 || index > list->size) {
    return SeaScriptResultNewError("list index out of bounds");
  }

  SeaScriptListNode *found = NULL;

  // 0 and last index as extra cases for constant complexity access
  if (index == 0) {
    found = list->first;
  } else if (index == list->size) {
    found = list->last;
  } else {
    found = nodeLookup(list->first, index);
  }

  if (found == NULL) {
    return SeaScriptResultNewError("list element at index not found");
  }

  return SeaScriptResultNewSuccess(found->value);
}

SeaScriptResult *SeaScriptListPop(SeaScriptList *list) {
  if (list->first == NULL)
    return SeaScriptResultNewError("list does not contain any elements");

  SeaScriptListNode *cur = list->first;
  for (int i = 0; i < list->size; i++) {
    if (i == list->size - 1)
      break;
    cur = cur->next;
  }
  if (cur == NULL) {
    return SeaScriptResultNewError("iterated too far");
  }
  SeaScriptResult *r = SeaScriptResultNewSuccess(cur->value);
  list->last = cur;
  list->size--;
  return r;
}

void SeaScriptListAppend(SeaScriptList *list, void *element) {
  SeaScriptListNode *n = NULL;
  n = malloc(sizeof(SeaScriptListNode));
  n->value = element;
  n->next = NULL;

  if (list->first == NULL) {
    list->first = n;
    list->last = n;
  }

  list->last->next = n;
  list->last = n;
  list->size++;
}

void SeaScriptListFree(SeaScriptList *list) {
  if (list == NULL)
    return;
  if (list->size == 0 || list->first == NULL)
    return;
  freeNode(list->first);
  free(list);
  list = NULL;
}
