#ifndef TABLE_H
#define TABLE_H

#define TABLE_MAX_LOAD 0.75
#define TOMBSTONE_VALUE (-1)

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char *chars;
  uint32_t hash;
  int len;
} String;

typedef struct {
  String *key;
  int64_t value;
} Entry;

typedef struct {
  int capacity;
  int count;
  Entry *entries;
} Table;

void initTable(Table *table);
void freeTable(Table *table);
void growTable(Table *table, int capacity);

String *copyString(const char *chars);
void freeString(String *string);

bool tableSet(Table *table, String *key, int value);
bool tableGet(Table *table, String *key, int *value);
bool tableDelete(Table *table, String *key);

#endif
