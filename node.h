#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Node{
	char *type;
	char *data;
	int cap, childCnt, lineNum;
	struct Node** childs;
} Node;

Node* Groot = NULL;

Node* new_node(int line, char *type, char *data, int childCnt, ...){
	Node *now = (Node*)malloc(sizeof(Node));
	now->lineNum;
	now->data = strdup(data);
	now->type = strdup(type);
	now->childCnt = now->cap = childCnt;
	now->childs = (Node**)malloc(sizeof(Node*) * childCnt);
	va_list ap;
	va_start(ap, childCnt);
	int i;
	for(i = 0; i < childCnt; ++i) now->childs[i] = va_arg(ap, Node*);
	va_end(ap);
	return now;
}

void print_ast(Node *now, int dep){
	int i = 0;
	for(i = 0; i < 2 * dep; ++i) printf("-");
	printf("|%s (%s)\n", now->type, now->data);
	for(i = 0; i < now->childCnt; ++i){
		if(now->childs[i]) print_ast(now->childs[i], dep + 1);
	}
}	
