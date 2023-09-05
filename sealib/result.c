#include "result.h"
#include <stdio.h>
#include <stdlib.h>

SeaScriptResult *SeaScriptResultNewSuccess(void *value) {
  SeaScriptResult *result = NULL;
  result = malloc(sizeof(SeaScriptResult));
  result->value = value;
  result->hasError = 0;
  return result;
};

SeaScriptResult *SeaScriptResultNewError(const char *error) {
  SeaScriptResult *result = NULL;
  result = malloc(sizeof(SeaScriptResult));
  result->error = error;
  result->hasError = 1;
  return result;
};

void *SeaScriptResultExpect(SeaScriptResult *result, const char *error) {
  if (result == NULL)
    return NULL;
  if (result->hasError) {
    SeaScriptResultFree(result);
    fprintf(stderr, "err: %s\n", error);
    exit(EXIT_FAILURE);
  }
  void *value = result->value;
  SeaScriptResultFree(result);
  return value;
}

void *SeaScriptResultUnwrap(SeaScriptResult *result) {
  if (result == NULL)
    return NULL;
  if (result->hasError) {
    fprintf(stderr, "err: %s\n", result->error);
    SeaScriptResultFree(result);
    exit(EXIT_FAILURE);
  }
  void *value = result->value;
  SeaScriptResultFree(result);
  return value;
}

void SeaScriptResultFree(SeaScriptResult *result) {
  if (result == NULL) {
    return;
  }
  free(result);
  result = NULL;
};
