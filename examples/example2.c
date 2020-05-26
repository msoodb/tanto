/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../tanto.h"

/* 
 * create this json object and print on stdout or file.
 *
 {
   "functions": [
     "push",
     "pop"
   ],
   "main": "main.c",
   "color": "red",
   "parser": "tanto",
   "path": "http://www.gnu.org/licenses/",
   "server": {
     "lex": "lexer",
     "library": {
       "tests": "test.c",
       "usage": "example.c"
     }
   }
 }
*/



/* 
 * create json, add field, and print 
*/
int main()
{
	TJSON_t *json = NULL;
	TANTO_INIT(&json);


	TJSON_t *server = tanto_create_node(TANTO_JSON_OBJECT, "server", NULL);
	tanto_push(&json, server);

	TJSON_t *path = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "path", "http://www.gnu.org/licenses/");
	tanto_push(&json, path);

	
	TJSON_t *parser = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "parser", "tanto");
	TJSON_t *library = tanto_create_node(TANTO_JSON_OBJECT, "library", NULL);
	TJSON_t *lex = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "lex", "lexer");	
	tanto_push(&json, parser);
	tanto_push(&server, library);
	tanto_push(&server, lex);

	TJSON_t *usage = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"usage", "example.c");
	TJSON_t *tests = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"tests", "test.c");	
	tanto_push(&library, usage);	
	tanto_push(&library, tests);


	TJSON_t *color = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "color", "red");
	tanto_push(&json, color);

	TJSON_t *main = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"main", "main.c");	
	tanto_push(&json, main);


	TJSON_t *functions = tanto_create_node(TANTO_JSON_ARRAY, "functions", NULL);
	tanto_push(&json, functions);
	TJSON_t *pop = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"pop", NULL);	
	tanto_push(&functions, pop);
	TJSON_t *push = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"push", NULL);	
	tanto_push(&functions, push);
	
	
	//tanto_print(json);
		
	tanto_write_file("json/example3-output.json", json);

	return 0;
}
