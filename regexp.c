#include "regexp.h"

#include <ctype.h>
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
      case 'W': {
        next->type = REGEXP_WORD;
        break;
      }
      case 'N': {
        next->type = REGEXP_NUMBER;
        break;
      }
      default:
        fprintf(stderr, "Unknown regexp specifier %c\n", *temp);
        next->type = REGEXP_WORD;
        break;
      }
      if (*++temp == '?') {
        next->optional = true;
        temp++;
      } else {
        next->optional = false;
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

void regexpPrint(Regexp *regexp, FILE *out) {
  while (regexp != 0) {
    switch (regexp->type) {
    case REGEXP_STRING: {
      fprintf(out, "\"%s\"", regexp->string);
      break;
    }
    case REGEXP_WORD: {
      fprintf(out, "word");
      break;
    }
    case REGEXP_NUMBER: {
      fprintf(out, "number");
      break;
    }
    default:
      fprintf(out, "unknown: %d", regexp->type);
      break;
    }
    if (regexp->optional) {
      fprintf(out, " (optional)");
    }
    putc('\n', out);
    regexp = regexp->next;
  }
}

bool regexpMatch(const char *str, Regexp *regexp,
                 RegexpMatchFunction matchFunction, void *context) {
  const char *temp = str;
  int i = 0;
  while (regexp != 0) {
    switch (regexp->type) {
    case REGEXP_NUMBER: {
      const char *endStr = 0;
      long value = strtol(temp, (char**)&endStr, 10);
      if (endStr == temp) {
        if (!regexp->optional) {
          return false;
        }
      } else {
        matchFunction(&value, i, context);
      }
      temp = endStr;
      break;
    }
    case REGEXP_WORD: {
      // Get size of the word
      int len;
      for (len = 0; !isspace(temp[len]); len++)
        ;
      if (len == 0) {
        if (!regexp->optional) {
          return false;
        }
      } else {
        char word[len + 1];
        strncpy(word, temp, len);
        matchFunction(word, i, context);
      }
      temp += len;
      break;
    }
    case REGEXP_STRING: {
      if (strncmp(temp, regexp->string, strlen(regexp->string)) != 0) {
        return false;
      } else {
        temp += strlen(regexp->string);
        regexp = regexp->next;
        continue;  // Don't increment i
      }
    }
    }
    i++;
    regexp = regexp->next;
  }
  return true;
}
