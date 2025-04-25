#include "./table.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void initTable(Table *table) {
  table->capacity = 0;
  table->count = 0;
  table->entries = NULL;
}

void freeTable(Table *table) {
  free(table->entries);
  initTable(table);
}

void growTable(Table *table, int capacity) {
  Entry *entries = malloc(capacity * sizeof(Entry));

  int count = 0;
  // copy all entries from old table to new table
  for (int i = 0; i < table->capacity; i++) {
    Entry *entry = &table->entries[i];
    if (entry->key == NULL)
      continue;

    // TODO: copy entry to new table and increment count
    count++;
  }

  free(table->entries);
  table->entries = entries;
  table->count = count;
  table->capacity = capacity;
}

static uint32_t hashString(const char *chars, const size_t length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash = (hash ^ (uint8_t)chars[i]) * 16777619;
  }
  return hash;
}

String *copyString(const char *chars) {
  String *result = malloc(sizeof(String));
  result->chars = strdup(chars);
  result->len = strlen(chars);
  result->hash = hashString(chars, result->len);
  return result;
}

void freeString(String *string) {
  free(string->chars);
  free(string);
}

static bool compareKey(const String *a, const String *b) {
  return a->len == b->len && a->hash == b->hash &&
         strcmp(a->chars, b->chars) == 0;
}

static Entry *findEntry(Entry *entries, const int capacity, const String *key) {
  // Begin looking for an index from key hash % capacity
  // since capacity will always be a power of 2, hash % capacity == hash &
  // (capacity - 1)
  uint32_t index = key->hash & (capacity - 1);
  for (;;) {
    Entry *entry = &entries[index];
    if (entry->key == NULL || compareKey(entry->key, key)) {
      return entry;
    }

    index = (index + 1) & (capacity - 1);
  }
}

bool tableSet(Table *table, String *key, int value) {
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    const int newCapacity = table->capacity < 8 ? 8 : table->capacity * 2;
    growTable(table, newCapacity);
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);
  const bool isNewKey = entry->key == NULL;
  if (isNewKey) {
    table->count++;
  }

  entry->key = key;
  entry->value = value;
  return isNewKey;
}
