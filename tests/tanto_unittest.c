//
// Created by alireza on 5/26/20.
//

#include "../tanto.h"
#include "./acutest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

TJSON_t* parse_sample_json()
{
  char *stream;
  stream = tanto_read_file("tests/sample.json");
  TJSON_t *json = NULL;
  TANTO_INIT(&json);

  tanto_parse(&json, stream);
  return json;
}

char* find_value(char *key)
{
  TJSON_t *json = parse_sample_json();

  char *fin =  tanto_find(json, key)->value;
  return fin;
}

TJSON_t* get_object(char *key)
{
  TJSON_t *json = parse_sample_json();
  TJSON_t *fin =  tanto_find(json, key);

  if (fin != NULL)
  {
    return fin->child;
  }

  return fin;
}

void tests(void)
{
  TEST_CHECK_(strcmp(find_value("root"), "mine") == 0, "find_value(\"root\")");
  TEST_CHECK_(strcmp(find_value("parser"), "tanto") == 0, "find_value(\"parser\")");
  TEST_CHECK_(strcmp(find_value("name"), "Awesome 4K") == 0, "find_value(\"name\")");
  TEST_CHECK_(strcmp(find_value("private"), "true") == 0, "find_value(\"private\")");

  // TODO : handle multiple datatypes.
  TEST_CHECK_(strcmp(find_value("version"), "1") == 0, "find_value(\"version\")");

  // Check objects
  TEST_CHECK_(get_object("versions") != NULL , "get_object(\"versions\")");

  // TODO: we need another method for arrays
  TEST_CHECK_(get_object("functions") != NULL , "get_object(\"functions\")");
}

TEST_LIST = {
  {"int tests();", tests},
  {0}
};
