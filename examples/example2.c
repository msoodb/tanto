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
    {
      "delete2": "iterative",
      "delete1": "recursive"
    },
    "push",
    "pop"
  ],
  "main": "main.c",
  "color": "red",
  "order": 63928.120,
  "parser": "tanto",
  "path": "http://www.gnu.org/licenses/",
  "server": {
    "lex": "lexer",
    "library": {
      "address": null,
      "support": true,
      "test": "test.c",
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
	TJSON_INIT(&json);


	TJSON_t *server = tjson_create_node_object("server");
	TJSON_t *path = tjson_create_node_string("path", "http://www.gnu.org/licenses/");
	tjson_push(&json, server);
	tjson_push(&json, path);

	
	TJSON_t *parser = tjson_create_node_string("parser", "tanto");
	TJSON_t *order = tjson_create_node_number("order", 63928.12);
	TJSON_t *library = tjson_create_node_object("library");
	TJSON_t *lex = tjson_create_node_string("lex", "lexer");	
	tjson_push(&json, parser);
	tjson_push(&json, order);
	tjson_push(&server, library);
	tjson_push(&server, lex);

	TJSON_t *usage = tjson_create_node_string("usage", "example.c");
	TJSON_t *test = tjson_create_node_string("test", "test.c");
	TJSON_t *support = tjson_create_node_bool("support", true);
	TJSON_t *address = tjson_create_node_null("address");
	tjson_push(&library, usage);	
	tjson_push(&library, test);	
	tjson_push(&library, support);
	tjson_push(&library, address);


	TJSON_t *color = tjson_create_node_string("color", "red");
	TJSON_t *main = tjson_create_node_string("main", "main.c");
	tjson_push(&json, color);
	tjson_push(&json, main);


	TJSON_t *functions = tjson_create_node_array("functions");
	tjson_push(&json, functions);
	
	TJSON_t *pop = tjson_create_node_string(NULL, "pop");
	TJSON_t *push = tjson_create_node_string(NULL, "push");
	TJSON_t *delete = tjson_create_node_object(NULL);
	tjson_push(&functions, pop);	
	tjson_push(&functions, push);	
	tjson_push(&functions, delete);

	TJSON_t *delete1 = tjson_create_node_string("delete1", "recursive");	
	TJSON_t *delete2 = tjson_create_node_string("delete2", "iterative");
	tjson_push(&delete, delete1);
	tjson_push(&delete, delete2);
	
	
	tjson_print(json);		
	//tjson_write_file("../json/example2-output.json", json);

	if (json != NULL) tjson_erase(&json);

	return 0;
}
