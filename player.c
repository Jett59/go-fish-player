#include <stdio.h>

#include "regexp.h"

#define MAXLINE 512

void myMatchFunction(void *data, int i, void *context) {
  printf("Match function got called for i=%d\n", i);
}

int main() {
  Regexp *regexp = regexpCompile("|W| |N?|");
  regexpPrint(regexp, stdout);
  printf("Type a line to match it: ");
  char line[MAXLINE];
  fgets(line, MAXLINE, stdin);
  if (regexpMatch(line, regexp, myMatchFunction, 0)) {
    printf("Matched\n");
  } else {
    printf("Didn't match\n");
  }
  return 0;
}
