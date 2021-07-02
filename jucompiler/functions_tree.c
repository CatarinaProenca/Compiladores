#include <stdlib.h>
#include "functions_tree.h"

node* add_node(char* value, node* child, char* type, int linha, int coluna){
	node* new_node = (node*)malloc(sizeof(node));

	new_node->value = value;
	new_node->type = type;
	new_node->child = child;
	new_node->brother = NULL;
	new_node->to_annote = 0;
	new_node->linha = linha;
	new_node->coluna = coluna;

	return new_node;
}

int count_brothers(node* root){
	int count = 0;
	node* aux_node = root;

	while(aux_node!=NULL){
		aux_node = aux_node->brother;
		count+=1;
	}
	return count;
}

char* type_to_string(int act_type){
	if(act_type == 0){
		return "Bool";
	}

	else if(act_type == 1){
		return "Int";
	}

	else{
		return "Double";
	}
}


void add_brother(node* node_f, node* brother){

	if((node_f==NULL)||(brother==NULL))
		return;

	node* aux_node = node_f;

	while(aux_node->brother!=NULL)
		aux_node = aux_node->brother;

	aux_node->brother=brother;
}

void print_tree(node* root, int prof){
	int i;

	if(root==NULL){
		free(root);
		return;
	}

	if(root->type!=NULL){
		i=0;
		while(i<prof){
			printf("..");
			i+=1;
		}


		printf("%s",root->type);
		if(root->value!=NULL){
			printf("(%s)",root->value);
			if(root->note != NULL){
				printf(" - %s",root->note);
			}
			if(root->param_note != NULL){
				printf("%s",root->param_note);
			}
			printf("\n");
		}else{
			if(root->note != NULL){
				printf(" - %s",root->note);
			}
			if(root->param_note != NULL){
				printf("(%s)",root->param_note);
			}
			printf("\n");
		}
	}

	print_tree(root->child, prof+1);
	print_tree(root->brother, prof);


}