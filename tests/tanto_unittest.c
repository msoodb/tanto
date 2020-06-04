//
// Created by alireza on 5/26/20.
//

#include "../tanto.h"
#include "./acutest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TJSON_EXIST_FILE_COUNT         1
#define TJSON_NONE_EXIST_FILE_COUNT    1

char *tjson_exist_files[] = {
	"tests/sample_01.json",
	"tests/sample_02.json"
};

char *tjson_none_exist_files[] = {
	"tests/sample_03.json"
};


TJSON_t* parse_sample_json()
{
	char *json_file = "sample_01.json";
	char *stream;

	stream = tjson_read_file(json_file);

	TJSON_t *json = NULL;
	TJSON_INIT(&json);

	tjson_parse(&json, stream);
	return json;
}

/*char* find_value(char *key)
{
	TJSON_t *json = parse_sample_json();

	char *fin = tanto_find(json, key)->v_string;
	return fin;
	}*/

/*TJSON_t* get_object(char *key)
{
	TJSON_t *json = parse_sample_json();
	TJSON_t *fin =  tanto_find(json, key);

	if (fin != NULL) return fin->child;
	
	return fin;
	}*/

void tests(void)
{
	/*TEST_CHECK_(strcmp(find_value("root"), "mine") == 0, "find_value(\"root\")");
	TEST_CHECK_(strcmp(find_value("parser"), "tanto") == 0, "find_value(\"parser\")");
	TEST_CHECK_(strcmp(find_value("name"), "Awesome 4K") == 0, "find_value(\"name\")");
	TEST_CHECK_(strcmp(find_value("private"), "true") == 0, "find_value(\"private\")");

	// TODO : handle multiple datatypes.
	TEST_CHECK_(strcmp(find_value("version"), "1") == 0, "find_value(\"version\")");

	// Check objects
	TEST_CHECK_(get_object("versions") != NULL , "get_object(\"versions\")");

	// TODO: we need another method for arrays
	TEST_CHECK_(get_object("functions") != NULL , "get_object(\"functions\")");

	*/
}

void tests_read(void)
{
	char *json_file;
	char *stream;

	json_file = NULL;
	stream = NULL;

	/* exist files */
	int i = 0;
	for (i = 0; i < TJSON_EXIST_FILE_COUNT; ++i) {
		json_file = tjson_exist_files[i];		
		stream = tjson_read_file(json_file);
		TEST_CHECK_((stream != NULL), "stream has been read");
	}

	/* none exist files */
	for (i = 0; i < TJSON_NONE_EXIST_FILE_COUNT; ++i) {
		json_file = tjson_none_exist_files[i];		
		stream = tjson_read_file(json_file);
		TEST_CHECK_((stream == NULL), "stream has NOT been read");
	}

}

void tests_parse(void)
{

	char *json_file;
	char *stream;
	TJSON_t *json;
	

	int result;

	json_file = NULL;
	stream = NULL;
	result = -1;

	
	/* exist files */
	int i = 0;	
	for (i = 0; i < TJSON_EXIST_FILE_COUNT; ++i) {
		json_file = tjson_exist_files[i];		
		stream = tjson_read_file(json_file);
		json = NULL;
		TJSON_INIT(&json);
		result = tjson_parse(&json, stream);
		TEST_CHECK_((result != -1), "stream parsed succesfully");
	}

	/* none exist files */
	for (i = 0; i < TJSON_NONE_EXIST_FILE_COUNT; ++i) {
		json_file = tjson_none_exist_files[i];		
		stream = tjson_read_file(json_file);
		result = tjson_parse(&json, stream);
		TEST_CHECK_((result == -1), "stream NOT parsed succesfully");
	}

}

TEST_LIST = {
	{"int tests();", tests},
	{"int tests_read();", tests_read},
	{"int tests_parse();", tests_parse},
	{0}
};
