#include "result.h"
#include <stdio.h>
#include <stdlib.h>

CsResult *CsSuccess(void *value) {
  CsResult *result = NULL;
  if (result = malloc(sizeof(CsResult)), result == NULL) {
    fprintf(stderr, "sealib: failed to allocate result for success\n");
    exit(EXIT_FAILURE);
  };
  result->value = value;
  result->hasError = 0;
  return result;
};

CsResult *CsError(const char *error) {
  CsResult *result = NULL;
  if (result = malloc(sizeof(CsResult)), result == NULL) {
    fprintf(stderr, "sealib: failed to allocate result for error\n");
    exit(EXIT_FAILURE);
  };
  result->error = error;
  result->hasError = 1;
  return result;
};

void *CsExpect(CsResult *result, const char *error) {
  if (result == NULL)
    return NULL;
  if (result->hasError) {
    CsFreeResult(result);
    fprintf(stderr, "sealib: %s\n", error);
    exit(EXIT_FAILURE);
  }
  void *value = result->value;
  CsFreeResult(result);
  return value;
}

void *CsUnwrap(CsResult *result) {
  if (result == NULL)
    return NULL;
  if (result->hasError) {
    fprintf(stderr, "sealib: %s\n", result->error);
    CsFreeResult(result);
    exit(EXIT_FAILURE);
  }
  void *value = result->value;
  CsFreeResult(result);
  return value;
}

void CsFreeResult(CsResult *result) {
  if (result == NULL) {
    return;
  }
  free(result);
  result = NULL;
};
