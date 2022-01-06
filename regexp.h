#ifndef _REGEXP_H
#define _REGEXP_H

#include <stdbool.h>

#include <stdio.h>

typedef enum {
  REGEXP_STRING,
  REGEXP_WILDCARD,
  REGEXP_NUMBER
} RegexpComponentType;

typedef struct RegexpComponent {
  RegexpComponentType type;
  const char *string;
  bool optional;
  struct RegexpComponent *previous, *next;
} RegexpComponent;

typedef RegexpComponent Regexp;

typedef void (*RegexpMatchFunction)(void *data, int i, void *context);

bool regexpMatch(const char *str, Regexp *regexp,
                 RegexpMatchFunction matchFunction, void *context);

Regexp *regexpCompile(const char *str);

void regexpPrint(Regexp *regexp, FILE *out);

void regexpFree(Regexp *regexp);

#endif