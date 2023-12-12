#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "sake/string.h"

START_TEST(check_string_new)
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

START_TEST(check_string_size)
{
  {
    char test[] = "test\n";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 5);
    ck_assert_uint_eq(sake_string_utf8_size(s), 5);
    sake_string_free(s);
  }

  {
    char test[] = "éé";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 4);
    ck_assert_uint_eq(sake_string_utf8_size(s), 2);
    sake_string_free(s);
  }

  {
    char test[] = "邹邹";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 6);
    ck_assert_uint_eq(sake_string_utf8_size(s), 2);
    sake_string_free(s);
  }

  {
    char test[] = "𑜴𑜴";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 8);
    ck_assert_uint_eq(sake_string_utf8_size(s), 2);
    sake_string_free(s);
  }

  {
    char test[] = "ⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄ";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 116);
    ck_assert_uint_eq(sake_string_utf8_size(s), 64);
    sake_string_free(s);
  }

  {
    char test[] = "";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 0);
    ck_assert_uint_eq(sake_string_utf8_size(s), 0);
    sake_string_free(s);
  }
}
END_TEST

START_TEST(check_string_at)
{
  {
    char test[] = "test\né邹𑜴é";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 9);
    uint32_t utf8 = sake_string_at(s, 0);
    ck_assert_str_eq((char *)&utf8, "t");
    utf8 = sake_string_at(s, 1);
    ck_assert_str_eq((char *)&utf8, "e");
    utf8 = sake_string_at(s, 2);
    ck_assert_str_eq((char *)&utf8, "s");
    utf8 = sake_string_at(s, 3);
    ck_assert_str_eq((char *)&utf8, "t");
    utf8 = sake_string_at(s, 4);
    ck_assert_str_eq((char *)&utf8, "\n");
    utf8 = sake_string_at(s, 5);
    ck_assert_str_eq((char *)&utf8, "é");
    utf8 = sake_string_at(s, 6);
    ck_assert_str_eq((char *)&utf8, "邹");
    uint64_t utf8_ = sake_string_at(s, 7);
    ck_assert_str_eq((char *)&utf8_, "𑜴");
    utf8 = sake_string_at(s, 8);
    ck_assert_str_eq((char *) &utf8, "é");

    sake_string_free(s);
  }
}
END_TEST

START_TEST(check_string_push_back)
{
  {
    char test[] = "test\n";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 5);
    ck_assert_uint_eq(sake_string_utf8_size(s), 5);

    s = sake_string_push_back(s, "q");
    ck_assert_str_eq(s, "test\nq");
    ck_assert_uint_eq(sake_string_raw_size(s), 6);
    ck_assert_uint_eq(sake_string_utf8_size(s), 6);

    s = sake_string_push_back(s, "퉡");
    ck_assert_str_eq(s, "test\nq퉡");
    ck_assert_uint_eq(sake_string_raw_size(s), 9);
    ck_assert_uint_eq(sake_string_utf8_size(s), 7);

    s = sake_string_push_back(s, "ⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄ");
    ck_assert_str_eq(s, "test\nq퉡ⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄⶃڪ]'/͵6٦і⻢۹뛱%=퉡▟qA G¦ZV̛͌Ĩ݄ާ̟썋:Ⱄ");
    ck_assert_uint_eq(sake_string_raw_size(s), 125);
    ck_assert_uint_eq(sake_string_utf8_size(s), 71);
    
    sake_string_free(s);
  }
}
END_TEST


START_TEST(check_string_pop_back)
{
  {
    char test[] = "test\né퉡𑜴0a";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test\né퉡𑜴0");
    ck_assert_uint_eq(sake_string_raw_size(s), 15);
    ck_assert_uint_eq(sake_string_utf8_size(s), 9);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test\né퉡𑜴");
    ck_assert_uint_eq(sake_string_raw_size(s), 14);
    ck_assert_uint_eq(sake_string_utf8_size(s), 8);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test\né퉡");
    ck_assert_uint_eq(sake_string_raw_size(s), 10);
    ck_assert_uint_eq(sake_string_utf8_size(s), 7);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test\né");
    ck_assert_uint_eq(sake_string_raw_size(s), 7);
    ck_assert_uint_eq(sake_string_utf8_size(s), 6);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test\n");
    ck_assert_uint_eq(sake_string_raw_size(s), 5);
    ck_assert_uint_eq(sake_string_utf8_size(s), 5);

    sake_string_pop_back(s);
    ck_assert_str_eq(s, "test");
    ck_assert_uint_eq(sake_string_raw_size(s), 4);
    ck_assert_uint_eq(sake_string_utf8_size(s), 4);
    
    sake_string_free(s);
  }
}
END_TEST

START_TEST(check_string_erase)
{
  {
    char test[] = "test\né퉡𑜴0a";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "testé퉡𑜴0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 15);
    ck_assert_uint_eq(sake_string_utf8_size(s), 9);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "test퉡𑜴0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 13);
    ck_assert_uint_eq(sake_string_utf8_size(s), 8);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "test𑜴0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 10);
    ck_assert_uint_eq(sake_string_utf8_size(s), 7);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "test0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 6);
    ck_assert_uint_eq(sake_string_utf8_size(s), 6);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "testa");
    ck_assert_uint_eq(sake_string_raw_size(s), 5);
    ck_assert_uint_eq(sake_string_utf8_size(s), 5);

    sake_string_erase(s, 4);
    ck_assert_str_eq(s, "test");
    ck_assert_uint_eq(sake_string_raw_size(s), 4);
    ck_assert_uint_eq(sake_string_utf8_size(s), 4);

    sake_string_free(s);
  }
}
END_TEST

START_TEST(check_string_erase_range)
{
  {
    char test[] = "test\né퉡𑜴0a";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_erase_range(s, 4, 8);
    ck_assert_str_eq(s, "test0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 6);
    ck_assert_uint_eq(sake_string_utf8_size(s), 6);

    sake_string_free(s);
  }

  {
    char test[] = "test\né퉡𑜴0a";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_erase_range(s, 1, 9);
    ck_assert_str_eq(s, "ta");
    ck_assert_uint_eq(sake_string_raw_size(s), 2);
    ck_assert_uint_eq(sake_string_utf8_size(s), 2);

    sake_string_free(s);
  }

  {
    char test[] = "test\né퉡𑜴0a";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_erase_range(s, 0, 5);
    ck_assert_str_eq(s, "é퉡𑜴0a");
    ck_assert_uint_eq(sake_string_raw_size(s), 11);
    ck_assert_uint_eq(sake_string_utf8_size(s), 5);

    sake_string_free(s);
  }
}
END_TEST

START_TEST(check_string_insert)
{
  {
    char test[] = "test";
    sake_string s = sake_string_new(test);
    ck_assert_str_eq(s, test);
    ck_assert_uint_eq(sake_string_raw_size(s), 4);
    ck_assert_uint_eq(sake_string_utf8_size(s), 4);

    s = sake_string_insert(s, 1, "\né퉡𑜴0a");
    ck_assert_str_eq(s, "t\né퉡𑜴0aest");
    ck_assert_uint_eq(sake_string_raw_size(s), 16);
    ck_assert_uint_eq(sake_string_utf8_size(s), 10);

    sake_string_free(s);
  }
}
END_TEST

Suite *check_sake_suite(void)
{
  Suite * s;
  TCase * test;

  s = suite_create("CheckSakeString");
  
  test = tcase_create("CheckStringNew");
  tcase_add_test(test, check_string_new);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringSize");
  tcase_add_test(test, check_string_size);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringAt");
  tcase_add_test(test, check_string_at);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringPushBack");
  tcase_add_test(test, check_string_push_back);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringPopBack");
  tcase_add_test(test, check_string_pop_back);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringErase");
  tcase_add_test(test, check_string_erase);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringEraseRange");
  tcase_add_test(test, check_string_erase_range);
  suite_add_tcase(s, test);

  test = tcase_create("CheckStringInsert");
  tcase_add_test(test, check_string_insert);
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
