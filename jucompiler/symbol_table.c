#include "symbol_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern node* root;

void sym_table(node* nd){
		node* temp = nd->child;
		table = create_node("GLOBAL", temp->value, CLASS_T,"");
		endpoint = table;
}

void add_parameter(node_symt node, char* param_value, char* type){
	node_param new = (node_param)malloc(sizeof(param_node));

	new->param_value = (char*)strdup(param_value);
	new->next = NULL;
	node_param temp = node->param_list;

	if(strcmp(type,"stringArray")==0){
		strcpy(type,"String[]");
	}
	if(strcmp(type,"bool")==0){
		strcpy(type,"boolean");
	}

	new->type = (char*)strdup(type);

	if(temp == NULL){
		node->param_list = new;
		return;
	}

	while(temp->next != NULL){
		temp = temp->next;
	}

	temp->next = new;

}

void add_parameter_scope(scope_node *node, char* param_value, char* type){
	node_param new = (node_param)malloc(sizeof(param_node));

	new->param_value = (char*)strdup(param_value);
	new->next = NULL;
	node_param temp = node->param_list;

	if(strcmp(type,"stringArray")==0){
		strcpy(type,"String[]");
	}
	if(strcmp(type,"bool")==0){
		strcpy(type,"boolean");
	}

	new->type = (char*)strdup(type);

	if(temp == NULL){
		node->param_list = new;
		return;
	}

	while(temp->next != NULL){
		temp = temp->next;
	}

	temp->next = new;
}

list_error create_list(){
	list_error aux;
	aux = (list_error) malloc(sizeof(error_node));
	if(aux!= NULL){
	  aux->error = strdup("");
	  aux->next = NULL;
	}
	return aux;
}

void add_error(char* str){
	list_error new = (list_error)malloc(sizeof(error_node));
	list_error temp =  erros;

	new->error = (char*)strdup(str);
	new->next = NULL;

	if(temp == NULL){
		temp = new;
		return;
	}

	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = new;
}


char* insert_node(node_symt nd, char* scope){ 						// NULL - sucesso | !NULL - error

	if(strcmp(scope,"GLOBAL")==0){
		node_symt temp = table;

		if(nd->table_type == FIELD_DEC ){
			while(temp != NULL){
				if(temp->table_type == FIELD_DEC){
					if(strcmp(temp->table_name, nd->table_name)==0){
						return "erro";
					}
				}
				temp = temp->next;
			}
		}


		if(nd->table_type == METHOD_DEC){
			while(temp != NULL){
				int flag_params = 0;
				int total_params1 = 0;
				int total_params2 = 0;

				if(temp->table_type == METHOD_DEC){
					if(strcmp(temp->table_name, nd->table_name)==0){
						node_param tmp = temp->param_list;
						node_param tmp1 = nd->param_list;

						while(tmp != NULL){
							tmp = tmp->next;
							total_params1 += 1;
						}

						while(tmp1 != NULL){
							tmp1 = tmp1->next;
							total_params2 += 1;
						}

						if(total_params1 == total_params2){
							tmp = temp->param_list;
							tmp1 = nd->param_list;

							while(tmp != NULL){
								if(tmp1 != NULL){
									if(strcmp(tmp->type, tmp1->type)==0){
										flag_params += 1;
									}
									tmp1 = tmp1->next;
								}

								tmp = tmp->next;
							}


							if(flag_params == total_params1){
								node_param tmp = temp->param_list;
								char* str3 = (char *) malloc(1000*sizeof(char));
								char* str_tmp;
								strcpy(str3, temp->table_name);
								strcat(str3,"(");
								while(tmp != NULL){
									str_tmp = (char*)strdup(tmp->type);
									strcat(str3,str_tmp);
									if(tmp->next != NULL)
										strcat(str3,",");
									tmp = tmp->next;
								}
								if(tmp != NULL){
									str_tmp = (char*)strdup(tmp->type);
									strcat(str3,str_tmp);
								}
								strcat(str3,")");

								return str3;
						}
						}
					}
				}
				temp = temp->next;
			}

		}

		endpoint->next = nd;
		endpoint = nd;
	}
	return NULL;
}


int insert_node_local(node_symt nd, node_symt root){
	node_symt temp = root->child;

	while(temp->child != NULL){
		if(strcmp(temp->table_name,nd->table_name)==0){
			// repetido - ERRO
			return 0;
		}
		temp = temp->child;
	}

	if(strcmp(temp->table_name,nd->table_name)==0){
		// repetido - ERRO
		return 0;
	}

	temp->child = nd;
	return 1;
}

node_symt create_node(char* table_scope, char* table_name, TABLE_TYPE table_type, char* return_type){
	// Creates a node with: Scope, Name, T_Type, Return Type, Param Type

	if(strcmp(table_name,"_") == 0){
		return NULL;
	}


	node_symt new = (node_symt)malloc(sizeof(symt_node));

	new->table_scope = (char*)strdup(table_scope);

	new->table_name = (char*)malloc(5000*sizeof(char*));
	new->table_name = (char*)strdup(table_name);

	new->table_type = table_type;

	if(strcmp(return_type,"stringArray")==0){
		strcpy(return_type,"String[]");
	}
	if(strcmp(return_type,"bool")==0){
		strcpy(return_type,"boolean");
	}

	new->return_type = (char*)strdup(return_type);

	new->param_list = NULL;
	new->param = 0;

	new->child = NULL;
	new->next = NULL;

	return new;
}

void print_table_element(node_symt temp){

	if(temp->table_type == METHOD_DEC){
		printf("%s\t(",temp->table_name);
		node_param tmp = temp->param_list;

		if(tmp!=NULL){
			while(tmp->next != NULL){
				printf("%s,",tmp->type);
				tmp = tmp->next;
			}
			printf("%s)\t%s\n", tmp->type, temp->return_type);
		}

		else{
			printf(")\t%s\n", temp->return_type);
		}
	}

	else if(temp->table_type == FIELD_DEC){
		printf("%s\t\t%s\n",temp->table_name, temp->return_type);
	}

	else if(temp->table_type == VAR_DEC){
		printf("%s\t\t%s", temp->table_name, temp->return_type);

		if(temp->param){
			printf("\tparam");
		}
		printf("\n");
	}

	else if(temp->table_type == RETURN_T){
		printf("%s\t\t%s\n",temp->table_name,temp->return_type);
	}

}

void print_local_table(node_symt temp){
	node_symt tmp = temp->child;

	printf("===== Method %s(",temp->table_name);
	node_param tmp_param = temp->param_list;

	if(tmp_param!=NULL){
		while(tmp_param->next != NULL){
			printf("%s,",tmp_param->type);
			tmp_param = tmp_param->next;
		}
		printf("%s) Symbol Table =====\n", tmp_param->type);
	}

	else{
		printf(") Symbol Table =====\n");
	}

	while(tmp != NULL){
		print_table_element(tmp);
		tmp = tmp->child;
	}
	printf("\n");
}

void print_sym_table(){
	node_symt temp = table;

	printf("===== Class %s Symbol Table =====\n", temp->table_name);

	while(temp != NULL){
		print_table_element(temp);
		temp = temp->next;
	}
	printf("\n");

	temp = table;
	//imprime filhos de cada no da tabela global
	while(temp != NULL){
		if(temp->table_type == METHOD_DEC){
			// tabela local
			print_local_table(temp);
		}
		temp = temp->next;
	}
}

void print_erros(){
	list_error temp = erros;

	while(temp!=NULL){
		printf("%s", temp->error);
		temp = temp->next;
	}
}


char* search_var_field(node* nd, scope_node* scope){
	node_symt tmp = table;

	while(tmp != NULL){
		if(tmp->table_type == FIELD_DEC){
			if(strcmp(tmp->table_name,nd->value)==0)
				return tmp->return_type;
		}
		tmp = tmp->next;
	}
	return NULL;
}

char* search_var(node* nd, scope_node* scope){
	node_symt tmp = table;

	while(tmp != NULL){
		if(strcmp(tmp->table_name,scope->type) == 0 && tmp->table_type == METHOD_DEC){

			node_param param_t = tmp->param_list;
			node_param param_scope = scope->param_list;

			int total_params1 = 0;
			int total_params2 = 0;
			int flag_total = 0;


			while(param_t != NULL){
				param_t = param_t->next;
				total_params1 += 1;
			}

			while(param_scope != NULL){
				param_scope = param_scope->next;
				total_params2 += 1;
			}

			if(total_params1 == total_params2){
				param_t = tmp->param_list;
				param_scope = scope->param_list;

				while(param_t != NULL){
					if(param_scope != NULL){
						if(strcmp(param_t->type, param_scope->type)==0){
							flag_total += 1;
						}
						param_scope = param_scope->next;
					}

					param_t = param_t->next;
				}

				if(flag_total == total_params1){
					node_symt tmp1 = tmp->child;
					while(tmp1 != NULL){
						if(strcmp(tmp1->table_name,nd->value) == 0 && tmp1->table_type == VAR_DEC){
							return tmp1->return_type;
						}
						tmp1 = tmp1->child;
					}
					//não é local
					break;
				}
			}

		}

		tmp = tmp->next;
	}

	return NULL;
}

char* search_var_method(node *nd,scope_node *scope){
	node_symt tmp = table;
	while(tmp != NULL){
		if(strcmp(tmp->table_name,nd->value) == 0 && tmp->table_type == METHOD_DEC){

			node_param param_t = tmp->param_list;
			node *temp = nd->brother;

			int total_params1 = 0;
			int total_params2 = 0;
			int flag_total = 0;

			while(param_t != NULL){
				param_t = param_t->next;
				total_params1 += 1;
			}

			while(temp != NULL){
				temp = temp->brother;
				total_params2 += 1;
			}

			if(total_params1 == total_params2){
				param_t = tmp->param_list;
				temp = nd->brother;

				while(param_t != NULL){
					if(temp != NULL){
						if((strcmp(temp->note,param_t->type)==0)){
							flag_total += 1;
							temp = temp->brother;
						}
					}
					param_t = param_t->next;
				}

				if(flag_total == total_params1){
					node_param param_t = tmp->param_list;
					char* str3 = (char *) malloc(1000*sizeof(char));
					char* str_tmp;
					strcpy(str3,"(");
					while(param_t != NULL){
						str_tmp = (char*)strdup(param_t->type);
						strcat(str3,str_tmp);
						if(param_t->next != NULL)
							strcat(str3,",");
						param_t = param_t->next;
					}
					if(param_t != NULL){
						str_tmp = (char*)strdup(param_t->type);
						strcat(str3,str_tmp);
					}
					strcat(str3,")");
					nd->param_note = (char*)strdup(str3);

					return tmp->return_type;
				}
			}
		}
		tmp = tmp->next;
	}
	return NULL;
}

char* search_var_method_various(node *nd,scope_node *scope){
	node_symt tmp = table;
	node_symt tmp_aux = (node_symt)malloc(sizeof(symt_node));;
	int n_global = 0;
	flag_ambiguous=0;

	while(tmp != NULL){
		if(strcmp(tmp->table_name,nd->value) == 0 && tmp->table_type == METHOD_DEC){

			node_param param_t = tmp->param_list;
			node *temp = nd->brother;

			int total_params1 = 0;
			int total_params2 = 0;
			int flag_total_various = 0;

			while(param_t != NULL){
				param_t = param_t->next;
				total_params1 += 1;
			}

			while(temp != NULL){
				temp = temp->brother;
				total_params2 += 1;
			}

			if(total_params1 == total_params2){
				param_t = tmp->param_list;
				temp = nd->brother;

				int n = 0;
				while(param_t != NULL){
					if(temp != NULL){
						if((strcmp(temp->note,param_t->type)==0)){
							n += 1;
							flag_total_various += 1;
						}
						else{
							if(strcmp(temp->note, "double")==0){
								if(strcmp(param_t->type, "double")==0){
									flag_total_various += 1;
								}
							}
							else if((strcmp(temp->note, "int")==0)){
								if(strcmp(param_t->type, "int")==0 || strcmp(param_t->type, "double")==0){
									flag_total_various += 1;
								}
							}
						}

						temp = temp->brother;
					}
					param_t = param_t->next;
				}
				if(flag_total_various == total_params1){
					flag_ambiguous +=1;
					if(n >= n_global){
						tmp_aux = create_node(tmp->table_scope,tmp->table_name,tmp->table_type,tmp->return_type);
						tmp_aux->param_list = tmp->param_list;

						n_global = n;
					}
				}
			}
		}
		tmp = tmp->next;
	}

	if(tmp_aux != NULL){
		node_param param_t = tmp_aux->param_list;
		char* str3 = (char *) malloc(1000*sizeof(char));
		char* str_tmp;
		strcpy(str3,"(");
		while(param_t != NULL){
			str_tmp = (char*)strdup(param_t->type);
			strcat(str3,str_tmp);

			if(param_t->next != NULL)
				strcat(str3,",");
			param_t = param_t->next;
		}
		if(param_t != NULL){
			str_tmp = (char*)strdup(param_t->type);
			strcat(str3,str_tmp);
		}
		strcat(str3,")");

		nd->param_note = (char*)strdup(str3);
		
		return tmp_aux->return_type;
	}

	return NULL;
}

void freeParamList(param_node *node){
    if (node->next != NULL)
        freeParamList(node->next);

    // free
    free(node->param_value);
    free(node->type);
    free(node);
}

void freeErrorList(error_node *node){
    if (node->next != NULL)
        freeErrorList(node->next);

    // free
    free(node->error);
    free(node);
}

void freeTree(node *node){
    if(node==NULL)
        return;
        
    // keep going
    if (node->child != NULL)
        freeTree(node->child);
    if (node->brother != NULL)
        freeTree(node->brother);
   
    free(node);
}


void freeSymbols(node_symt s){
    // keep going
    if (s->next != NULL)
        freeSymbols(s->next);

    // free
    if(s->table_type == 1){
        if(s->table_scope != NULL)
            free(s->table_scope); 
        s->table_scope = NULL;
        if(s->table_name != NULL)
            free(s->table_name);
        s->table_name = NULL;
        if(s->return_type != NULL)
            free(s->return_type);
        s->return_type = NULL;
        if(s->param_list != NULL)
            freeParamList(s->param_list);
        s->param_list = NULL;
    }
    else if (s->table_type == 2){
        free(s->table_name);
        free(s->return_type);
    }
    free(s);

}

