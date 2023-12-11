#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "sake/array.h"
#include "sake/vector.h"
#include "sake/list.h"
#include "sake/string.h"

bool predicate(const void * a, const void * b) {
  return *(int32_t *)a == *(int32_t *)b;
}

int32_t comparator(const void * a, const void * b) {
  return ( *(int32_t *)a - *(int32_t *)b );
}

bool dynamic_predicate(const void * a, const void * b) {
  return **(int32_t **)a == *(int32_t *)b;
}

int32_t dynamic_comparator(const void * a, const void * b) {
  return ( **(int32_t **)a - **(int32_t **)b );
}

START_TEST(check_list_new_static)
{
  int32_t res1[5] = {17, 20, 30, 31, 40};
  int32_t res2[5] = {40, 31, 30, 20, 17};
  int32_t res3[3] = {20, 100, 31};

  struct sake_list l;
  int32_t * cur;
  int32_t i;

  sake_list_new(&l, sizeof(int32_t), NULL);

  int32_t a = 30;
  sake_list_insert_tail(&l, &a);
  int32_t b = 20;
  sake_list_insert_tail(&l, &b);
  int32_t c = 40;
  sake_list_insert_tail(&l, &c);

  sake_list_mergesort(&l, comparator);

  int32_t e = 17;
  sake_list_insert_sorted(&l, &e, comparator);
  int32_t f = 31;
  sake_list_insert_sorted(&l, &f, comparator);

  i = 0;
  cur = sake_list_head(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(*cur, res1[i]);
    i++;
    cur = sake_list_next(cur);
  }

  i = 0;
  cur = sake_list_tail(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(*cur, res2[i]);
    i++;
    cur = sake_list_prev(cur);
  }

  int target = 30;
  cur = sake_list_find(&l, predicate, &target);
  ck_assert_int_eq(*cur, 30);

  int32_t d = 100;
  sake_list_insert_prev(&l, cur, &d);

  sake_list_remove(&l, cur);
  sake_list_remove_head(&l);
  sake_list_remove_tail(&l);

  i = 0;
  cur = sake_list_head(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(*cur, res3[i]);
    i++;
    cur = sake_list_next(cur);
  }

  sake_list_free(&l);
}
END_TEST

START_TEST(check_list_new_dynamic)
{
  int32_t res1[5] = {17, 20, 30, 31, 40};
  int32_t res2[5] = {40, 31, 30, 20, 17};
  int32_t res3[3] = {20, 100, 31};

  struct sake_list l;
  int32_t ** cur;
  int32_t i;

  sake_list_new(&l, sizeof(int32_t *), free);

  int32_t *a = malloc(sizeof(int32_t));
  *a = 30;
  sake_list_insert_tail(&l, &a);
  int32_t *b = malloc(sizeof(int32_t));
  *b = 20;
  sake_list_insert_tail(&l, &b);
  int32_t *c = malloc(sizeof(int32_t));
  *c = 40;
  sake_list_insert_tail(&l, &c);

  sake_list_mergesort(&l, dynamic_comparator);

  int32_t *e = malloc(sizeof(int32_t));
  *e = 17;
  sake_list_insert_sorted(&l, &e, dynamic_comparator);
  int32_t *f = malloc(sizeof(int32_t));
  *f = 31;
  sake_list_insert_sorted(&l, &f, dynamic_comparator);

  i = 0;
  cur = sake_list_head(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(**cur, res1[i]);
    i++;
    cur = sake_list_next(cur);
  }

  i = 0;
  cur = sake_list_tail(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(**cur, res2[i]);
    i++;
    cur = sake_list_prev(cur);
  }

  int target = 30;
  cur = sake_list_find(&l, dynamic_predicate, &target);
  ck_assert_int_eq(**cur, 30);

  int32_t *d = malloc(sizeof(int32_t));
  *d = 100;
  sake_list_insert_prev(&l, cur, &d);

  sake_list_remove(&l, cur);
  sake_list_remove_head(&l);
  sake_list_remove_tail(&l);

  i = 0;
  cur = sake_list_head(&l);
  while (cur != NULL)
  {
    ck_assert_int_eq(**cur, res3[i]);
    i++;
    cur = sake_list_next(cur);
  }

  sake_list_free(&l);
}
END_TEST

Suite *check_sake_suite(void)
{
  Suite * s;
  TCase * test;

  s = suite_create("CheckSakeList");

  test = tcase_create("CheckListNewStatic");
  tcase_add_test(test, check_list_new_static);
  suite_add_tcase(s, test);

  test = tcase_create("CheckListNewDynamic");
  tcase_add_test(test, check_list_new_dynamic);
  suite_add_tcase(s, test);

  return s;
}

int32_t main(void)
{
  int32_t number_failed;
  Suite *s;
  SRunner *sr;

  s = check_sake_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
