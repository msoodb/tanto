//
// Created by alireza on 5/26/20.
//

#include "../tanto.h"
#include "./acutest.h"

int add(int a, int b)
{
  return a + b;
}

void test_add(void)
{
  TEST_CHECK_(add(2, 2) == (2 + 2), "add(%d, %d)==%d", 2, 2, (2+2));
}

TEST_LIST = {
  {"int add(int, int);", test_add},
  {0}
};
