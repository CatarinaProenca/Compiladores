#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"


node* node_aux;
node* node_aux1;
node* node_aux2;
node* node_aux3;

int act_type;

int yylex(void);
void yyerror(char *s);
node* add_node(char* value, node* child, char* type, int linha, int coluna);
int count_brothers(node* root);
char* type_to_string(int act_type);
void add_brother(node* node_f, node* brother);
void print_tree(node* root, int prof);