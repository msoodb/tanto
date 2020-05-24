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
   "variables": [
     "char",
     "int",
     "float",
     "double"
   ],
 }
*/



/* 
 * create json, add field, and print 
*/
void main()
{
	TJSON_t *c = NULL;
	tanto_init(&c);

	TJSON_t *functions = tanto_create_node(TANTO_JSON_ARRAY, "functions", NULL);
	tanto_push(&c, functions);
	TJSON_t *pop = tanto_create_node(TANTO_JSON_FIELD ,"pop", NULL);	
	tanto_push(&functions, pop);
	TJSON_t *push = tanto_create_node(TANTO_JSON_FIELD ,"push", NULL);	
	tanto_push(&functions, push);

	TJSON_t *variables = tanto_create_node(TANTO_JSON_ARRAY, "variables", NULL);
	tanto_push(&c, variables);
	TJSON_t *char_var = tanto_create_node(TANTO_JSON_FIELD ,"char", NULL);	
	tanto_push(&variables, char_var);
	TJSON_t *int_var = tanto_create_node(TANTO_JSON_FIELD ,"int", NULL);	
	tanto_push(&variables, int_var);
	TJSON_t *float_var = tanto_create_node(TANTO_JSON_FIELD ,"float", NULL);	
	tanto_push(&variables, float_var);
	TJSON_t *double_var = tanto_create_node(TANTO_JSON_FIELD ,"double", NULL);	
	tanto_push(&variables, double_var);
	
	
	tanto_print(c);
		
	//tanto_write_file("c.json", json);


	TJSON_t *node = NULL;
	node = tanto_find(variables, "double");
	if (node != NULL) {
		printf("%s\n", node->key);
	}

}
