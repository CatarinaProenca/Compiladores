#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {CLASS_T, METHOD_DEC, FIELD_DEC, VAR_DEC, RETURN_T} TABLE_TYPE;

typedef struct node{
   char *value;
   char *type;
   struct node *child;
   struct node *brother;

   int to_annote;

   char *note;
   char *param_note;

   int linha;
   int coluna;
}node;

typedef struct token{
  char *value;
  int linha, coluna;
}token;

typedef struct param_node *node_param;
typedef struct param_node{
  char* param_value;
  char* type;

  int linha;
  int coluna;

  node_param next;
}param_node;

typedef struct scope_node{
  char* type;
  node_param param_list;
  char* return_type;
}scope_node;

typedef struct node_error *list_error;
typedef struct node_error{
  char* error;
  list_error next;
}error_node;
list_error erros;

typedef struct symt_node *node_symt;
typedef struct symt_node{
  char* table_scope;                           
  char* table_name;                             // Id of table (Method id /Field id, Class id)
  TABLE_TYPE table_type;

  char* return_type;                            // Type of return value (method) / token type (field dec)
  node_param param_list;

  int param;

  node_symt child;                              // Pointer to elements of same local table
  node_symt next;                               // Pointer to other tables
}symt_node;

node_symt table;        // Table BeginPoint
node_symt endpoint;
int flag_ambiguous;

node_symt create_node(char* table_scope, char* table_name, TABLE_TYPE table_type, char* return_type);
void sym_table();
char* insert_node(node_symt node, char* scope);
void print_local_table(node_symt temp);
void print_table_element(node_symt temp);
void print_sym_table();
int insert_node_local(node_symt node, node_symt root);
void add_parameter(node_symt node, char* param_value, char* type);
void add_parameter_scope(scope_node *node, char* param_value, char* type);
char* search_var(node* nd, scope_node* scope);
char* search_var_field(node* nd, scope_node* scope);
void add_note(node* nd, char* note);

void annotate_tree(node* root);
void annotate_method_dec(node* nd, scope_node * scope);
void annotate_statement(node* nd, scope_node * scope);
void ann_call_id(node* nd,scope_node *scope);

list_error create_list();
void add_error(char* str);
void print_erros();

void freeParamList(param_node *node);
void freeTree(node *node);
void freeSymbols(node_symt table);
void freeErrorList(error_node *node);