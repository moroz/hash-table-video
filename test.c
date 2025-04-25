#include "./table.h"
#include <criterion/criterion.h>

Test(Table, initResizeFree) {
  Table t;
  initTable(&t);

  cr_assert_eq(t.entries, NULL);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 0);

  growTable(&t, 8);
  cr_assert_neq(t.entries, NULL);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 8);

  freeTable(&t);
  cr_assert_eq(t.entries, NULL);
  cr_assert_eq(t.count, 0);
  cr_assert_eq(t.capacity, 0);
}
