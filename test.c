#include <criterion/criterion.h>
#include <stdio.h>

#include "./table.h"

Test(Table, initResizeFree) {
  Table t;
  initTable(&t);

  cr_assert_eq(t.entries, NULL);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 0);

  growTable(&t, 8);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 8);
  cr_assert_neq(t.entries, NULL);

  freeTable(&t);
  cr_assert_eq(t.entries, NULL);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 0);
}

Test(Table, tableSetGet) {
  Table t;
  initTable(&t);

  String *key = copyString("foo");
  tableSet(&t, key, 42);

  cr_assert_eq(t.count, 1);
  cr_assert_eq(t.capacity, 8);
  cr_assert_neq(t.entries, NULL);

  int actual = -1;
  cr_assert(tableGet(&t, key, &actual));
  cr_assert_eq(actual, 42);
}

typedef struct {
  char *key;
  int value;
} testEntry;

static const testEntry ENTRIES[] = {{"foo", 42}, {"bar", 69}, {"baz", 420}};

Test(Table, deleteEntry) {
  Table t;
  initTable(&t);

  for (int i = 0, n = sizeof(ENTRIES) / sizeof(testEntry); i < n; i++) {
    const testEntry *entry = &ENTRIES[i];
    String *key = copyString(entry->key);
    tableSet(&t, key, entry->value);
  }

  cr_assert_eq(t.count, 3);

  String *key = copyString("foo");
  int actual = -1;
  cr_assert(tableGet(&t, key, &actual));
  cr_assert_eq(actual, 42);

  cr_assert(tableDelete(&t, key));

  actual = -1;
  cr_assert_not(tableGet(&t, key, &actual));
  cr_assert_eq(actual, -1);
}

static const testEntry LOTTA_ENTRIES[] = {
    {"foo", 42},   {"bar", 69},  {"baz", 420},  {"bepis", 1},
    {"booty", 69}, {"BEPIS", 2}, {"FOOBAR", 3}, {"bEpIs", 4}};

Test(Table, growAndGet) {
  Table t;
  initTable(&t);

  for (int i = 0, n = sizeof(LOTTA_ENTRIES) / sizeof(testEntry); i < n; i++) {
    const testEntry *entry = &LOTTA_ENTRIES[i];
    String *key = copyString(entry->key);
    tableSet(&t, key, entry->value);
  }

  cr_assert_eq(t.count, 8);
  cr_assert_eq(t.capacity, 16);

  String *key = copyString("baz");
  int actual = -1;
  cr_assert(tableGet(&t, key, &actual));
  cr_assert_eq(actual, 420);
}
