#ifndef TABLE_H
#define TABLE_H

#define TABLE_MAX_LOAD 0.75

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char *chars;
  uint32_t hash;
  int len;
} String;

typedef struct {
  String *key;
  int value;
} Entry;

typedef struct {
  int capacity;
  int count;
  Entry *entries;
} Table;

void initTable(Table *table);
void freeTable(Table *table);
void growTable(Table *table, int capacity);

#endif
