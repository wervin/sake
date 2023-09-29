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

START_TEST(check_array_heapsort)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t sorted[10] = {1, 2, 4, 9, 29, 51, 54, 1249, 1249, 4124};

  sake_array_heapsort(a, 10, sizeof(int32_t), comparator);

  for (int32_t i = 0; i < 10; i++)
  { 
    ck_assert_int_eq(a[i], sorted[i]);
  }
}
END_TEST

START_TEST(check_array_quicksort)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t sorted[10] = {1, 2, 4, 9, 29, 51, 54, 1249, 1249, 4124};

  sake_array_quicksort(a, 10, sizeof(int32_t), comparator);

  for (int32_t i = 0; i < 10; i++)
  { 
    ck_assert_int_eq(a[i], sorted[i]);
  }
}
END_TEST

START_TEST(check_array_binarysearch)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  sake_array_quicksort(a, 10, sizeof(int32_t), comparator);
  int32_t target = 2;
  int32_t idx = sake_array_binarysearch(a, 10, sizeof(int32_t), comparator, &target);
  ck_assert_int_eq(idx, 1);
}
END_TEST

START_TEST(check_array_find)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t target = 2;
  int32_t idx = sake_array_find(a, 10, sizeof(int32_t), predicate, &target);
  ck_assert_int_eq(idx, 5);
}
END_TEST

START_TEST(check_array_rfind)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t target = 2;
  int32_t idx = sake_array_rfind(a, 10, sizeof(int32_t), predicate, &target);
  ck_assert_int_eq(idx, 5);
}
END_TEST

START_TEST(check_vector_new_static)
{
  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);
  ck_assert_int_eq(sake_vector_size(vec), 0);
  ck_assert_int_eq(sake_vector_capacity(vec), 1);

  int32_t a = 10;
  vec = sake_vector_push_back(vec, &a);
  ck_assert_int_eq(vec[0], a);
  ck_assert_int_eq(sake_vector_size(vec), 1);
  ck_assert_int_eq(sake_vector_capacity(vec), 1);

  int32_t b = 20;
  vec = sake_vector_push_back(vec, &b);
  ck_assert_int_eq(vec[1], b);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 2);

  int32_t c = 30;
  vec = sake_vector_push_back(vec, &c);
  ck_assert_int_eq(vec[2], c);
  ck_assert_int_eq(sake_vector_size(vec), 3);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_erase(vec, 1);
  ck_assert_int_eq(vec[1], c);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  int32_t  d = 40;
  vec = sake_vector_insert(vec, 1, &d);
  ck_assert_int_eq(vec[1], d);
  ck_assert_int_eq(sake_vector_size(vec), 3);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_pop_back(vec);
  ck_assert_int_eq(vec[1], d);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_new_dynamic)
{
  int32_t ** vec = sake_vector_new(sizeof(int32_t *), free);
  ck_assert_int_eq(sake_vector_size(vec), 0);
  ck_assert_int_eq(sake_vector_capacity(vec), 1);

  int32_t * a = malloc(sizeof(int32_t));
  *a = 10;
  vec = sake_vector_push_back(vec, &a);
  ck_assert_int_eq(*vec[0], *a);
  ck_assert_int_eq(sake_vector_size(vec), 1);
  ck_assert_int_eq(sake_vector_capacity(vec), 1);

  int32_t * b = malloc(sizeof(int32_t));
  *b = 20;
  vec = sake_vector_push_back(vec, &b);
  ck_assert_int_eq(*vec[1], *b);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 2);

  int32_t * c = malloc(sizeof(int32_t));
  *c = 30;
  vec = sake_vector_push_back(vec, &c);
  ck_assert_int_eq(*vec[2], *c);
  ck_assert_int_eq(sake_vector_size(vec), 3);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_erase(vec, 1);
  ck_assert_int_eq(*vec[1], *c);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  int32_t * d = malloc(sizeof(int32_t));
  *d = 40;
  vec = sake_vector_insert(vec, 1, &d);
  ck_assert_int_eq(*vec[1], *d);
  ck_assert_int_eq(sake_vector_size(vec), 3);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_pop_back(vec);
  ck_assert_int_eq(*vec[1], *d);
  ck_assert_int_eq(sake_vector_size(vec), 2);
  ck_assert_int_eq(sake_vector_capacity(vec), 4);

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_copy)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

  int32_t * vec1 = sake_vector_new(sizeof(int32_t), NULL);
  int32_t * vec2 = sake_vector_new(sizeof(int32_t), NULL);

  for (int i = 0; i < 10; i++)
  {
    vec1 = sake_vector_push_back(vec1, &a[i]);
    ck_assert_int_eq(vec1[i], a[i]);
    ck_assert_int_eq(sake_vector_size(vec1), i + 1);
  }

  vec2 = sake_vector_copy(vec1, vec2);
  for (int i = 0; i < 10; i++)
  {
    ck_assert_int_eq(vec2[i], a[i]);
  }

  ck_assert_int_eq(sake_vector_size(vec1), sake_vector_size(vec2));
  ck_assert_int_eq(sake_vector_capacity(vec1), sake_vector_capacity(vec2));
  
  sake_vector_free(vec1);
  sake_vector_free(vec2);
}
END_TEST


START_TEST(check_vector_heapsort)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t sorted[10] = {1, 2, 4, 9, 29, 51, 54, 1249, 1249, 4124};

  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);

  for (int32_t i = 0; i < 10; i++)
  {
    vec = sake_vector_push_back(vec, &a[i]);
  }

  sake_vector_heapsort(vec, comparator);

  for (int32_t i = 0; i < 10; i++)
  { 
    ck_assert_int_eq(vec[i], sorted[i]);
  }

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_quicksort)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
  int32_t sorted[10] = {1, 2, 4, 9, 29, 51, 54, 1249, 1249, 4124};

  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);

  for (int32_t i = 0; i < 10; i++)
  {
    vec = sake_vector_push_back(vec, &a[i]);
  }

  sake_vector_quicksort(vec, comparator);

  for (int32_t i = 0; i < 10; i++)
  { 
    ck_assert_int_eq(vec[i], sorted[i]);
  }

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_binarysearch)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);

  for (int i = 0; i < 10; i++)
  {
    vec = sake_vector_push_back(vec, &a[i]);
  }

  sake_vector_quicksort(vec, comparator);

  int32_t target = 2;
  int32_t idx = sake_vector_binarysearch(vec, comparator, &target);
  ck_assert_int_eq(idx, 1);

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_find)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);

  for (int i = 0; i < 10; i++)
  {
    vec = sake_vector_push_back(vec, &a[i]);
  }

  int32_t target = 2;
  int32_t idx = sake_vector_find(vec, predicate, &target);
  ck_assert_int_eq(idx, 5);

  sake_vector_free(vec);
}
END_TEST

START_TEST(check_vector_rfind)
{
  int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

  int32_t * vec = sake_vector_new(sizeof(int32_t), NULL);

  for (int i = 0; i < 10; i++)
  {
    vec = sake_vector_push_back(vec, &a[i]);
  }

  int32_t target = 2;
  int32_t idx = sake_vector_rfind(vec, predicate, &target);
  ck_assert_int_eq(idx, 5);

  sake_vector_free(vec);
}
END_TEST

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

START_TEST(check_string)
{
  {
    char test[] = "test\n";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    sake_string_free(s);
  }

  {
    char test[] = "azùjfafùzeîghez$aruewjwf,ùqsd,az$epojazenamflkabznkq:bfwxklfjzapeo";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    sake_string_free(s);
  }

  {
    char test[] = "eazepjazr^pu)_&)é_'&é)u&é'lazbn&éàu&)";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    sake_string_free(s);
  }
}
END_TEST

Suite *check_sake_suite(void)
{
  Suite * s;
  TCase * tc_core_array, * tc_core_vector, * tc_core_list, *tc_core_string;

  s = suite_create("CheckSakeSuite");
  
  tc_core_array = tcase_create("CheckArray");
  tcase_add_test(tc_core_array, check_array_heapsort);
  tcase_add_test(tc_core_array, check_array_quicksort);
  tcase_add_test(tc_core_array, check_array_binarysearch);
  tcase_add_test(tc_core_array, check_array_find);
  tcase_add_test(tc_core_array, check_array_rfind);

  tc_core_vector = tcase_create("CheckVector");
  tcase_add_test(tc_core_vector, check_vector_new_static);
  tcase_add_test(tc_core_vector, check_vector_new_dynamic);
  tcase_add_test(tc_core_vector, check_vector_copy);
  tcase_add_test(tc_core_array, check_vector_heapsort);
  tcase_add_test(tc_core_array, check_vector_quicksort);
  tcase_add_test(tc_core_array, check_vector_binarysearch);
  tcase_add_test(tc_core_array, check_vector_find);
  tcase_add_test(tc_core_array, check_vector_rfind);
  
  tc_core_list = tcase_create("CheckList");
  tcase_add_test(tc_core_list, check_list_new_static);
  tcase_add_test(tc_core_list, check_list_new_dynamic);

  tc_core_string = tcase_create("CheckString");
  tcase_add_test(tc_core_string, check_string);

  suite_add_tcase(s, tc_core_array);
  suite_add_tcase(s, tc_core_vector);
  suite_add_tcase(s, tc_core_list);
  suite_add_tcase(s, tc_core_string);

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
