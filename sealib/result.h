/*
The result.h component exposes utilites for a sophisticated error handling
approach, inspired by rust. It allows for definiting functions that return
Results instead of values of their type that indicate failure, such as -1
for numeric operations or an empty string for string operations.

Guide for using this sealib component:

1. Define a function that returns a SeaScriptResult pointer:

  SeaScriptResult *squareRoot(double n) {
    if (n < 0) {
      return SeaScriptResultNewError(
          "can't compute square root of negative integer");
    }
    double r = sqrt(n);
    if (r < 0) {
      return SeaScriptResultNewError("failed to compute square root");
    }
    return SeaScriptResultNewSuccess((void *)&r);
  }

2. Call this function

  SeaScriptResult* res = squareRoot(-5)

3. Use one of the following functions to extract the value of the
SeaScriptResult and thereby consume it:

  *(double *) SeaScriptResultUnwrap(res)
  *(double *) SeaScriptResultExpect(res, "failed to compute sqrt")
 */
#ifndef RESULT_H
#define RESULT_H

typedef struct SeaScriptResult {
  // if the Result holds no error, this contains the success value
  void *value;
  // if the Result holds an error, this contains the error message
  const char *error;
  // this indicates if the Result holds an error
  int hasError;
} SeaScriptResult;

// allocates a new Result, sets ->hasError to 0 and ->value to the given value
SeaScriptResult *SeaScriptResultNewSuccess(void *value);

// allocates a new Result, sets ->hasError to 1 and ->error to the given error
SeaScriptResult *SeaScriptResultNewError(const char *error);

// returns the value of the Result and destroys it. If the Result contains an
// error the error message is printed to stderr and the process exists with
// EXIT_FAILURE
void *SeaScriptResultUnwrap(SeaScriptResult *result);

// returns the value of the Result and destroys it. If the Result contains an
// error the provided error message is printed to stderr and the process exists
// with EXIT_FAILURE
void *SeaScriptResultExpect(SeaScriptResult *result, const char *error);

// frees the allocated memory region for the given Result, sets the
// pointer to point to NULL
void SeaScriptResultFree(SeaScriptResult *result);

#endif
