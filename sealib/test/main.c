#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void tlog(const char *str) { printf("test: %s\n", str); };

int main(void) {
  tlog("testing lists...");
  testList();
  tlog("testing results...");
  // TODO
  return EXIT_SUCCESS;
}
