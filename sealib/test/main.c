#include "../result.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void tlog(const char *str) { printf("test: %s\n", str); };

int main(void) {
  tlog("testing map...");
  testMap();
  return EXIT_SUCCESS;
}
