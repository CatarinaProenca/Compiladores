#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantics.h"
#include <ctype.h>


token* create_token(char *value, int linha, int coluna){
    token *new = (token*)malloc(sizeof(token));
    if(value != NULL){
        new->value = (char*)strdup(value);
    }
    else{
        new->value = NULL;
    }
    new->linha = linha;
    new->coluna = coluna;
    return new;
}

void check_program(node *nd){
	node *temp = nd->child;

	erros = create_list();
	while(temp != NULL){
		if(strcmp(temp->type,"MethodDecl") == 0){
			check_method_dec(temp,"GLOBAL");
		}
    	else if(strcmp(temp->type,"FieldDecl") == 0){
			check_field_dec(temp,"GLOBAL");
		}
		temp = temp->brother;
	}
}

void check_method_dec(node*nd, char* scope){
	node* temp = nd;
	char* name = (char*)strdup(temp->child->child->brother->value);
	char* name1 = (char*)strdup(name);
	char* type = (char*)strdup(temp->child->child->type);
	char* str;
	int res;

	type[0] = tolower(type[0]);
	char* type1 = (char*)strdup(type);

	node_symt new = create_node(scope,name,METHOD_DEC,type);

	node_symt return_node = create_node(name1,"return",RETURN_T,type1);

	if(new != NULL){
		new->child = return_node;

		temp = nd->child->child->brother->brother->child;

		while(temp != NULL){
			if(temp->child->brother != NULL){
				name = (char*)strdup(temp->child->brother->value);
				type = (char*)strdup(temp->child->type);

				type[0] = tolower(type[0]);
				add_parameter(new,name,type);

				node_symt node_param = create_node(name1,name,VAR_DEC,type);

				if(node_param != NULL){
					node_param->param = 1;

					res = insert_node_local(node_param,new);
					if(res == 0){
						add_error(SAD_error(temp->child->brother,temp->child->brother->value));
					}
				}

				else{
					add_error(SIR_error(temp->child->brother));
				}
			}else{
				strcpy(name,"NULL");
				type = (char*)strdup(temp->child->type);
				type[0] = tolower(type[0]);
				add_parameter(new,name,type);
			}
			temp = temp->brother;
		}

		str = insert_node(new,scope);

		if(str != NULL){
      		nd->to_annote = 1;
			add_error(SAD_error(nd->child->child->brother,str));
		}
	}
  else{
    add_error(SIR_error(temp->child->child->brother));
  }
}

void check_field_dec(node*nd, char* scope){
	char* res = (char*)malloc(5000*sizeof(char));

	node *temp = nd;

	char* name;
	char* type;

	type = (char*)strdup(temp->child->type);
	name = (char*)strdup(temp->child->brother->value);

	type[0] = tolower(type[0]);
	node_symt new = create_node(scope,name,FIELD_DEC,type);

	if(new != NULL){
		res = insert_node(new,scope);

		if(res!=NULL){
			add_error(SAD_error(temp->child->brother,temp->child->brother->value));
		}
	}
	else{
		add_error(SIR_error(temp->child->brother));
	}
}


int check_var(node* nd, scope_node* scope){
	node_symt tmp = table;
	node* temp = nd;

	char* type = (char*)strdup(temp->child->type);
	type[0] = tolower(type[0]);

	char* name = (char*)strdup(temp->child->brother->value);
	node_symt node_param1 = create_node(scope->type,name,VAR_DEC,type);

	if(node_param1 != NULL){
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

					int flag = 0;
					if(flag_total == total_params1){
						node_symt tmp1 = tmp->child;
						while(tmp1 != NULL){
							if(strcmp(tmp1->table_name,nd->child->brother->value) == 0 && tmp1->table_type == VAR_DEC){
								flag = 1;
							}
							tmp1 = tmp1->child;
						}
						if(flag != 0)
							return 1;
						else if(flag == 0)
							insert_node_local(node_param1,tmp);
					}
				}
			}
			tmp = tmp->next;
		}
	}
	else{
		add_error(SIR_error(temp->child->brother));
	}

	return 0;
}


// Anotated Tree Related Functions

void annotate_tree(node* root){
	node* temp = root->child;

	while(temp != NULL){
		// analisar a arvore recursivamente, para cada caso expecifico do Program

		if(strcmp(temp->type,"MethodDecl") == 0 && temp->to_annote == 0){
			scope_node *scope = (scope_node*)malloc(sizeof(scope_node));
			scope->type = (char*)strdup(temp->child->child->brother->value);
			scope->return_type = (char*)strdup(temp->child->child->type);

			node* tmp = temp->child->child->brother->brother->child;
			char* type;
			char* name;

			while(tmp != NULL){
				if(tmp->child->brother != NULL){
					name = (char*)strdup(tmp->child->brother->value);
					type = (char*)strdup(tmp->child->type);
					type[0] = tolower(type[0]);
					add_parameter_scope(scope,name,type);
				}
				else{
					strcpy(name,"NULL");
					type = (char*)strdup(tmp->child->type);
					type[0] = tolower(type[0]);
					add_parameter_scope(scope,name,type);
				}
				tmp = tmp->brother;
			}

			annotate_method_dec(temp,scope);
		
		}

		temp = temp->brother;
	}
}

void annotate_method_dec(node *nd, scope_node *scope){

	node* tmp = nd->child->brother->child;
	while(tmp != NULL){
		if(strcmp(tmp->type,"VarDecl")!=0){
			annotate_statement(tmp,scope);
		}
		else{
			int res = check_var(tmp, scope);
			if(res == 1){
				add_error(SAD_error(tmp->child->brother,tmp->child->brother->value));
			}

		}
		tmp = tmp->brother;
	}
}


void annotate_statement(node *nd, scope_node *scope){
	if(strcmp(nd->type,"RealLit") == 0){
		add_note(nd,"double");
	}
	else if(strcmp(nd->type,"BoolLit") == 0){
		add_note(nd,"boolean");
	}
	else if(strcmp(nd->type,"DecLit") == 0){
		add_note(nd,"int");
		int i;

		char* aux = (char*)malloc(5000*sizeof(char));
	    char* result = (char*)malloc(5000*sizeof(char));

	    for(i = 0; i<= strlen(nd->value);i++){
	      if(nd->value[i]!='_'){
	        strncat(result, &nd->value[i],1);
	      }
	    }

	    sprintf(aux, "%d", 2147483647);
	    if(strcmp(aux,result)!=0){
	      if(atoi(result) <= -2147483648 || atoi(result) >= 2147483647){
	        add_error(NOB_error(nd,nd->value));
	      }
	    }
	}
	else if(strcmp(nd->type,"StringArray") == 0){
		add_note(nd,"String[]");
	}
	else if(strcmp(nd->type,"StrLit") == 0){
		add_note(nd,"String");
	}
	else if(strcmp(nd->type,"Length") == 0){
		annotate_statement(nd->child,scope);
		add_note(nd,"int");

	    if(nd->child->note == NULL){
	      add_error(OCBA1_error(nd,return_operator(nd->type),"none"));
	    }

		else if(strcmp(nd->child->note,"String[]")!=0){
			add_error(OCBA1_error(nd,return_operator(nd->type),nd->child->note));
		}
	}
	else if(strcmp(nd->type,"Id") == 0){
		char* var_type = search_var(nd,scope);
		if(var_type != NULL){
			// existe e podemos anotar
			//printf("note - %s\n",v_type);
			add_note(nd,var_type);
		}else {
			char* var_type1 = search_var_field(nd, scope);
			if(var_type1 != NULL)
				add_note(nd,var_type1);
			else{
			// aqui - erro undefined variable
				add_note(nd,"undef");
				add_error(CFS_error(nd,nd->value));
			}
		}
	}
	// ADD SUB MUL DIV MOD
	if(strcmp(nd->type,"Add") == 0 || strcmp(nd->type,"Sub") == 0 || strcmp(nd->type,"Mul") == 0 || strcmp(nd->type,"Div") == 0){
		// Note do Add fica o maior dos dois filhos
		annotate_statement(nd->child,scope);	//1o filho
		annotate_statement(nd->child->brother,scope); //2o filho

		if(nd->child->note == NULL || nd->child->brother->note == NULL){
			add_note(nd,"undef");
			if(nd->child->note == NULL && nd->child->brother->note != NULL)
				add_error(OCBA2_error(nd,return_operator(nd->type), "undef", nd->child->brother->note));
			else if(nd->child->brother->note == NULL && nd->child->note != NULL)
				add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, "undef"));
			else if(nd->child->brother->note == NULL && nd->child->note == NULL)
				add_error(OCBA2_error(nd,return_operator(nd->type), "undef", "undef"));

		}else if(strcmp(nd->child->note,"undef")==0 || strcmp(nd->child->brother->note,"undef")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"String[]")==0 || strcmp(nd->child->brother->note,"String[]")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"boolean")==0 || strcmp(nd->child->brother->note,"boolean")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd, return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"double")==0 || strcmp(nd->child->brother->note,"double")==0){
			add_note(nd,"double");
		}else if(strcmp(nd->child->note,"int")==0 || strcmp(nd->child->brother->note,"int")==0){
			add_note(nd,"int");
		}
	}
	else if(strcmp(nd->type,"Or") == 0 || strcmp(nd->type,"And") == 0 || strcmp(nd->type,"Eq") == 0 || strcmp(nd->type,"Ne") == 0 || strcmp(nd->type,"Lt") == 0 || strcmp(nd->type,"Gt") == 0 || strcmp(nd->type,"Le") == 0 || strcmp(nd->type,"Ge") == 0 || strcmp(nd->type,"Xor") == 0 || strcmp(nd->type,"Not") == 0){
		// sempre boolean
		annotate_statement(nd->child,scope);
		if(strcmp(nd->type,"Not") != 0){
			annotate_statement(nd->child->brother,scope);

	      	if(nd->child->brother->note == NULL && nd->child->note == NULL){
	       		add_error(OCBA2_error(nd,"return_operator(nd->type)", "none", "none"));
	       		add_note(nd,"undef");
	      	}
	      	else if(nd->child->brother->note == NULL && nd->child->note != NULL){
				add_error(OCBA2_error(nd,return_operator(nd->type),nd->child->note, "none"));
				add_note(nd,"undef");
	      	}
	      	else if(nd->child->note == NULL && nd->child->brother->note != NULL){
	        	add_error(OCBA2_error(nd,return_operator(nd->type),"none", nd->child->brother->note));
	        	add_note(nd,"undef");
	      	}
			else if((strcmp(nd->child->note, "undef")==0 || strcmp(nd->child->brother->note, "undef")==0)){
				add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
				add_note(nd,"undef");
			}
			else if((strcmp(nd->child->note, "String[]")==0 || strcmp(nd->child->brother->note, "String[]")==0)){
				if((strcmp(nd->child->note, "String[]")==0 && strcmp(nd->child->brother->note, "String[]")==0) && (strcmp(nd->type,"Eq")==0 || strcmp(nd->type,"Ne")==0))
					add_note(nd,"boolean");
				else{
					add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
					add_note(nd,"undef");
				}
			}

	     	else if(strcmp(nd->type, "Or")==0 || strcmp(nd->type,"And") == 0){
	        	if((strcmp(nd->child->note, "boolean")!=0 || strcmp(nd->child->brother->note, "boolean")!=0)){
	          		add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
	      			add_note(nd,"undef");
	      		}else{
	      			add_note(nd,"boolean");
	      		}
	      	}
	      	else if(strcmp(nd->type,"Eq")==0 || strcmp(nd->type,"Ne")==0){
	        	if((strcmp(nd->child->note, nd->child->brother->note))!=0){
	          		if((strcmp(nd->child->note,"double")==0) && (strcmp(nd->child->brother->note,"int")==0)){
	          			add_note(nd,"boolean");
	          		}
	          		else if((strcmp(nd->child->note,"int")==0) && (strcmp(nd->child->brother->note,"double")==0)){
	   					add_note(nd,"boolean");
	          		}
	          		else{
	          			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
	          			add_note(nd,"undef");
	          		}
	        	}else{
	        		add_note(nd,"boolean");
	        	}
	      	}
	      	else if(strcmp(nd->type,"Lt") == 0 || strcmp(nd->type,"Gt") == 0 || strcmp(nd->type,"Le") == 0 || strcmp(nd->type,"Ge") == 0){
	        	if((strcmp(nd->child->note, nd->child->brother->note))!=0){
	          		if((strcmp(nd->child->note,"double")==0) && (strcmp(nd->child->brother->note,"int")==0)){
	          			add_note(nd,"boolean");
	          		}
	          		else if((strcmp(nd->child->note,"int")==0) && (strcmp(nd->child->brother->note,"double")==0)){
	   					add_note(nd,"boolean");
	          		}
	          		else{
	          			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
	          			add_note(nd,"undef");
	          		}
	        	}
	        	else if((strcmp(nd->child->note,"boolean")==0) && (strcmp(nd->child->brother->note,"boolean")==0)){
	        		add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
	          		add_note(nd,"undef");
	        	}
	        	else{
	        		add_note(nd,"boolean");
	        	}
	      	}
	      	else{
	      		if((strcmp(nd->child->note, "int")==0 && strcmp(nd->child->brother->note, "int")==0)){
	      			add_note(nd,"int");
	      		}
		        else if((strcmp(nd->child->note, "boolean")!=0 || strcmp(nd->child->brother->note, "boolean")!=0)){
		          	if((strcmp(nd->child->note, nd->child->brother->note))!=0){
		            	if((strcmp(nd->child->note, "double")!=0 && strcmp(nd->child->brother->note, "int")!=0) ||(strcmp(nd->child->note, "int")!=0 && strcmp(nd->child->brother->note,"double")!=0))
		              		add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		          			add_note(nd,"undef");
		          	}else{
		          		add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		          		add_note(nd,"undef");
		          	}
		        }
		        else{
		        	add_note(nd,"boolean");
		        }
	      	}
		}

	    else if(nd->child->note == NULL){
	     	add_error(OCBA1_error(nd,return_operator(nd->type), "none"));
	     	add_note(nd,"undef");
	    }

		else if(strcmp(nd->child->note, "boolean")!=0 && (strcmp(nd->type,"Not") == 0)){
			add_error(OCBA1_error(nd,return_operator(nd->type), nd->child->note));
			add_note(nd,"undef");
		}
		else{
			add_note(nd,"boolean");
		}
	}

	else if(strcmp(nd->type,"Lshift") == 0 || strcmp(nd->type,"Rshift") == 0){
		if(nd->brother!=NULL){
			annotate_statement(nd->brother,scope);
		}
	}
	else if(strcmp(nd->type,"Mod") == 0){
		annotate_statement(nd->child,scope);
		annotate_statement(nd->child->brother,scope);

		if(nd->child->note == NULL && nd->child->brother->note != NULL){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), "none", nd->child->brother->note));
		}if(nd->child->note != NULL && nd->child->brother->note == NULL){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, "none"));
		}else if(strcmp(nd->child->note,"undef")==0 || strcmp(nd->child->brother->note,"undef")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"String[]")==0 || strcmp(nd->child->brother->note,"String[]")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"boolean")==0 || strcmp(nd->child->brother->note,"boolean")==0){
			add_note(nd,"undef");
			add_error(OCBA2_error(nd,return_operator(nd->type), nd->child->note, nd->child->brother->note));
		}else if(strcmp(nd->child->note,"double")==0 || strcmp(nd->child->brother->note,"double")==0){
			add_note(nd,"double");
		}else if(strcmp(nd->child->note,"int")==0 || strcmp(nd->child->brother->note,"int")==0){
			add_note(nd,"int");
		}
	}
	else if(strcmp(nd->type,"Minus") == 0 || strcmp(nd->type,"Plus") == 0){
		annotate_statement(nd->child,scope);
    if(nd->child->note == NULL){
      add_note(nd,"undef");
      add_error(OCBA1_error(nd,return_operator(nd->type), "none"));
    }
	else if(strcmp(nd->child->note,"boolean") != 0){
			add_note(nd,nd->child->note);
		}
    else{
			add_error(OCBA1_error(nd,return_operator(nd->type), nd->child->note));
			add_note(nd,"undef");
		}
	}

	else if(strcmp(nd->type,"Call") == 0){
		node* tmp;

		tmp = nd->child->brother;
		while(tmp != NULL){
			//printf("- %s\n",tmp->value);
			annotate_statement(tmp,scope);
			tmp = tmp->brother;
		}

 		tmp = nd->child;
		ann_call_id(tmp,scope);

		add_note(nd,nd->child->note);
		if(strcmp(nd->child->note,"undef")!=0){
			strcpy(nd->child->note,"");
		}else{
			strcpy(nd->child->param_note,"");
		}

	}
	else if(strcmp(nd->type,"Assign") == 0){
		annotate_statement(nd->child,scope);
		annotate_statement(nd->child->brother,scope);

		add_note(nd,nd->child->note);

    if(nd->child->brother->note == NULL && nd->child->note == NULL)
      add_error(OCBA2_error(nd,"=","none", "none"));
    else if(nd->child->brother->note == NULL && nd->child->note != NULL)
		  add_error(OCBA2_error(nd,"=",nd->child->note,"none"));
    else if(nd->child->note == NULL && nd->child->brother->note != NULL)
      add_error(OCBA2_error(nd,"=","none",nd->child->brother->note));
		else if(strcmp(nd->child->note, nd->child->brother->note)!=0){
			if(strcmp(nd->child->note,"double")!=0){
				add_error(OCBA2_error(nd,"=",nd->child->note, nd->child->brother->note));
			}
			else if(strcmp(nd->child->note,"double")==0){
				if(strcmp(nd->child->brother->note,"int")!=0){
					add_error(OCBA2_error(nd,"=",nd->child->note, nd->child->brother->note));
				}
			}
		}else if(strcmp(nd->child->note, nd->child->brother->note)==0 && strcmp(nd->child->note,"String[]")==0){
			add_error(OCBA2_error(nd,"=",nd->child->note, nd->child->brother->note));
		}
    else if(strcmp(nd->child->note, nd->child->brother->note)==0 && strcmp(nd->child->note,"undef")==0){
			add_error(OCBA2_error(nd,"=",nd->child->note, nd->child->brother->note));
		}
	}
	else if(strcmp(nd->type,"ParseArgs") == 0){
		annotate_statement(nd->child,scope);
		annotate_statement(nd->child->brother,scope);

		add_note(nd,"int");

		if(nd->child->note == NULL && nd->child->brother->note != NULL){
			add_error(OCBA2_error(nd,"Integer.parseInt","none", nd->child->brother->note));
		}
		else if(nd->child->note != NULL && nd->child->brother->note == NULL){
			add_error(OCBA2_error(nd,"Integer.parseInt", nd->child->note, "none"));
		}
		else if(strcmp(nd->child->note,"String[]")==0){
			if(strcmp(nd->child->brother->note,"int")!=0)
				add_error(OCBA2_error(nd,"Integer.parseInt",nd->child->note, nd->child->brother->note));
		}
		else if(strcmp(nd->child->note,"String[]")!=0){
			add_error(OCBA2_error(nd,"Integer.parseInt",nd->child->note, nd->child->brother->note));
		}
	}
	else if(strcmp(nd->type,"Print") == 0){
		annotate_statement(nd->child,scope);

    if(nd->child->note == NULL){
      add_error(ITS_error(nd->child,"none","System.out.print"));
    }

		else if(nd->child->child != NULL){
			if(strcmp(nd->child->note,"undef")==0 || strcmp(nd->child->note,"String[]")==0 || strcmp(nd->child->note,"void")==0)
				add_error(ITS_error(nd->child->child,nd->child->note,"System.out.print"));
		}else{
			if(strcmp(nd->child->note,"undef")==0 || strcmp(nd->child->note,"String[]")==0 || strcmp(nd->child->note,"void")==0)
				add_error(ITS_error(nd->child,nd->child->note,"System.out.print"));
		}
	}
	else if(strcmp(nd->type,"While") == 0){
		annotate_statement(nd->child,scope);

		if(strcmp(nd->child->brother->type,"Block")!=0){
			annotate_statement(nd->child->brother,scope);
		}
		else{
			if(nd->child->brother->child != NULL){
				node *tmp = nd->child->brother->child;

				while(tmp != NULL){
					annotate_statement(tmp,scope);
					tmp = tmp->brother;
				}
			}
		}

    	if(nd->child->note == NULL){
      		add_error(ITS_error(nd->child,"none","while"));
    	}

		else if(strcmp(nd->child->note,"boolean")!=0){
			add_error(ITS_error(nd->child,nd->child->note,"while"));
		}
	}
	else if(strcmp(nd->type,"If") == 0){
		annotate_statement(nd->child,scope);

		if(strcmp(nd->child->brother->type,"Block")!=0){
			annotate_statement(nd->child->brother,scope);
		}
		else{
			if(nd->child->brother->child != NULL){
				node *tmp = nd->child->brother->child;

				while(tmp != NULL){
					annotate_statement(tmp,scope);
					tmp = tmp->brother;
				}
			}
		}

		if(strcmp(nd->child->brother->brother->type,"Block")!=0){
			annotate_statement(nd->child->brother->brother,scope);
		}
		else{
			if(nd->child->brother->brother->child != NULL){
				node *tmp = nd->child->brother->brother->child;

				while(tmp != NULL){
					annotate_statement(tmp,scope);
					tmp = tmp->brother;
				}
			}
		}
    if(nd->child->note == NULL){
      add_error(ITS_error(nd->child,"none","if"));
    }
		else if(strcmp(nd->child->note,"boolean")!=0){
			add_error(ITS_error(nd->child,nd->child->note,"if"));
		}

	}
	else if(strcmp(nd->type,"Return") == 0){
		char* scope_type = (char*)malloc(5000*sizeof(char));

		if(nd->child != NULL){
			annotate_statement(nd->child,scope);

			scope_type = return_type(scope->return_type);

	      	if(nd->child->note == NULL){
	        	add_error(ITS_error(nd->child,"none","return"));
	      	}

			else if(strcmp(nd->child->note, scope_type)!=0){
				if(strcmp(nd->child->note,"int")!=0 && strcmp(scope_type,"double")!=0){
					add_error(ITS_error(nd->child,nd->child->note,"return"));
				}else if(strcmp(nd->child->note,"int")==0 && strcmp(scope_type,"double")!=0){
					add_error(ITS_error(nd->child,nd->child->note,"return"));
				}else if(strcmp(nd->child->note,"int")!=0 && strcmp(scope_type,"double")==0){
					add_error(ITS_error(nd->child,nd->child->note,"return"));
				}
			}
		}

		else{
			if(strcmp(scope->return_type,"Void")!=0){
				add_error(ITS_error(nd,"void","return"));
			}
		}

	}
}

void ann_call_id(node *nd, scope_node *scope){
	char* var_type = search_var_method(nd,scope);
	if(var_type != NULL){
		// existe e podemos anotar
		//printf("note - %s\n",v_type);
		add_note(nd,var_type);
	}else{
		// aqui - erro undefined variable
		var_type = search_var_method_various(nd,scope);
		if(var_type != NULL){
			// existe e podemos anotar
			add_note(nd,var_type);

			node* tmp = nd->brother;
			char* str3 = (char *) malloc(1000*sizeof(char));
			char* str_type;
			strcpy(str3, nd->value);
			strcat(str3,"(");

			while(tmp!= NULL){
				str_type= (char*)strdup(tmp->note);
				strcat(str3,str_type);
				if(tmp->brother != NULL)
					strcat(str3,",");
				tmp = tmp->brother;
			}

			if(tmp != NULL){
				str_type = (char*)strdup(tmp->note);
				strcat(str3,str_type);
			}
			strcat(str3,")");

      if(flag_ambiguous>1){
        add_error(RMIA_error(nd, str3));
    	add_note(nd,"undef");
    	}
	}
		else{
			add_note(nd,"undef");
			node* tmp = nd->brother;
			char* str3 = (char *) malloc(1000*sizeof(char));
			char* str_type;
			strcpy(str3, nd->value);
			strcat(str3,"(");

			while(tmp!= NULL){
				str_type= (char*)strdup(tmp->note);
				strcat(str3,str_type);
				if(tmp->brother != NULL)
					strcat(str3,",");
				tmp = tmp->brother;
			}

			if(tmp != NULL){
				str_type = (char*)strdup(tmp->note);
				strcat(str3,str_type);
			}
			strcat(str3,")");
			add_error(CFS_error(nd,str3));
		}
	}
}

void add_note(node *nd, char* note){
	nd->note = (char*)strdup(note);
}

char* SAD_error(node *nd, char* str){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Symbol ");
	strcat(aux, str);
	strcat(aux, " already defined\n");

	return aux;
}

char* NOB_error(node *nd, char* value){
  char* aux;
  char* v = (char*) malloc(1000*sizeof(char));

  aux = return_col_line(nd->linha,  nd->coluna);
  snprintf(v, (sizeof v)*1000, "%s: Number %s out of bounds\n",aux, value);

  return v;
}

char* SIR_error(node *nd){
	char* aux;
	aux = return_col_line(nd->linha, nd->coluna);
	strcat(aux, ": Symbol _ is reserved\n");

	return aux;
}

char* CFS_error(node *nd, char* str){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Cannot find symbol ");
	strcat(aux, str);
	strcat(aux, "\n");

	return aux;
}

char* OCBA1_error(node *nd, char* op, char* str){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Operator ");
	strcat(aux, op);
	strcat(aux, " cannot be applied to type ");
	strcat(aux,str);
	strcat(aux,"\n");

	return aux;
}

char* OCBA2_error(node *nd, char* op, char* str1, char* str2){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Operator ");
	strcat(aux, op);
	strcat(aux, " cannot be applied to types ");
	strcat(aux,str1);
	strcat(aux,", ");
	strcat(aux,str2);
	strcat(aux,"\n");

	return aux;
}

char* ITS_error(node *nd, char* str1, char* str2){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Incompatible type ");
	strcat(aux, str1);
	strcat(aux, " in ");
	strcat(aux, str2);
	strcat(aux, " statement\n");

	return aux;
}

char* RMIA_error(node *nd, char* str){
	char* aux;
	aux = return_col_line(nd->linha,  nd->coluna);
	strcat(aux, ": Reference to method ");
	strcat(aux, str);
	strcat(aux, " is ambiguous\n");

	return aux;
}

char* return_col_line(int line, int col){
	char* aux = (char*) malloc(1000*sizeof(char));
	snprintf(aux, (sizeof aux)*1000, "Line %d, col %d",line, col);
	return aux;
}

char* return_type(char* type){
	if(strcmp(type,"Int") == 0)
		return "int";
	else if (strcmp(type,"Bool") == 0)
		return "boolean";
	else if (strcmp(type,"Double") == 0)
		return "double";
	else if (strcmp(type,"StringArray") == 0)
		return "string[]";
	return "";
}

char* return_operator(char* type){
    if(strcmp(type,"Add") == 0 || strcmp(type,"Plus") == 0)
        return "+";
    else if (strcmp(type,"Sub") == 0 || strcmp(type,"Minus") == 0 )
        return "-";
    else if (strcmp(type,"Mul") == 0)
        return "*";
    else if (strcmp(type,"Div") == 0)
        return "/";
    else if (strcmp(type,"Ge") == 0)
        return ">=";
    else if (strcmp(type,"Gt") == 0)
        return ">";
    else if (strcmp(type,"Eq") == 0)
        return "==";
    else if (strcmp(type,"Assign") == 0)
        return "=";
    else if (strcmp(type,"Le") == 0)
        return "<=";
    else if (strcmp(type,"Lt") == 0)
        return "<";
    else if (strcmp(type,"Ne") == 0)
        return "!=";
    else if (strcmp(type,"Not") == 0)
        return "!";
    else if (strcmp(type,"And") == 0)
        return "&&";
    else if (strcmp(type,"Xor") == 0)
    	return "^";
    else if (strcmp(type,"Or") == 0)
        return "||";
    else if (strcmp(type,"Mod") == 0)
        return "%";
	else if (strcmp(type,"Length") == 0)
		return ".length";
    return 0;
}
