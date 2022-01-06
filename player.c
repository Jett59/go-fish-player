#include <stdio.h>

#include "regexp.h"

int main() {
  Regexp *regexp = regexpCompile("Hello |S| there! |N?| YAY");
  regexpPrint(regexp, stdout);
  return 0;
}
