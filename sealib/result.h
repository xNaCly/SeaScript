/*
The result.h component exposes utilites for a sophisticated error handling
approach, inspired by rust results. It allows for defining functions that return
CsResult instead of special values of their type that indicate failure,
such as -1 for numeric operations or an empty string for string operations.

Guide for using this sealib component:

1. Define a function that returns a CsResult pointer:

  CsResult *squareRoot(double n) {
    if (n < 0) {
      return CsError(
          "can't compute square root of negative integer");
    }
    double *r = malloc(sizeof(double))
    *r = sqrt(n);
    if (r < 0) {
      return CsError("failed to compute square root");
    }
    return CsSuccess(r);
  }

(remember to allocate the value you want to escape to the heap and free it
afterwards)

2. Call this function

  CsResult* res = squareRoot(-5)

3. Use one of the following functions to extract the value of the
Result and thereby consume it:

  double res = *(double *) CsUnwrap(res)
  res = *(double *) CsExpect(res, "failed to compute sqrt")
  free(res)
 */
#ifndef RESULT_H
#define RESULT_H

#include <stdint.h>

typedef struct {
  // if the Result holds no error, this contains the success value
  void *value;
  // if the Result holds an error, this contains the error message
  const char *error;
  // this indicates if the Result holds an error
  int8_t hasError;
} CsResult;

// allocates a new Result, sets ->hasError to 0 and ->value to the given value
CsResult *CsSuccess(void *value);

// allocates a new Result, sets ->hasError to 1 and ->error to the given error
CsResult *CsError(const char *error);

// returns the value of the Result and destroys it. If the Result contains an
// error the error message is printed to stderr and the process exists with
// EXIT_FAILURE
void *CsUnwrap(CsResult *result);

// returns the value of the Result and destroys it. If the Result contains an
// error the provided error message is printed to stderr and the process exists
// with EXIT_FAILURE
void *CsExpect(CsResult *result, const char *error);

// frees the allocated memory region for the given Result, sets the
// pointer to point to NULL
void CsFreeResult(CsResult *result);

#endif
