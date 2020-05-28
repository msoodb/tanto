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
#define _LIBRARY_AUTHORS    "msoodb (Masoud Bolhassani)"
#define _LIBRARY_VERSION    "0.0.1"
#define _LIBRARY_URL        "https://msoodb.org/tanto"

#define TJSON_INIT(json)     tjson_init((json));

/*#define TANTO_JSON_OBJECT          0
#define TANTO_JSON_ARRAY           1
#define TANTO_JSON_OBJECT_FIELD    5
#define TANTO_JSON_ARRAY_FIELD     6*/


#define TJSON_STRING          0
#define TJSON_NUMBER          1
#define TJSON_OBJECT          2
#define TJSON_ARRAY           3
#define TJSON_BOOL            4
#define TJSON_NULL            5


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


void _tjson_erase_node(TJSON_t **node)
{
	if (*node != NULL){
		if ((*node)->key != NULL) free((*node)->key);
		if ((*node)->v_string != NULL) free((*node)->v_string);
		free(*node);
	}		
}

void _stack_push(S_NODE_t **stack, TJSON_t *data)
{ 
	S_NODE_t *node = (S_NODE_t *)malloc(sizeof(S_NODE_t));
	if (node == NULL) return;
	
	node->data = data;
	node->next = (*stack);

	(*stack) = node;
}

TJSON_t *_stack_pop(S_NODE_t **stack)
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

TJSON_t *_tjson_create_node_empty(char *key, int v_type)
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
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_STRING);
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
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_NUMBER);
	if (node == NULL) return NULL;
	
	node->v_number = v_number;
 
	return node;
}

TJSON_t *tjson_create_node_object(char *key)
{
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_OBJECT);
	if (node == NULL) return NULL;

	return node;
}

TJSON_t *tjson_create_node_array(char *key)
{
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_ARRAY);
	if (node == NULL) return NULL;

	return node;
}

TJSON_t *tjson_create_node_bool(char *key, bool v_bool)
{
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_BOOL);
	if (node == NULL) return NULL;

	node->v_bool = v_bool;

	return node;
}

TJSON_t *tjson_create_node_null(char *key)
{
	TJSON_t *node = _tjson_create_node_empty(key, TJSON_NULL);
	if (node == NULL) return NULL;

	return node;
}

/*TJSON_t *tjson_create_node(int v_type, char *key, char *value)
{
	T JSON_t *node = (TJSON_t*) malloc(sizeof(TJSON_t) * 1);
	if (node == NULL) return NULL;
	
	node->child = NULL;
	node->next = NULL;

	node->key = NULL;
	node->value = NULL;
	node->type = type;
	
	if (key != NULL) {
		size_t z;
		z = strlen(key);
		node->key = (char *)malloc(sizeof(char) * (z+1));
		memcpy(node->key, key, z);
		node->key[z] = '\0';
	}

	if (value != NULL) {
		size_t z;
		z = strlen(value);
		node->value = (char *)malloc(sizeof(char) * (z+1));
		memcpy(node->value, value, z);
		node->value[z] = '\0';
		}

	return node;
	}*/

/*TJSON_t *_tanto_create_node(int type, char *key, char *value)
{
	TJSON_t *node = (TJSON_t*) malloc(sizeof(TJSON_t) * 1);
	if (node == NULL) return NULL;
	
	node->child = NULL;
	node->next = NULL;

	node->key = key;
	node->value = value;
	node->type = type;
	
	return node;
	}*/

void tjson_init(TJSON_t **json)
{
	TJSON_t *node = _tjson_create_node_empty(NULL, TJSON_OBJECT);
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

void _tjson_print(TJSON_t *json, FILE *fp, int type, int level)
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
		case TJSON_STRING: {
			if (json->v_string != NULL) fprintf(fp, "\"%s\"", json->v_string);
			break;
		}
		case TJSON_NUMBER: {
			fprintf(fp, "%6.2lf", json->v_number);
			break;
		}
		case TJSON_BOOL: {
			if (json->v_bool == true) fprintf(fp, "%s", "true");
			else if (json->v_bool == false) fprintf(fp, "%s", "false");
			break;
		}
		case TJSON_NULL: {
			fprintf(fp, "%s", "null");
			break;
		}
		default:
			break;
		}
		
		
		comma = 1;
		
		if ((json->v_type == TJSON_OBJECT) ||
		    (json->v_type == TJSON_ARRAY)) {
			_tjson_print(json->child, fp, json->v_type, level+1);
		}
		
		json = json->next;
	}
	fprintf(fp, "\n%*s%c", (level-1)*2, "", end);
	if (level == 1) fprintf(fp, "\n");
}

void tjson_print(TJSON_t *json)
{
	_tjson_print(json->child, stdout, json->v_type, 1);
}


void tjson_print_addr(TJSON_t *json)
{
	while (json != NULL) {

		printf("\t%s\n", "  --------------------");
		printf("%8p", (void *)json);
		printf("%s", " | ");
		printf("%8p %s", (void *)json->next, "|");
		printf("%8p %s\n", (void *)json->child, "|");
		printf("\t%s\n\n", "  --------------------");
		tjson_print_addr(json->child);

		json = json->next;
	}	
}

TJSON_t *tjson_lex_object(char *chunk)
{       
	TJSON_t *node;
	size_t step;

	char *key;
	char *value;
	int type;
	
	node = NULL;

	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;	
	if (strlen(chunk) < 1) goto failure;

	char delimiter = *chunk;
	if (delimiter == '{') {
		key = NULL;
		type = TJSON_OBJECT;
		value = NULL;
		goto success;
	}


	// key
	step = strcspn(chunk, "\"");
	key = malloc(sizeof(char) * (step + 1));	
	if (key == NULL) goto failure;	
	memcpy(key, chunk, step);
	key[step] = '\0';
	chunk += step + 1;
			

	// type
	type = TJSON_OBJECT;

	// value
	value = NULL;

	goto success;

failure:
	node = NULL;
	if (key != NULL) free(key);
	return node;
	
success:
	//node = _tanto_create_node(type, key, value);
	return node;
}

TJSON_t *tjson_lex_array(char *chunk) 
{ 
 	TJSON_t *node;
	size_t step;

	char *key;
	char *value;
	int type;
	
	node = NULL;

	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	if (strlen(chunk) <= 1) goto failure;


	// key
	step = strcspn(chunk, "\"");
	key = malloc(sizeof(char) * (step + 1));	
	if (key == NULL) goto failure;	
	memcpy(key, chunk, step);
	key[step] = '\0';
	chunk += step + 1;
			

	// type
	type = TJSON_ARRAY;

	// value
	value = NULL;

	goto success;

failure:
	node = NULL;
	return node;

success:
	//node = _tanto_create_node(type, key, value);
	return node;
}
  
TJSON_t *tjson_lex_field(char *chunk)
{       
	TJSON_t *node;
	size_t step;

	char *key;
	char *value;
	int type;
	
	node = NULL;
	
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++; 
	if (strlen(chunk) <= 1) goto failure;

	// key
	step = strcspn(chunk, "\"");
	key = malloc(sizeof(char) * (step + 1));	
	if (key == NULL) goto failure;	
	memcpy(key, chunk, step);
	key[step] = '\0';
	chunk += step + 1;
			

	// type 
	while(isspace((unsigned char)*chunk)) chunk++;
	if (*chunk == ',' || *chunk == ']') {
		type = TJSON_STRING;
		value = NULL;
		goto success;
	}

	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;

	// value
	step = strcspn(chunk, "\"");
	value = malloc(sizeof(char) * (step + 1));
	if (value == NULL) goto failure;
	
	memcpy(value, chunk, step);
	value[step] = '\0';
	type = TJSON_STRING;
	
success:
	//node = _tanto_create_node(type, key, value);
	return node;

failure:	
	node = NULL;
	return node;
}

int tjson_parse(TJSON_t **json, const char *stream)
{
	if (stream == NULL) return -1;
	
	S_NODE_t *stack;
	TJSON_t *current;
	TJSON_t *new;
	char *chunk; 
	size_t step;
	char delimiter;
	
	stack = NULL;
	chunk = NULL;
	step = 0;
	

	step = strcspn(stream, "{");
	stream += (step + 1);
	current = *json;

	// iterate over stream
	while (*stream != '\0') {	
		step = strcspn(stream, ",{[]}");

		// get chunk plus delimiters
		chunk = malloc(sizeof(char) * (step + 2));
		memcpy(chunk, stream, step + 1);
		chunk[step + 1] = '\0';

		// delimiter
		delimiter = *(stream + step);

		
		switch (delimiter) {
		case ',': {			
			new = tjson_lex_field(chunk);
			if (new != NULL) tjson_push(&current, new);
			break;
		}
		case '{': {			
			//new = tanto_lex_object(chunk);
			tjson_push(&current, new);
			_stack_push(&stack, current);
			current = new;
			break;
		}
		case '[': {
			//new = tjson_lex_array(chunk);
			tjson_push(&current, new);
			_stack_push(&stack, current);
			current = new;
			break;
		}
		case ']': {
			//new = tjson_lex_field(chunk);			
			if (new != NULL) tjson_push(&current, new);
			current = _stack_pop(&stack);
			break;
		}
		case '}': {
			new = tjson_lex_field(chunk);
			if (new != NULL) tjson_push(&current, new);
			current = _stack_pop(&stack);
			break;
		}
		default:
			break;
		}

		if (chunk != NULL) free (chunk);
		stream += (step+1);
 	}

	if (current != NULL) _tjson_erase_node(&current);
	if (stack != NULL) {		
		free(stack);
		return -1;
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

	_tjson_print(json->child, fp, json->v_type, 1);
	
	fclose(fp);
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

		_tjson_erase_node(&current);
	}
}

#endif  //__TANTO_H
