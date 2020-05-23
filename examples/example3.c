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

	TJSON_t *root = tanto_init_node(TANTO_JSON_OBJ, "root", NULL);
	tanto_push(&json, root);

	TJSON_t *node1 = tanto_init_node(TANTO_JSON_FIELD, "hello", "world!");
	TJSON_t *node2 = tanto_init_node(TANTO_JSON_FIELD, "parser", "tanto");
	TJSON_t *node3 = tanto_init_node(TANTO_JSON_OBJ, "library", NULL);
	TJSON_t *node4 = tanto_init_node(TANTO_JSON_FIELD, "lex", "lexer");
	tanto_child_push(&root, node1);
	tanto_child_push(&root, node2);
	tanto_child_push(&root, node3);
	tanto_child_push(&root, node4);

	TJSON_t *node31 = tanto_init_node(TANTO_JSON_FIELD ,"usage", "example.c");
	tanto_child_push(&node3, node31);

	tanto_print(json->child, 1);
		
	//tanto_write_file("hello_world.json", json);
}
