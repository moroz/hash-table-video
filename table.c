#include "./table.h"

#include <stddef.h> // for NULL
#include <stdlib.h> // for free(), malloc()
#include <string.h> // for memset()

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
  memset(entries, 0, capacity * sizeof(Entry));

  // TODO: copy and count entries from the old table
  // to the new one

  free(table->entries);
  table->entries = entries;
  table->capacity = capacity;
  table->count = 0;
}
