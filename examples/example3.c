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
 * create json, add field, and print 
*/
void main()
{
	TJSON_t *json = NULL;
	tanto_init(&json);


	TJSON_t *server = tanto_create_node(TANTO_JSON_OBJ, "server", NULL);
	tanto_push(&json, server);

	TJSON_t *path = tanto_create_node(TANTO_JSON_FIELD, "path", "http://www.gnu.org/licenses/");
	tanto_push(&json, path);

	
	TJSON_t *node2 = tanto_create_node(TANTO_JSON_FIELD, "parser", "tanto");
	TJSON_t *node3 = tanto_create_node(TANTO_JSON_OBJ, "library", NULL);
	TJSON_t *node4 = tanto_create_node(TANTO_JSON_FIELD, "lex", "lexer");	
	tanto_push(&json, node2);
	tanto_push(&server, node3);
	tanto_push(&server, node4);

	TJSON_t *node31 = tanto_create_node(TANTO_JSON_FIELD ,"usage", "example.c");
	TJSON_t *node32 = tanto_create_node(TANTO_JSON_FIELD ,"tests", "test.c");	
	tanto_push(&node3, node31);	
	tanto_push(&node3, node32);


	TJSON_t *node5 = tanto_create_node(TANTO_JSON_FIELD, "color", "red");
	tanto_push(&json, node5);

	TJSON_t *node33 = tanto_create_node(TANTO_JSON_FIELD ,"main", "main.c");	
	tanto_push(&json, node33);




	TJSON_t *functions = tanto_create_node(TANTO_JSON_ARRAY, "functions", NULL);
	tanto_push(&json, functions);
	TJSON_t *function1 = tanto_create_node(TANTO_JSON_FIELD ,"pop", NULL);	
	tanto_push(&functions, function1);
	TJSON_t *function2 = tanto_create_node(TANTO_JSON_FIELD ,"push", NULL);	
	tanto_push(&functions, function2);
	
	
	//tanto_print(json);
		
	tanto_write_file("hello_world.json", json);
}
