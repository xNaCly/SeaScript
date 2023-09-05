#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <stdint.h>

typedef struct {
  void *value;
  int8_t hasValue;
} SeaScriptOptional;

#endif
