#include "regexp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Regexp *regexpCompile(const char *str) {
  const char *temp = str;
  RegexpComponent *first = calloc(1, sizeof(RegexpComponent));
  RegexpComponent *next = first;
  do {
    if (*temp == '|') {
      switch (*++temp) {
      case 'S': {
        next->type = REGEXP_WILDCARD;
        break;
      }
      case 'N': {
        next->type = REGEXP_NUMBER;
        break;
      }
      default:
        fprintf(stderr, "Unknown regexp specifier %c\n", *temp);
        next->type = REGEXP_WILDCARD;
        break;
      }
      if (*++temp == '?') {
        next->optional = true;
        temp++;
      } else {
        next->optional = true;
      }
      if (*(temp++) != '|') {
        fprintf(stderr, "Missing terminator on expression\n");
      }
    } else {
      const char *stringEnd = strchr(temp, '|');
      int stringLength;
      if (stringEnd == 0) {
        stringLength = strlen(temp);
      } else {
        stringLength = stringEnd - temp;
      }
      const char *string = strndup(temp, stringLength);
      next->string = string;
      temp = stringEnd;
    }
    if (temp != 0 && *temp != 0) {
      RegexpComponent *newNext = calloc(1, sizeof(RegexpComponent));
      next->next = newNext;
      newNext->previous = next;
      next = newNext;
    } else {
      break;
    }
  } while (true);
  return first;
}