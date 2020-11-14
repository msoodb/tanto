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



/* 
 * lexing function const and define 
*/
#define TJSON_IN_WHITE_SPACE      -1
#define TJSON_IN_STRING            0
#define TJSON_IN_ESCAPE_CHAR       1
#define TJSON_IN_IDENTIFIER        2

const int __tjson_keyword_count = 3;
const char *__tjson_keyword[] = {
	"true",
	"false",
	"null"
};

enum TJSON_TOKEN_TYPE {
	TJSON_TOKEN_KEYWORD,
	TJSON_TOKEN_IDENTIFIER,
	TJSON_TOKEN_STRING,
	TJSON_TOKEN_NUMBER,
	TJSON_TOKEN_SP_CHAR
};

/* 
 * parsing function const and define 
*/
#define TJSON_INIT(json)    __tjson_init((json));
#define __is_white_space(c) (c == 9 || c == 10 || c == 13 || c == 32)
#define __is_onenine(c)     (c >= 49 && c <= 57)
#define __is_digit(c)       (c == 48 || __is_onenine(c))

#define TJSON_ROOT      -1
#define TJSON_STRING     0
#define TJSON_NUMBER     1
#define TJSON_OBJECT     2
#define TJSON_ARRAY      3
#define TJSON_BOOL       4
#define TJSON_NULL       5

typedef struct __tjson
{
	struct __tjson *child;
	struct __tjson *next;
	char *key;
	int v_type;
	
	char *v_string;
	double v_number;
	bool v_bool;	

} TJSON_t;


typedef struct __ttoken
{
	char *data;
	int line;
	enum TJSON_TOKEN_TYPE type;

} TTOKEN_t;


typedef struct __tqueue
{	
	char *data;
	int line;
	enum TJSON_TOKEN_TYPE type;
	struct __tqueue *next;
} TQUEUE_t;


typedef struct __tstack
{
	TJSON_t *data;	
	struct __tstack *next;
} TSTACK_t;


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

void __tqueue_enqueue(TQUEUE_t **front, TQUEUE_t **rear,
		      char *data, int line, enum TJSON_TOKEN_TYPE type)
{
	TQUEUE_t *node = (TQUEUE_t *)malloc(sizeof(TQUEUE_t));
	if (node == NULL) return;

	node->data = data;
	node->line = line;
	node->type = type;

	if (*front == NULL && *rear == NULL) {
		*front = *rear = node;
		return;
	}
	
	(*rear)->next = node;
	*rear = (*rear)->next;
}

TQUEUE_t *__tqueue_dequeue(TQUEUE_t **front, TQUEUE_t **rear)
{
	TQUEUE_t *top;
	if (*front == NULL) return NULL;

	top = *front;	
	*front = (*front)->next;

	if (*front == NULL) *rear = NULL;

	return top;
}

void __tqueue_print(TQUEUE_t **front, TQUEUE_t **rear)
{	
	TQUEUE_t *node;
	char *str_type;
	node = NULL;
	str_type = NULL;
	
	printf("number\ttype\t\tvalue\n");
	printf("----------------------------------------\n");

	int i = 0;
	while (*front != NULL) {

		node = __tqueue_dequeue(front, rear);
		if (node == NULL || node->data == NULL) {
			printf("%d\t%s\t\t%s\n", ++i, "!!!!ERROR!!!!", "!!!!ERROR!!!!");
			return;
		}

		switch (node->type) {
		case TJSON_TOKEN_SP_CHAR: 
			str_type = "TJSON_TOKEN_SP_CHAR";
			break;
		case TJSON_TOKEN_STRING: 
			str_type = "TJSON_TOKEN_STRING";
			break;
		case TJSON_TOKEN_IDENTIFIER: 
			str_type = "TJSON_TOKEN_IDENTIFIER";
			break;
		case TJSON_TOKEN_NUMBER: 
			str_type = "TJSON_TOKEN_NUMBER";
			break;
		case TJSON_TOKEN_KEYWORD: 
			str_type = "TJSON_TOKEN_KEYWORD";
			break;
		default:
			str_type = "!!!!ERROR!!!!";
			break;
		}
		
		printf("%d\t%s\t\t%s\n", ++i, str_type, node->data);

		if (node != NULL && node->data != NULL) free(node->data);
		if (node != NULL) free(node);

		node = NULL;
	}

	if (node != NULL) free(node);
}

void __tstack_push(TSTACK_t **stack, TJSON_t *data)
{
	TSTACK_t *node = (TSTACK_t *)malloc(sizeof(TSTACK_t));
	if (node == NULL) return;
	
	node->data = data;
	node->next = (*stack);

	(*stack) = node;
}

TJSON_t *__tstack_pop(TSTACK_t **stack)
{
	TJSON_t *data;
	TSTACK_t *top;
	if (*stack == NULL) return 0;

	top = *stack;	
	data = top->data;	
	*stack = top->next;

	free(top);	

	return data;
}

void __tstack_print(TSTACK_t *stack)
{
	while (stack != NULL) {	
		printf("%8p\n", (void *)stack);		
		stack = stack->next;
	}	
}

long tjson_read_file(const char *file, char **stream)
{
	FILE *fp;
	long f_size;
	
	fp = NULL;
	f_size = 0;
	*stream = NULL;
	
      	fp = fopen(file, "r");
	if (fp == NULL) return 0L;

	fseek(fp, 0L, SEEK_END);
        f_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);                               

	*stream = (char *)malloc(sizeof(char) * (f_size + 1));
	memset(*stream, '\0', f_size + 1);

	fread(*stream, sizeof(char), f_size, fp);
	fclose(fp);
	
	//stream[f_size] = '\0';	
	return f_size;
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
	TJSON_t *node = __tjson_create_node_empty(NULL, TJSON_ROOT);
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

	//if (level < 1) level = 1;

	char start, end;

	if (type == TJSON_OBJECT){
		start = '{';
		end = '}';
	}
	if (type == TJSON_ARRAY){
		start = '[';
		end = ']';
	}
	
	if (type == TJSON_OBJECT || type == TJSON_ARRAY)
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
		
		if ((json->v_type == TJSON_ROOT)
		    || (json->v_type == TJSON_OBJECT)
		    || (json->v_type == TJSON_ARRAY)) {
			__tjson_print(json->child, fp, json->v_type, level+1);
		}
		
		json = json->next;
	}
	if (type == TJSON_OBJECT || type == TJSON_ARRAY)
		fprintf(fp, "\n%*s%c", (level-1)*2, "", end);
	if (level == 1) fprintf(fp, "\n");
}

void tjson_print(TJSON_t *json)
{
	if (json == NULL) return;
	__tjson_print(json->child, stdout, json->v_type, 0);
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

void tjson_write_file(char *file, TJSON_t *json)
{
	FILE *fp;
	fp = fopen(file, "w");
	if(fp == NULL) return;

	__tjson_print(json->child, fp, json->v_type, 1);
	
	fclose(fp);
}

int __tjson_is_keyword(const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;

	int i;
	for (i = 0; i < __tjson_keyword_count; ++i) {
		if (strcmp(s, __tjson_keyword[i]) == 0)
			return 1;
	}
	return 0;
}

int __tjson_is_numeric(const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
	char *p;
	strtod (s, &p);
	return *p == '\0';
}

void __tjson_append_char(char **token, char *c)
{
	if (*token == NULL) {
		*token = (char *)malloc(sizeof(char));
		**token = '\0';
	}

	size_t token_size = strlen(*token);

	*token = (char *)realloc(*token, (token_size + 2) * sizeof(char));
	strncat(*token, c, 1);
}

int __tjson_pre_process(char **src, char **dest)
{
	int status;	
	char *arrow;
	char *file_name;
	char *stream;
	
	file_name = NULL;
	stream = NULL;
	status = TJSON_IN_WHITE_SPACE;
	arrow = *src;

	char c;
	while ((c = *arrow++) != '\0') {
		switch (c) {
		case '#':			
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				status = TJSON_IN_IDENTIFIER;
				break;
			case TJSON_IN_IDENTIFIER:
				status = TJSON_IN_WHITE_SPACE;
				tjson_read_file(file_name, &stream);
				__tjson_pre_process(&stream, dest);
				break;
			default:
				break;
			}
			break;
		default:
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				__tjson_append_char(dest, &c);
				break;
			case TJSON_IN_IDENTIFIER:
				__tjson_append_char(&file_name, &c);
				break;
			default:
				break;
			}
		}
	}

	if (file_name != NULL) free(file_name);
	if (stream != NULL) free(stream);

	return 1;
}

int __tjson_lex(char **arrow, TTOKEN_t **token)
{		
	enum TJSON_TOKEN_TYPE token_type;
	char *token_data;
	int status;
	int line_number;

	token_type = TJSON_TOKEN_IDENTIFIER;
	token_data = NULL;
	status = TJSON_IN_WHITE_SPACE;
	line_number = 1;

	if (*token != NULL && (*token)->data != NULL) free((*token)->data);
	if (*token != NULL) free(*token);
	*token = NULL;
	
	*token = (TTOKEN_t *)malloc(sizeof(TTOKEN_t) * 1);
	
	char c;	
	while ((c = *(*arrow)++) != '\0') {

		switch (c) {
		case ' ':
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				break;
			case TJSON_IN_STRING:
				__tjson_append_char(&token_data, &c);
				break;
			case TJSON_IN_ESCAPE_CHAR:
				return line_number;
			case TJSON_IN_IDENTIFIER:
				if (__tjson_is_numeric(token_data))
					token_type = TJSON_TOKEN_NUMBER;
				else if (__tjson_is_keyword(token_data))
					token_type = TJSON_TOKEN_KEYWORD;
				(*token)->data = token_data;
				(*token)->type = token_type;
				(*token)->line = line_number;
				return 0;
			default:
				break;
			}			
			break;
		case '\0':
		case '\t':
		case '\n':
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				break;
			case TJSON_IN_STRING:
				__tjson_append_char(&token_data, &c);
				break;
			case TJSON_IN_ESCAPE_CHAR:
				return line_number;
			case TJSON_IN_IDENTIFIER:
				if (__tjson_is_numeric(token_data))
					token_type = TJSON_TOKEN_NUMBER;
				else if (__tjson_is_keyword(token_data))
					token_type = TJSON_TOKEN_KEYWORD;
				(*token)->data = token_data;
				(*token)->type = token_type;
				(*token)->line = line_number;
				return 0;	
			default:
				break;
			}			
			line_number++;
			break;
		case '"':
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				status = TJSON_IN_STRING;
				token_type = TJSON_TOKEN_STRING;
				break;
			case TJSON_IN_STRING:
				(*token)->data = token_data;
				(*token)->type = token_type;
				(*token)->line = line_number;
				return 0;
			case TJSON_IN_ESCAPE_CHAR:
				status = TJSON_IN_STRING;
				__tjson_append_char(&token_data, &c);
				break;
			case TJSON_IN_IDENTIFIER:
				return line_number;
			default:
				break;
			}
			break;
		case '\\':
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				return line_number;
			case TJSON_IN_STRING:
				status = TJSON_IN_ESCAPE_CHAR;
				break;
			case TJSON_IN_ESCAPE_CHAR:
				status = TJSON_IN_STRING;
				break;
			case TJSON_IN_IDENTIFIER:
				return line_number;
			default:
				break;
			}
			__tjson_append_char(&token_data, &c);
			break;
		case '{':
		case '[':
		case ']':
		case '}':
		case '(':
		case ')':
		case ';':			
		case ':':
		case ',':
			switch (status) {
			case TJSON_IN_WHITE_SPACE:
				__tjson_append_char(&token_data, &c);
				(*token)->data = token_data;
				(*token)->type = TJSON_TOKEN_SP_CHAR;
				(*token)->line = line_number;
				return 0;
			case TJSON_IN_STRING:
				__tjson_append_char(&token_data, &c);
				break;
			case TJSON_IN_ESCAPE_CHAR:
				return line_number;
			case TJSON_IN_IDENTIFIER:
				if (__tjson_is_numeric(token_data))
					token_type = TJSON_TOKEN_NUMBER;
				else if (__tjson_is_keyword(token_data))
					token_type = TJSON_TOKEN_KEYWORD;
				(*token)->data = token_data;
				(*token)->type = token_type;
				(*token)->line = line_number;
				return 0;
			default:
				break;
			}
			break;
		default:
			switch (status) {
			case TJSON_IN_STRING:
				break;
			case TJSON_IN_ESCAPE_CHAR:
				switch (c) {
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					status = TJSON_IN_STRING;
					break;				
				default:
					return line_number;
				}
				break;
			case TJSON_IN_WHITE_SPACE:
				if (token_data != NULL) free(token_data);
				token_data = NULL;				
				status = TJSON_IN_IDENTIFIER;
				token_type = TJSON_TOKEN_IDENTIFIER;				
				break;
			default:
				break;
			}
			__tjson_append_char(&token_data, &c);
			break;
		}		
	}

	if (*token != NULL) free(*token);
	*token = NULL;
	
	return 0;
}

int __tjson_new_lex(char **arrow, TTOKEN_t **token)
{
	//int state;
	//state = 0;

	if (*token != NULL && (*token)->data != NULL) free((*token)->data);
	if (*token != NULL) free(*token);
	*token = NULL;
	
	*token = (TTOKEN_t *)malloc(sizeof(TTOKEN_t) * 1);
	(*token)->data = NULL;
	(*token)->line = 0;
	(*token)->type = TJSON_TOKEN_IDENTIFIER;

	char tilt = '*';
	char c;	
	while ((c = *(*arrow)++) != '\0') {
		if (__is_white_space(c)) {
			__tjson_append_char(&(*token)->data, &c);
		}else {
			__tjson_append_char(&(*token)->data, &tilt);
		}
	}
	return 0;

	if (*token != NULL) free(*token);
	*token = NULL;

	printf("\n");
	
	return 0;
}

int tjson_parse(TJSON_t **json, char **stream)
{
	if (*stream == NULL) return -1;

	TTOKEN_t *token;
	int line_number;
	int i;
	char *str_type;
	char *arrow;
	
	token = NULL;
	line_number = 1;
	i = 0;
	str_type = NULL;
	arrow = NULL;

	*json = NULL;

	char *pre_processed_stream;
	pre_processed_stream = NULL;

	
	__tjson_pre_process(stream, &pre_processed_stream);

	printf("number\ttype\t\t\t\tvalue\n");
	printf("------------------------------------------------\n");

	arrow = pre_processed_stream;
	while (*arrow != '\0') {

		if (__tjson_new_lex(&arrow, &token) > 0) return line_number;	
		if (token == NULL || token->data == NULL) break;
		
		line_number = token->line;
		
		switch (token->type) {
		case TJSON_TOKEN_SP_CHAR: 
			str_type = "TJSON_TOKEN_SP_CHAR";
			break;
		case TJSON_TOKEN_STRING: 
			str_type = "TJSON_TOKEN_STRING";
			break;
		case TJSON_TOKEN_IDENTIFIER: 
			str_type = "TJSON_TOKEN_IDENTIFIER";
			break;
		case TJSON_TOKEN_NUMBER: 
			str_type = "TJSON_TOKEN_NUMBER";
			break;
		case TJSON_TOKEN_KEYWORD: 
			str_type = "TJSON_TOKEN_KEYWORD";
			break;
		default:
			str_type = "!!!!ERROR!!!!";
			break;
		}
		
		printf("%d\t%s\t\t%s\n", ++i, str_type, token->data);
	}

	if (pre_processed_stream != NULL) free(pre_processed_stream);
	return 0;
}


/*int tjson_parse(TJSON_t **json, const char *stream)
0{
	if (stream == NULL) return -1;

	TQUEUE_t *front, *rear, *token;
	TJSON_t *current, *new;
	char *first, *second;
	TSTACK_t *stack;
	double v_number;
	bool v_bool;
	int line_number;
	
	front = rear = token = NULL;
	current = new = NULL;	
	first = second = NULL;
	stack = NULL;
	v_number = 0;
	v_bool = false;
	line_number = 0;


	if ((line_number = __tjson_lex(&front, &rear, stream)) > 0)
		return line_number;
	
	current = *json;

	__tqueue_print(&front, &rear);
	return 0;
	
	while (front != NULL) {
				
		token = __tqueue_dequeue(&front, &rear);
		if (token == NULL || token->data == NULL)
			return line_number;
		line_number = token->line;

		switch (token->type) {
		case TJSON_TOKEN_SP_CHAR: 
			if (strcmp(token->data, "{") == 0) {
				if (first == NULL && current->v_type == TJSON_OBJECT) {
					return line_number;
				}
				new = tjson_create_node_object(first);
				if (new == NULL)
					return line_number;
				tjson_push(&current, new);
				if (first != NULL) free(first);
				first = NULL;
				__tstack_push(&stack, current);
				current = new;
			}else if (strcmp(token->data, "[") == 0) {
				if (first == NULL && current->v_type == TJSON_OBJECT) {
					return line_number;
				}
				new = tjson_create_node_array(first);
				if (new == NULL)
					return line_number;
				tjson_push(&current, new);
				__tstack_push(&stack, current);
				if (first != NULL) free(first);
				first = NULL;
				current = new;
			}else if (strcmp(token->data, "}") == 0) {
				if (current->v_type != TJSON_OBJECT)
					return line_number;
				current = __tstack_pop(&stack);
			}else if (strcmp(token->data, "]") == 0) {
				if (current->v_type != TJSON_ARRAY)
					return line_number;
				current = __tstack_pop(&stack);	 
			}else if (strcmp(token->data, ":") == 0) {
				
			}else if (strcmp(token->data, ",") == 0) {
				
			}else {
				return line_number;
			}		
			break;
		case TJSON_TOKEN_STRING: 
			if (first == NULL && current->v_type == TJSON_OBJECT) {
				first = (char *)malloc(sizeof(char) * strlen(token->data));
				if (first != NULL) {
					strncpy(first, token->data, strlen(token->data));
				}
			}else {
				new = tjson_create_node_string(first, token->data);
				if (new == NULL) {
					return line_number;
				}
				tjson_push(&current, new);
				if (first != NULL) free(first);
				first = NULL;
			}
			break;
		case TJSON_TOKEN_IDENTIFIER: 
			return line_number;
		case TJSON_TOKEN_NUMBER: 
			if (first == NULL && current->v_type == TJSON_OBJECT) {
				first = (char *)malloc(sizeof(char) * strlen(token->data));
				if (first != NULL) {
					strncpy(first, token->data, strlen(token->data));
				}
			}else {
				v_number = strtod(token->data, NULL);
				new = tjson_create_node_number(first, v_number);
				if (new == NULL) {
					return line_number;
				}
				tjson_push(&current, new);
				if (first != NULL) free(first);
				first = NULL;
			}
			break;
		case TJSON_TOKEN_KEYWORD: 
			if (first == NULL && current->v_type == TJSON_OBJECT) {
				first = (char *)malloc(sizeof(char) * strlen(token->data));
				if (first != NULL) {
					strncpy(first, token->data, strlen(token->data));
				}
			}else {
				if (strcmp(token->data, "null") == 0) {
					new = tjson_create_node_null(first);
				}else if (strcmp(token->data, "true") == 0
					 || strcmp(token->data, "false") == 0) {
					v_bool = true ? strcmp(token->data, "true") == 0 : false;
					new = tjson_create_node_bool(first, v_bool);
				}else {					
					return line_number;
				}
				if (new == NULL) return line_number;
				tjson_push(&current, new);				
				if (first != NULL) free(first);
				first = NULL;
			}
			break;
		default:
			return line_number;
		}

		if (token != NULL && token->data != NULL) free(token->data);
		if (token != NULL) free(token);
		token = NULL;
	}

	if (front != NULL) free(front);
	if (rear != NULL) free(rear);
	if (token != NULL) free(token);

	if (stack != NULL) {
		free(stack);
		return line_number;
	}

	return 0;
}*/

#endif  //__TANTO_H
