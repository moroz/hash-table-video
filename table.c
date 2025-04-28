#include "./table.h"

#include <stdbool.h> // for bool
#include <stddef.h>  // for NULL
#include <stdint.h>  // for uint32_t, uint8_t
#include <stdlib.h>  // for free(), malloc()
#include <string.h>  // for memset(), strdup(), strlen()

void initTable(Table *table) {
  table->capacity = 0;
  table->count = 0;
  table->entries = NULL;
}

void freeTable(Table *table) {
  free(table->entries);
  initTable(table);
}

static bool compareKey(String *a, String *b) {
  return a->len == b->len && a->hash == b->hash &&
         strncmp(a->chars, b->chars, a->len) == 0;
}

static bool isTombstone(Entry *entry) {
  return entry->key == NULL && entry->value == TOMBSTONE_VALUE;
}

static Entry *findEntry(Entry *entries, const int capacity, String *key) {
  if (capacity == 0)
    return NULL;

  int index = key->hash & (capacity - 1);
  Entry *tombstone = NULL;

  for (;;) {
    Entry *entry = &entries[index];

    if (entry->key == NULL) {
      // actual empty bucket, return first tombstone if one was ever found
      if (entry->value == 0) {
        return tombstone == NULL ? entry : tombstone;
      }
      // we have encountered a tombstone
      if (tombstone == NULL) {
        tombstone = entry;
      }
    } else if (compareKey(entry->key, key)) {
      return entry;
    }

    index = (index + 1) & (capacity - 1);
  }
}

void growTable(Table *table, int capacity) {
  Entry *entries = malloc(capacity * sizeof(Entry));
  memset(entries, 0, capacity * sizeof(Entry));

  int count = 0;
  if (table->count > 0) {
    for (int i = 0; i < table->capacity; i++) {
      Entry *entry = &table->entries[i];

      if (entry->key == NULL) {
        continue;
      }

      Entry *dest = findEntry(entries, capacity, entry->key);
      dest->key = entry->key;
      dest->value = entry->value;
      count++;
    }
  }

  free(table->entries);
  table->entries = entries;
  table->capacity = capacity;
  table->count = count;
}

static uint32_t hashString(const char *chars) {
  uint32_t hash = 2166136261u;
  while (*chars) {
    hash = (hash ^ (uint8_t)chars[0]) * 16777619;
    chars++;
  }
  return hash;
}

String *copyString(const char *chars) {
  String *string = malloc(sizeof(String));
  string->hash = hashString(chars);
  string->len = strlen(chars);
  string->chars = strdup(chars);
  return string;
}

void freeString(String *string) {
  free(string->chars);
  free(string);
}

bool tableSet(Table *table, String *key, int value) {
  if (table->count + 1 > TABLE_MAX_LOAD * table->capacity) {
    growTable(table, table->capacity < 8 ? 8 : table->capacity * 2);
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);

  const bool isNewEntry = entry->key == NULL;

  if (isNewEntry && entry->value != TOMBSTONE_VALUE)
    table->count++;

  entry->key = key;
  entry->value = value;

  return isNewEntry;
}

bool tableGet(Table *table, String *key, int *value) {
  Entry *entry = findEntry(table->entries, table->capacity, key);
  if (entry->key == NULL) {
    return false;
  }

  *value = entry->value;
  return true;
}

bool tableDelete(Table *table, String *key) {
  Entry *entry = findEntry(table->entries, table->capacity, key);

  if (entry->key == NULL) {
    return false;
  }

  entry->key = NULL;
  entry->value = TOMBSTONE_VALUE;
  return true;
}
