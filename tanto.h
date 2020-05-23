#ifndef _TANTO_H
#define _TANTO_H

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

#define IS_EMPTY_LIST(lsit)  ((list) == NULL)

typedef struct __stack_node
{
	char data;	
	struct __stack_node *next;
} S_NODE_t;

typedef struct __json
{
	struct __json *child;
	struct __json *next;
	
	char *key;
	char *value;
	int type;
} TJSON_t;

void stack_push(S_NODE_t **stack, char data)
{
	S_NODE_t *new = (S_NODE_t *)malloc(sizeof(S_NODE_t));
	if (new == NULL) return;
	
	new->data = data;
	new->next = (*stack);

	(*stack) = new;
}

char stack_pop(S_NODE_t **stack)
{
	char data;
	S_NODE_t *top;
	if (*stack == NULL) return 0;

	top = *stack;	
	data = top->data;	
	*stack = top->next;
	free(top);
	
	return data;
}

TJSON_t *tanto_init_node(char *key, char *value, int type)
{
	TJSON_t *node = (TJSON_t*) malloc(sizeof(TJSON_t) * 1);
	if (node == NULL) return NULL;
	
	node->child = NULL;
	node->next = NULL;

	node->key = key;
	node->value = value;
	node->type = type;

	return node;
}

void tanto_push(TJSON_t **json, TJSON_t *node)
{
	TJSON_t *current = *json;
	*json = node;

	node->next = current;	
}

TJSON_t *tanto_pop(TJSON_t **json)
{
	if (*json == NULL) return NULL;
	
	TJSON_t *current = *json;

	*json = (*json)->next;
	
	return current;
}

TJSON_t *tanto_find(TJSON_t *json, char *key)
{	
	while (json != NULL) {
		if (strcmp(json->key, key) == 0) return json;
		json = json->next;
	}
	return NULL;
}

void tanto_print(TJSON_t *json)
{
	bool comma;
	comma = false;
	
	printf("%c\n", '{');
	while (json != NULL) {
		if (comma) printf("%c\n", ',');
		printf("\t");
		printf("\"%s\":", json->key);
		printf(" \"%s\"", json->value);
		comma = true;
		json = json->next;
	}
	printf("\n%c\n", '}');		
}

void tanto_print_addr(TJSON_t *json)
{
	while (json != NULL) {

		printf("\t%s\n", "  ----------");
		printf("%8d", json);
		printf("%s", " | ");
		printf("%8d %s\n", json->next, "|");
		printf("\t%s\n\n", "  ----------");		

		json = json->next;
	}	
}

TJSON_t *tanto_lex(char *chunk)
{
	TJSON_t *node;
	size_t step;

	char *key;
	char *value;
	int type;
	
	node = NULL;
	
	/* 
	 * key 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	if (strlen(chunk) == 0) return NULL;
	

	step = strcspn(chunk, "\"\0");

	key = malloc(sizeof(char) * (step + 1));	
	if (key == NULL) return NULL;
	
	memcpy(key, chunk, step);
	key[step] = '\0';
	chunk += step + 1;

	/*
	 * type
	 */
	type = 0;
		
	/* 
	 * value 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
		
	step = strcspn(chunk, "\",\0");
	value = malloc(sizeof(char) * (step + 1));
	if (value == NULL) return NULL;
	
	memcpy(value, chunk, step);
	value[step] = '\0';

	node = tanto_init_node(key, value, type);

	return node;
}

TJSON_t *tanto_parse(const char *stream)
{
	char *chunk;
	size_t step;
	TJSON_t *json = NULL;
	TJSON_t *node = NULL;
	
	chunk = NULL;
	step = 0;
	

	int i = 0;
	while (strlen(stream) > 0) {
	
		step = strcspn(stream, ",");
		chunk = malloc(sizeof(char) * (step + 1));
		memcpy(chunk, stream, step);
		chunk[step] = '\0';
		
		stream += step + 1;
				
		node = tanto_lex(chunk);
		tanto_push(&json, node);
	}

	return json;
}

char *tanto_read_file(const char *file)
{
	FILE *fp;
	long f_size;
	char *stream;
	
	fp = NULL;
	f_size = 0;
	
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

void tanto_write_file(char *file, TJSON_t *json)
{
	FILE *fp;
	fp = fopen(file, "w");
	if(fp == NULL) return;

	bool comma;
	comma = false;
	
	fprintf(fp, "%c\n", '{');
	while (json != NULL) {
		if (comma) fprintf(fp, "%c\n", ',');
		fprintf(fp, "\t");
		fprintf(fp, "\"%s\":", json->key);
		fprintf(fp, " \"%s\"", json->value);
		comma = true;
		json = json->next;
	}
	fprintf(fp, "\n%c\n", '}');

	fclose(fp);
}

bool is_match(char character1, char character2) 
{ 
	if (character1 == '(' && character2 == ')') return 1; 
	else if (character1 == '{' && character2 == '}') return 1; 
	else if (character1 == '[' && character2 == ']') return 1; 
	return 0; 
}

bool parenthes_balanced(char *exp) 
{ 
	int i = 0; 
 
	S_NODE_t *stack = NULL; 
  
	while (exp[i]){ 
		if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[') 
			stack_push(&stack, exp[i]); 
  
		if (exp[i] == '}' || exp[i] == ')' || exp[i] == ']'){ 
			if (!is_match(stack_pop(&stack), exp[i])) return 0;
		} 
		i++; 
	} 
     
	if (stack == NULL) return 1;
   
	return 0;
}

#endif  //_TANTO_H
