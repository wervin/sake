#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "sake/array.h"
#include "sake/vector.h"

bool predicate(const void *a, const void *b)
{
    return *(int32_t *)a == *(int32_t *)b;
}

int32_t comparator(const void *a, const void *b)
{
    return (*(int32_t *)a - *(int32_t *)b);
}

bool dynamic_predicate(const void *a, const void *b)
{
    return **(int32_t **)a == *(int32_t *)b;
}

int32_t dynamic_comparator(const void *a, const void *b)
{
    return (**(int32_t **)a - **(int32_t **)b);
}

START_TEST(check_vector_new_static)
{
    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);
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

    int32_t d = 40;
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
    int32_t **vec = sake_vector_new(sizeof(int32_t *), free);
    ck_assert_int_eq(sake_vector_size(vec), 0);
    ck_assert_int_eq(sake_vector_capacity(vec), 1);

    int32_t *a = malloc(sizeof(int32_t));
    *a = 10;
    vec = sake_vector_push_back(vec, &a);
    ck_assert_int_eq(*vec[0], *a);
    ck_assert_int_eq(sake_vector_size(vec), 1);
    ck_assert_int_eq(sake_vector_capacity(vec), 1);

    int32_t *b = malloc(sizeof(int32_t));
    *b = 20;
    vec = sake_vector_push_back(vec, &b);
    ck_assert_int_eq(*vec[1], *b);
    ck_assert_int_eq(sake_vector_size(vec), 2);
    ck_assert_int_eq(sake_vector_capacity(vec), 2);

    int32_t *c = malloc(sizeof(int32_t));
    *c = 30;
    vec = sake_vector_push_back(vec, &c);
    ck_assert_int_eq(*vec[2], *c);
    ck_assert_int_eq(sake_vector_size(vec), 3);
    ck_assert_int_eq(sake_vector_capacity(vec), 4);

    sake_vector_erase(vec, 1);
    ck_assert_int_eq(*vec[1], *c);
    ck_assert_int_eq(sake_vector_size(vec), 2);
    ck_assert_int_eq(sake_vector_capacity(vec), 4);

    int32_t *d = malloc(sizeof(int32_t));
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

START_TEST(check_vector_insert)
{
    int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};
    int32_t b[20] = {1, 54, 29, 1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249, 51, 1249, 2, 4, 4124, 9, 1249};

    int32_t *vec1 = sake_vector_new(sizeof(int32_t), NULL);

    for (int i = 0; i < 10; i++)
    {
        vec1 = sake_vector_push_back(vec1, &a[i]);
        ck_assert_int_eq(vec1[i], a[i]);
        ck_assert_int_eq(sake_vector_size(vec1), i + 1);
    }

    vec1 = sake_vector_insert_range(vec1, 3, a, sizeof(a) / sizeof(uint32_t));
    for (int i = 0; i < 20; i++)
    {
        ck_assert_int_eq(vec1[i], b[i]);
    }

    sake_vector_free(vec1);
}
END_TEST

START_TEST(check_vector_erase)
{
    int32_t b[20] = {1, 54, 29, 1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249, 51, 1249, 2, 4, 4124, 9, 1249};
    int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

    int32_t *vec1 = sake_vector_new(sizeof(int32_t), NULL);

    for (int i = 0; i < 20; i++)
    {
        vec1 = sake_vector_push_back(vec1, &b[i]);
        ck_assert_int_eq(vec1[i], b[i]);
        ck_assert_int_eq(sake_vector_size(vec1), i + 1);
    }

    sake_vector_erase_range(vec1, 3, 13);
    for (int i = 0; i < 10; i++)
    {
        ck_assert_int_eq(vec1[i], a[i]);
    }

    sake_vector_free(vec1);
}
END_TEST

START_TEST(check_vector_copy)
{
    int32_t a[10] = {1, 54, 29, 51, 1249, 2, 4, 4124, 9, 1249};

    int32_t *vec1 = sake_vector_new(sizeof(int32_t), NULL);
    int32_t *vec2 = sake_vector_new(sizeof(int32_t), NULL);

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

    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);

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

    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);

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

    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);

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

    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);

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

    int32_t *vec = sake_vector_new(sizeof(int32_t), NULL);

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

Suite *check_sake_suite(void)
{
    Suite *s;
    TCase *test;

    s = suite_create("CheckSakeVector");

    test = tcase_create("CheckVectorNewStatic");
    tcase_add_test(test, check_vector_new_static);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorNewDynamic");
    tcase_add_test(test, check_vector_new_dynamic);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorInsert");
    tcase_add_test(test, check_vector_insert);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorErase");
    tcase_add_test(test, check_vector_erase);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorCopy");
    tcase_add_test(test, check_vector_copy);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorHeapSort");
    tcase_add_test(test, check_vector_heapsort);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorQuickSort");
    tcase_add_test(test, check_vector_quicksort);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorBinarySearch");
    tcase_add_test(test, check_vector_binarysearch);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorFind");
    tcase_add_test(test, check_vector_find);
    suite_add_tcase(s, test);

    test = tcase_create("CheckVectorReverseFind");
    tcase_add_test(test, check_vector_rfind);
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
