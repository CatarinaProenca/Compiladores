#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table.h"

void check_program(node* node);
void check_field_dec(node *node, char* scope);
void check_method_dec(node *node, char* scope);
char* search_var(node* nd, scope_node *scope);
char* search_var_method(node* nd, scope_node *scope);
int search_assign(node* temp,scope_node *scope);
char* search_var_method_various(node *nd,scope_node *scope);
char* SAD_error(node *nd, char* str);
char* SIR_error(node *nd);
char* CFS_error(node *nd, char* str);
char* OCBA1_error(node *nd, char* op, char* str);
char* OCBA2_error(node *nd, char* op, char* str1, char* str2);
char* ITS_error(node *nd, char* str1, char* str2);
char* RMIA_error(node *nd, char* str);
char* NOB_error(node *nd, char* value);
char* return_col_line(int line, int col);
char* return_operator(char* type);
char* return_type(char* type);
token* create_token(char *value, int linha, int coluna);
