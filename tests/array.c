#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "sake/array.h"

bool predicate(const void * a, const void * b) {
  return *(int32_t *)a == *(int32_t *)b;
}

int32_t comparator(const void * a, const void * b) {
  return ( *(int32_t *)a - *(int32_t *)b );
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

Suite *check_sake_suite(void)
{
  Suite * s;
  TCase * test;

  s = suite_create("CheckSakeArray");
  
  test = tcase_create("CheckArrayHeapSort");
  tcase_add_test(test, check_array_heapsort);
  suite_add_tcase(s, test);

  test = tcase_create("CheckArrayQuickSort");
  tcase_add_test(test, check_array_quicksort);
  suite_add_tcase(s, test);

  test = tcase_create("CheckArrayBinarySearch");
  tcase_add_test(test, check_array_binarysearch);
  suite_add_tcase(s, test);

  test = tcase_create("CheckArrayFind");
  tcase_add_test(test, check_array_find);
  suite_add_tcase(s, test);

  test = tcase_create("CheckArrayReverseFind");
  tcase_add_test(test, check_array_rfind);
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
