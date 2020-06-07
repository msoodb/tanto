#ifndef __TANTO_H
#define __TANTO_H

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define _LIBRARY_NAME       "tanto"
#define _LIBRARY_DESC       "json parser and creator library in C"
#define _LIBRARY_AUTHORS    "msoodb (Masoud Bolhassani), adavari (Alireza Davari)"
#define _LIBRARY_VERSION    "0.0.1"
#define _LIBRARY_URL        "https://msoodb.org/tanto"

#define TJSON_INIT(json)     __tjson_init((json));


#define TJSON_ERROR     -1
#define TJSON_STRING     0
#define TJSON_NUMBER     1
#define TJSON_OBJECT     2
#define TJSON_ARRAY      3
#define TJSON_BOOL       4
#define TJSON_NULL       5


/*
 * PARSER definition
 */
#define TJSON_OUT_OF_TOKEN      -1
#define TJSON_IN_STRING_TOKEN    0
#define TJSON_IN_NUMBER_TOKEN    1
#define TJSON_IN_OBJECT_TOKEN    2
#define TJSON_IN_ARRAY_TOKEN     3
#define TJSON_IN_BOOL_TOKEN      4
#define TJSON_IN_NULL_TOKEN      5

#define TJSON_OUT_OF_ESCAPE_CHAR   0
#define TJSON_IN_ESCAPE_CHAR       1


typedef struct __json
{
	struct __json *child;
	struct __json *next;
	char *key;
	int v_type;
	
	char *v_string;
	double v_number;
	bool v_bool;	

} TJSON_t;


typedef struct __stack_node
{
	TJSON_t *data;	
	struct __stack_node *next;
} S_NODE_t;


void __tjson_erase_node(TJSON_t **node)
{	
	if (*node == NULL) return;

	if ((*node)->key != NULL) free((*node)->key);
	if ((*node)->v_string != NULL) free((*node)->v_string);
	
	free(*node);			
}

void tjson_erase(TJSON_t **json)
{
	TJSON_t *current = *json;
	
	while (*json) {	
		if ((*json)->child != NULL) {
			tjson_erase(&(*json)->child);
		}

		current = *json;
		*json = (*json)->next;

		__tjson_erase_node(&current);
	}
}

void __stack_push(S_NODE_t **stack, TJSON_t *data)
{
	S_NODE_t *node = (S_NODE_t *)malloc(sizeof(S_NODE_t));
	if (node == NULL) return;
	
	node->data = data;
	node->next = (*stack);

	(*stack) = node;
}

TJSON_t *__stack_pop(S_NODE_t **stack)
{
	TJSON_t *data;
	S_NODE_t *top;
	if (*stack == NULL) return 0;

	top = *stack;	
	data = top->data;	
	*stack = top->next;

	free(top);	

	return data;
}

void __stack_print(S_NODE_t *stack)
{
	while (stack != NULL) {	
		printf("%8p\n", (void *)stack);		
		stack = stack->next;
	}	
}

TJSON_t *__tjson_create_node_empty(char *key, int v_type)
{
	TJSON_t *node = (TJSON_t*) malloc(sizeof(TJSON_t) * 1);
	if (node == NULL) return NULL;

	
	node->child = NULL;
	node->next = NULL;
	node->key = NULL;
	node->v_type = v_type;

	node->v_string = NULL;
	node->v_number = 0;
	node->v_bool = false;

	
	if (key != NULL) {
		size_t z;
		z = strlen(key);		
		node->key = (char *)malloc(sizeof(char) * (z+1));		
		memcpy(node->key, key, z);
		node->key[z] = '\0';
	}

	return node;
}

TJSON_t *tjson_create_node_string(char *key, char *v_string)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_STRING);
	if (node == NULL) return NULL;

	if (v_string != NULL) {
		size_t z;
		z = strlen(v_string);
		node->v_string = (char *)malloc(sizeof(char) * (z+1));
		memcpy(node->v_string, v_string, z);
		node->v_string[z] = '\0';
	}

	return node;
}

TJSON_t *tjson_create_node_number(char *key, double v_number)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_NUMBER);
	if (node == NULL) return NULL;
	node->v_number = v_number;

	return node;
}

TJSON_t *tjson_create_node_object(char *key)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_OBJECT);
	if (node == NULL) return NULL;

	return node;
}

TJSON_t *tjson_create_node_array(char *key)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_ARRAY);
	if (node == NULL) return NULL;

	return node;
}

TJSON_t *tjson_create_node_bool(char *key, bool v_bool)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_BOOL);
	if (node == NULL) return NULL;

	node->v_bool = v_bool;
	return node;
}

TJSON_t *tjson_create_node_null(char *key)
{
	TJSON_t *node = __tjson_create_node_empty(key, TJSON_NULL);
	if (node == NULL) return NULL;

	return node;
}

void __tjson_init(TJSON_t **json)
{
	TJSON_t *node = __tjson_create_node_empty(NULL, TJSON_OBJECT);
	if (node == NULL) return;

	*json = node;
}

void tjson_push(TJSON_t **json, TJSON_t *node)
{
	node->next = (*json)->child;
	(*json)->child = node;
}

TJSON_t *tjson_pop(TJSON_t **json)
{
	if (*json == NULL) return NULL;
	
	TJSON_t *current = *json;
	*json = (*json)->next;
	
	return current;
}

TJSON_t *tjson_find(TJSON_t *json, char *key)
{
	if (json == NULL || json->child == NULL)
		return NULL;

	json = json->child;
	
	while (json != NULL) {
		if (strcmp(json->key, key) == 0) return json;
		json = json->next;
	}

	return NULL;
}

void __tjson_print(TJSON_t *json, FILE *fp, int type, int level)
{
	int comma;
	comma = 0;

	if (level < 1) level = 1;
	
	char start, end;
	start = '{';
	end = '}';

	if (type == TJSON_ARRAY){
		start = '[';
		end = ']';
	}
	
	fprintf(fp, "%c\n", start);
	while (json != NULL) {
		if (comma) fprintf(fp, "%c\n", ',');

		fprintf(fp, "%*s", level*2, "");
		
		if (json->key != NULL) {
			fprintf(fp, "\"%s\"", json->key);
			if (type != TJSON_ARRAY) fprintf(fp, ": " );
		}
		
		switch (json->v_type) {
		case TJSON_STRING:
			if (json->v_string != NULL)
				fprintf(fp, "\"%s\"", json->v_string);
			else
				fprintf(fp, "\"%s\"", "");			
			break;		
		case TJSON_NUMBER: 
			fprintf(fp, "%.3lf", json->v_number);
			break;		
		case TJSON_BOOL: 
			if (json->v_bool == true) fprintf(fp, "%s", "true");
			else if (json->v_bool == false) fprintf(fp, "%s", "false");
			break;		
		case TJSON_NULL: 
			fprintf(fp, "%s", "null");
			break;		
		default:
			break;
		}
		
		comma = 1;
		
		if ((json->v_type == TJSON_OBJECT) ||
		    (json->v_type == TJSON_ARRAY)) {
			__tjson_print(json->child, fp, json->v_type, level+1);
		}
		
		json = json->next;
	}
	fprintf(fp, "\n%*s%c", (level-1)*2, "", end);
	if (level == 1) fprintf(fp, "\n");
}

void tjson_print(TJSON_t *json)
{
	if (json == NULL) return;
	__tjson_print(json->child, stdout, json->v_type, 1);
}

void __tjson_print_addr(TJSON_t *json)
{
	while (json != NULL) {

		printf("\t%s\n", "  --------------------");
		printf("%8p", (void *)json);
		printf("%s", " | ");
		printf("%8p %s", (void *)json->next, "|");
		printf("%8p %s\n", (void *)json->child, "|");
		printf("\t%s\n\n", "  --------------------");
		__tjson_print_addr(json->child);

		json = json->next;
	}	
}

void __append_char(char **token, size_t token_size, char *c)
{
	size_t s;
	s = token_size;
	
	if (*token == NULL) {
		*token = (char *)malloc(sizeof(char));			
		*token = '\0';
		s = 1;
	}

	*token = (char *)realloc(*token, s * sizeof(char));
	strncat(*token, c, 1);
}

int tjson_parse(TJSON_t **json, const char *stream)
{
	if (stream == NULL) return -1;

	S_NODE_t *stack;
	TJSON_t *current, *new;
	stack = NULL;
	current = new = NULL;	
	
	char *token;
	size_t token_size;
	int status;
	token = NULL;
	token_size = 1;
	status = TJSON_OUT_OF_TOKEN;
	
	char *first, *second;
	first = second = NULL;

	double v_number;
	bool v_bool;
	v_number = 0;
	v_bool = false;


	int line_number;

	/* flages */
	bool is_dirty, save;
	int token_type;	
	int end_of_object_f, end_of_array_f;
	int escape_char_status;
	
	is_dirty = save = false;
	token_type = TJSON_STRING;
	end_of_object_f = end_of_array_f = 0;
	escape_char_status = TJSON_OUT_OF_ESCAPE_CHAR;


       	while(isspace((unsigned char)*stream)) stream++;
	if (*stream != '{') return -1;
		
	stream++;
	current = *json;


	line_number = 1;

	char c;
	do {
		c = *stream++;

		switch (c) {
		case ' ':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:			
				break;
			}
			break;		

		case '\n':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:
				break;
			}
			line_number++;
			break;		
		case '"':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				if (escape_char_status == TJSON_IN_ESCAPE_CHAR) {
					__append_char(&token, ++token_size, &c);
					escape_char_status = TJSON_OUT_OF_ESCAPE_CHAR;
				}else {					
					token_type = TJSON_STRING;
					status = TJSON_OUT_OF_TOKEN;
				}
				break;
			case TJSON_OUT_OF_TOKEN:				
				token = NULL;
				token_size = 1;
				status = TJSON_IN_STRING_TOKEN;
				token_type = TJSON_STRING;
				is_dirty = true;
				break;			
			default:				
				return line_number;
			}			
			break;
		case '\\':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				if (escape_char_status == TJSON_OUT_OF_ESCAPE_CHAR) {
					escape_char_status = TJSON_IN_ESCAPE_CHAR;
				}else {
					escape_char_status = TJSON_OUT_OF_ESCAPE_CHAR;
				}
				__append_char(&token, ++token_size, &c);
				break;
			default:
				return line_number;
			}
			break;
		case '{':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:				
				status = TJSON_OUT_OF_TOKEN;
				token_type = TJSON_OBJECT;
				second = NULL;				
				save = is_dirty = true;
				break;
			}			
			break;
		case '[':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:				
				status = TJSON_OUT_OF_TOKEN;
				token_type = TJSON_ARRAY;
				second = NULL;
				save = is_dirty = true;
				break;
			}
			break;
		case ']':			
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:				
				second = (char *)malloc(sizeof(char) * token_size);
				if (token != NULL) {
					strncpy(second, token, token_size);
				}else {
					second = NULL;
				}
				status = TJSON_OUT_OF_TOKEN;
				end_of_array_f = true;
				save = is_dirty;
				break;
			}			
			break;
		case '}':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:
				second = (char *)malloc(sizeof(char) * token_size);
				if (token != NULL) {
					strncpy(second, token, token_size);
				}else {
					second = NULL;
				}
				status = TJSON_OUT_OF_TOKEN;
				end_of_object_f = true;
				save = is_dirty;
			 	break;
			}			
			break;
		case ':':
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:				
				first = (char *)malloc(sizeof(char) * token_size);
				strncpy(first, token, token_size);
				status = TJSON_OUT_OF_TOKEN;
				break;
			}				
			break;
		case '\0':
			break;
		case ',':			
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				__append_char(&token, ++token_size, &c);
				break;
			default:				
				second = (char *)malloc(sizeof(char) * token_size);
				if (token != NULL) {
					strncpy(second, token, token_size);
				}else {
					second = NULL;
				}
				status = TJSON_OUT_OF_TOKEN;
				save = is_dirty;
				break;
			}			
			break;
		default:
			if (escape_char_status == TJSON_IN_ESCAPE_CHAR) {
				switch (c) {
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					__append_char(&token, ++token_size, &c);
					escape_char_status = TJSON_OUT_OF_ESCAPE_CHAR;
					break;				
				default:
					return line_number;
				}
				break;
			}
			switch (status) {
			case TJSON_IN_STRING_TOKEN:
				break;
			case TJSON_IN_NUMBER_TOKEN:
				if (!isdigit(c)
				    && c != '.'
				    && c != '-'
				    && c != '+'
				    && c != 'e'
				    && c != 'E') return line_number;
				break;
			case TJSON_IN_BOOL_TOKEN:
				if (c != 't'
				    && c != 'r'
				    && c != 'u'
				    && c != 'e'
				    && c != 'f'
				    && c != 'a'
				    && c != 'l'
				    && c != 's') return line_number;
				break;
			case TJSON_IN_NULL_TOKEN:
				if (c != 'n'
				    && c != 'u'
				    && c != 'l') return line_number;
				break;
			case TJSON_OUT_OF_TOKEN:
				token = NULL;
				token_size = 1;
				is_dirty = true;
				if (isdigit(c) || c == '-') {
					status = TJSON_IN_NUMBER_TOKEN;
					token_type = TJSON_NUMBER;
				}
				else if (c == 't' || c == 'f') {
					status = TJSON_IN_BOOL_TOKEN;
					token_type = TJSON_BOOL;
				}
				else if (c == 'n') {
					status = TJSON_IN_NULL_TOKEN;
					token_type = TJSON_NULL;
				}
				break;
			default:
				return line_number;
			}			
			__append_char(&token, ++token_size, &c);
			break;
		}
		
		if (save) {
			new = NULL;

			//printf("(SAVE) %s:%s\n", first, second);
			
			switch (token_type){
			case TJSON_STRING:
				new = tjson_create_node_string(first, second);
				if (new == NULL) {		
					return line_number;
				}
				tjson_push(&current, new);
				break;
			case TJSON_NUMBER:
				v_number = strtod(second, NULL);
				new = tjson_create_node_number(first, v_number);
				if (new == NULL) {
					return line_number;
				}
				tjson_push(&current, new);
				break;
			case TJSON_BOOL:
				if (strcmp(second, "true") == 0 || strcmp(second, "false") == 0) {
					v_bool = true ? strcmp(second, "true") == 0 : false;
					new = tjson_create_node_bool(first, v_bool);
				}else {
					return line_number;
				}
				if (new == NULL) return line_number;
				tjson_push(&current, new);
				break;
			case TJSON_NULL:
				if (strcmp(second, "null") == 0)
					new = tjson_create_node_null(first);
				else {					
					return line_number;
				}
				if (new == NULL) return line_number;
				tjson_push(&current, new);
				break;				
			case TJSON_ARRAY:
				new = tjson_create_node_array(first);
				if (new == NULL) return line_number;
				tjson_push(&current, new);
				__stack_push(&stack, current);
				current = new;	
				break;
			case TJSON_OBJECT:
				new = tjson_create_node_object(first);
				if (new == NULL) return line_number;
				tjson_push(&current, new);
				__stack_push(&stack, current);
				current = new;	
				break;
			default:
				return line_number;
			}

			if (first != NULL) free(first);
			if (second != NULL) free(second);
			first = second = NULL;
			is_dirty = save = false;			
		}

		if (end_of_object_f) {			
			if (current->v_type != TJSON_OBJECT) return line_number;
			current = __stack_pop(&stack);
			end_of_object_f = false;
		}

		if (end_of_array_f) {
			if (current->v_type != TJSON_ARRAY) return line_number;
			current = __stack_pop(&stack);
			end_of_array_f = false;
		}

	} while (c != '\0');

	if (stack != NULL) {		
		free(stack);
		return line_number;
	}

	return 0;
}

char *tjson_read_file(const char *file)
{
	FILE *fp;
	long f_size;
	char *stream;
	
	fp = NULL;
	f_size = 0;
	stream = NULL;
	
      	fp = fopen(file, "r");
	if (fp == NULL) return NULL;

	fseek(fp, 0L, SEEK_END);
        f_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);                               
			
	stream = (char *)malloc(sizeof(char) * (f_size + 1));
        fread(stream, sizeof(char), f_size, fp);
	fclose(fp);

	stream[f_size] = '\0';
	
	return stream;
}

void tjson_write_file(char *file, TJSON_t *json)
{
	FILE *fp;
	fp = fopen(file, "w");
	if(fp == NULL) return;

	__tjson_print(json->child, fp, json->v_type, 1);
	
	fclose(fp);
}

#endif  //__TANTO_H
