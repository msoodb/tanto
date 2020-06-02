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

#define TJSON_INIT(json)     tjson_init((json));


#define TJSON_STRING          0
#define TJSON_NUMBER          1
#define TJSON_OBJECT          2
#define TJSON_ARRAY           3
#define TJSON_BOOL            4
#define TJSON_NULL            5

#define TJSON_OUT_TOKEN 0
#define TJSON_IN_TOKEN 1

#define TJSON_OUT_ESCAPE_CHAR 0
#define TJSON_IN_ESCAPE_CHAR 1


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

		_tjson_erase_node(&current);
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
		case TJSON_STRING:
			if (json->v_string != NULL) fprintf(fp, "\"%s\"", json->v_string);
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

/* TO DO : pass pointer to str, ...(char **str) */
char *_trim_whitespace(char *str)
{
	char *end;

	while(isspace((unsigned char)*str)) str++;
	if(*str == 0) 
		return str;

	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	end[1] = '\0';
	return str;
}

/* TO DO : pass pointer to str, ...(char **str) */
char *_trim_double_quote(char *str)
{
	if (str == NULL) return NULL;
	char *end;

	while(*str == '"') str++;
	if(*str == 0) 
		return str;

	end = str + strlen(str) - 1;
	while(end > str && *end == '"') end--;

	end[1] = '\0';
	return str;
}

int _is_numeric(const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;

	char * p;
	strtod (s, &p);
	return *p == '\0';
}


int _tjson_tokenize(const char *stream, char **token)
{
	
	int token_status;
	int escape_status;
	int finish;	
	size_t step;

	*token = (char *)malloc(sizeof(char));	
	if (*token == NULL) return -1;
	**token = '\0';

	/* DELETE this useless piece of shit and OPEN UP GATES of HELL, or FIND and DESTROY the BUG */
	char *token2;
	token2 = (char *)malloc(sizeof(char));
	if (token2 == NULL) return -1;
	*token2 = '\0';
		
	token_status = TJSON_OUT_TOKEN;
	escape_status = TJSON_OUT_ESCAPE_CHAR;
	finish = step = 0;

	char c;
	while ((finish == 0) && ((c = *(stream + step)) != '\0') ) {
		
		if (token_status == TJSON_OUT_TOKEN && (isspace(c) || c == ':')) {
			step++;
			continue;
		}

		switch (c) {
		case '"':
			if (token_status == TJSON_OUT_TOKEN) token_status = TJSON_IN_TOKEN;
			else if (escape_status == TJSON_OUT_ESCAPE_CHAR) finish = 1;
			else {
				escape_status = TJSON_OUT_ESCAPE_CHAR;
				strncat(*token, &c, 1);
			}				
			break;
		case '\\':
			escape_status = (escape_status == TJSON_IN_ESCAPE_CHAR) ?
				TJSON_OUT_ESCAPE_CHAR : TJSON_IN_ESCAPE_CHAR;
			strncat(*token, &c, 1);
			break;
		//case '\/':
		case 'b':
		case 'f':
		case 'n':
		case 'r':
		case 't':
		case 'u':
			if (escape_status == TJSON_IN_ESCAPE_CHAR) escape_status = TJSON_OUT_ESCAPE_CHAR;			
			strncat(*token, &c, 1);
			break;
		case ',':
		case ']':
		case '}':
			finish = 1;
			break;
		default:
			strncat(*token, &c, 1);
			break;
		}			
		step++;
	}

	return step;
}

int tjson_lex(char *chunk, TJSON_t **node)
{	
	size_t step;
	
	char *key;
	char *value;

	char *v_string;
	double v_number;
	bool v_bool;

	step = 0;	
	key = value = v_string = NULL;
	v_number = 0;
	v_bool = false;

	while(isspace((unsigned char)*chunk)) chunk++;
	char delimiter = *chunk;

	switch (delimiter) {
	case '\0':
	case ',':
	case ']':
	case '}':
		*node = NULL;
		goto success;
	case '[':
		*node = tjson_create_node_array(NULL);
		goto success;	
	case '{':
		*node = tjson_create_node_object(NULL);
		goto success;
	case '"':
		break;
	default:
		break;			
	}

	
	// extract key
	step = strcspn(chunk, ":,]\n");
	key = malloc(sizeof(char) * (step + 1));
	if (key == NULL)
		goto failure;	
	memcpy(key, chunk, step);
	key[step] = '\0';		
	key = _trim_whitespace(key);
	chunk += step + 1;
	
	// extract value
	while(isspace((unsigned char)*chunk) || *chunk == ':') chunk++;
	step = strcspn(chunk, ",]}\n");
	if (step > 0) {
		value = malloc(sizeof(char) * (step + 1));
		if (value == NULL)
			goto failure;	
		memcpy(value, chunk, step);
		value[step] = '\0';	
		chunk += step + 1;

		value = _trim_whitespace(value);		
	}
	if (key && value == NULL) {
		value = key;
		key = NULL;
	}
	key = _trim_double_quote(key);
		


	if (_is_numeric(value)) {		
		v_number = strtod(value, NULL);
		*node = tjson_create_node_number(key, v_number);
	}
	else if (strcmp(value, "null") == 0) {
		*node = tjson_create_node_null(key);
	}
	else if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
		v_bool = true ? strcmp(value, "true") == 0 : false;
		*node = tjson_create_node_bool(key, v_bool);
	}
	else if (strcmp(value, "[") == 0) {
		*node = tjson_create_node_array(key);
	}
	else if (strcmp(value, "{") == 0) {
		*node = tjson_create_node_object(key);
	}
	else {
		v_string = _trim_double_quote(value);	
		*node = tjson_create_node_string(key, v_string);
	}
	
success:
	return 1;

failure:
	*node = NULL;
	return -1;
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
	current = NULL;
	new = NULL;
	chunk = NULL;
	step = 0;
	
	while(isspace((unsigned char)*stream)) stream++;
	delimiter = *stream;
	if (delimiter != '{') return -1;
		
	stream++;
	current = *json;

	while (*stream != '\0') {
		step = strcspn(stream, ",{[]}");

		chunk = malloc(sizeof(char) * (step + 2));
		memcpy(chunk, stream, step + 1);
		chunk[step + 1] = '\0';

		if (tjson_lex(chunk, &new) < 0) {
			return -1;
		}

		if (new != NULL) {
			tjson_push(&current, new);
		}

		
		delimiter = *(stream + step);
		
		switch (delimiter) {
		case '{': 			
		case '[':
			_stack_push(&stack, current);
			current = new;			
			break;		
		case ']':
			if (current->v_type != TJSON_ARRAY) return -1;
			current = _stack_pop(&stack);			
			break;		
		case '}':
			if (current->v_type != TJSON_OBJECT) return -1;
			current = _stack_pop(&stack);			
			break;		
		default:
			break;
		}
		
		if (chunk != NULL) free (chunk);
		stream += (step + 1);
 	}

	// TO DO : Bug Fix. */
	if (current != NULL) tjson_erase(&current);
	
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


#endif  //__TANTO_H
