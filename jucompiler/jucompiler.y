%{
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "functions_tree.h"


 int yylex(void);
 void yyerror(char *s);
 extern int yyleng;
 extern int flag_tree;
 extern char* yytext;
 extern int line_num, col_num;

 node *root;

%}

%union{
 struct token* value;
 struct node* node;
}

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left GE GT LT LE
%left RSHIFT LSHIFT
%left PLUS MINUS
%left DIV MOD STAR
%right NOT
%left LPAR RPAR LSQ RSQ
%nonassoc IF
%nonassoc ELSE

%token<value>  ID
%token<value>  INTLIT
%token<value>  REALLIT
%token<value>  STRLIT
%token<value>  BOOLLIT
%token<value>  RESERVED
%token<value>  SEMICOLON
%token<value>  CLASS
%token<value>  RETURN
%token<value>  LSHIFT
%token<value>  RSHIFT
%token<value>  AND
%token<value>  ASSIGN
%token<value>  STAR
%token<value>  COMMA
%token<value>  DIV
%token<value>  EQ
%token<value>  GE
%token<value>  GT
%token<value>  LBRACE
%token<value>  LPAR
%token<value>  LE
%token<value>  LSQ
%token<value>  LT
%token<value>  ARROW
%token<value>  MINUS
%token<value>  NE
%token<value>  NOT
%token<value>  OR
%token<value>  XOR
%token<value>  MOD
%token<value>  PLUS
%token<value>  RBRACE
%token<value>  RPAR
%token<value>  RSQ
%token<value>  ELSE
%token<value>  IF
%token<value>  WHILE
%token<value>  VOID
%token<value>  STATIC
%token<value>  PUBLIC
%token<value>  INT
%token<value>  DOUBLE
%token<value>  BOOL
%token<value>  STRING
%token<value>  PRINT
%token<value>  PARSEINT
%token<value>  DOTLENGTH

%type<node> Program
%type<node> ProgramAux
%type<node> MethodDecl
%type<node> FieldDecl
%type<node> FieldDeclAux
%type<node> Type
%type<node> MethodHeader
%type<node> FormalParams
%type<node> FormalParamsAux
%type<node> MethodBody
%type<node> MethodBodyAux
%type<node> VarDecl
%type<node> VarDeclAux
%type<node> Statement
%type<node> AssignmentAux
%type<node> Assign
%type<node> StatementAux
%type<node> MethodInvocation
%type<node> MethodInvocationAux
%type<node> ParseArgs
%type<node> Expr


%%
Program: CLASS ID LBRACE ProgramAux RBRACE              {node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); $$ = add_node(NULL,node_aux,"Program",0,0); add_brother(node_aux,$4); root=$$;}
;


ProgramAux:                                             {$$ = NULL;}
         | MethodDecl ProgramAux                       {$$ = $1; add_brother($$,$2);}
         | FieldDecl ProgramAux                        {$$ = $1; add_brother($$,$2);}
         | SEMICOLON ProgramAux                        {$$ = $2;}
;


MethodDecl: PUBLIC STATIC MethodHeader MethodBody       {add_brother($3,$4); $$ = add_node(NULL,$3,"MethodDecl",0,0);}
;


FieldDecl: PUBLIC STATIC Type ID FieldDeclAux SEMICOLON   {node_aux = add_node($4->value,NULL,"Id",$4->linha, $4->coluna);add_brother($3,node_aux); $$ = add_node(NULL,$3,"FieldDecl",0,0); add_brother($$,$5);}
        | error SEMICOLON                                {$$ = NULL;}
;


FieldDeclAux:                                             {$$ = NULL;}
         |COMMA ID FieldDeclAux                          {node_aux1 = add_node(NULL,NULL,type_to_string(act_type),0,0); node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); add_brother(node_aux1,node_aux); $$ = add_node(NULL,node_aux1,"FieldDecl",0,0); add_brother($$,$3);}
;

Type: BOOL                                              {act_type = 0; $$ = add_node(NULL, NULL, "Bool",$1->linha, $1->coluna);}
    |INT                                               {act_type = 1; $$ = add_node(NULL, NULL, "Int",$1->linha, $1->coluna);}
    |DOUBLE                                            {act_type = 2; $$ = add_node(NULL, NULL, "Double",$1->linha, $1->coluna);}
;


MethodHeader: Type ID LPAR FormalParams RPAR           {node_aux3 = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); node_aux2 = add_node(NULL,$4,"MethodParams",0,0); $$ = add_node(NULL,$1,"MethodHeader",0,0); add_brother($1,node_aux3); add_brother(node_aux3,node_aux2);}
            |Type ID LPAR RPAR                         {node_aux3 = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); node_aux2 = add_node(NULL,NULL,"MethodParams",0,0); $$ = add_node(NULL,$1,"MethodHeader",0,0); add_brother($1,node_aux3); add_brother(node_aux3,node_aux2);}
            |VOID ID LPAR FormalParams RPAR            {node_aux1 = add_node(NULL,NULL,"Void",0,0); node_aux3 = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); node_aux2 = add_node(NULL,$4,"MethodParams",0,0); $$ = add_node(NULL,node_aux1,"MethodHeader",0,0); add_brother(node_aux1,node_aux3); add_brother(node_aux3,node_aux2);}
            |VOID ID LPAR RPAR                         {node_aux1 = add_node(NULL,NULL,"Void",0,0); node_aux3 = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); node_aux2 = add_node(NULL,NULL,"MethodParams",0,0); $$ = add_node(NULL,node_aux1,"MethodHeader",0,0); add_brother(node_aux1,node_aux3); add_brother(node_aux3,node_aux2);}
;

FormalParams: Type ID FormalParamsAux                   {node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); add_brother($1,node_aux); $$ = add_node(NULL,$1,"ParamDecl",0,0); add_brother($$,$3);}
            |STRING LSQ RSQ ID                         {node_aux1 = add_node(NULL,NULL,"StringArray",0,0); node_aux = add_node($4->value,NULL,"Id",$4->linha, $4->coluna); add_brother(node_aux1,node_aux); $$ = add_node(NULL,node_aux1,"ParamDecl",0,0);}
;

FormalParamsAux:                                        {$$ = NULL;}
              | COMMA Type ID FormalParamsAux          {node_aux = add_node($3->value,NULL,"Id",$3->linha, $3->coluna); $$ = add_node(NULL,$2,"ParamDecl",0,0); add_brother($2,node_aux); add_brother($$,$4);}
;


MethodBody: LBRACE MethodBodyAux RBRACE                 {$$ = add_node(NULL,$2,"MethodBody",0,0);}
;


MethodBodyAux:                                          {$$ = NULL;}
         |Statement MethodBodyAux                      {if($1!=NULL){$$ = $1; add_brother($$,$2);} else $$=$2;}
         |VarDecl MethodBodyAux                        {$$ = $1; add_brother($$,$2);}
;


VarDecl: Type ID VarDeclAux SEMICOLON                   {node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); add_brother($1,node_aux); $$ = add_node(NULL,$1,"VarDecl",0,0); add_brother($$,$3);}
;


VarDeclAux:                                             {$$ = NULL;}
         |COMMA ID VarDeclAux                          {node_aux1 = add_node(NULL,NULL,type_to_string(act_type),0,0); node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); add_brother(node_aux1,node_aux); $$ = add_node(NULL,node_aux1,"VarDecl",0,0); add_brother($$,$3);}
;

Statement: LBRACE StatementAux RBRACE                   {if(($2 != NULL) && ($2->brother != NULL)){$$ = add_node(NULL,$2,"Block",0,0);} else $$=$2;}
         |IF LPAR Assign RPAR Statement ELSE Statement {$$ = add_node(NULL,$3,"If",0,0);int exe=0; if(($5 == NULL) || (($5 != NULL) && ($5->brother != NULL))){ node_aux = add_node(NULL,$5,"Block",0,0); add_brother($3,node_aux);exe=1;} else{add_brother($3,$5);exe=2;};
                                                                                     if(($7 == NULL) || (($7 != NULL) && ($7->brother != NULL))){ node_aux1 = add_node(NULL,$7,"Block",0,0); add_brother(node_aux,node_aux1);} else{
                                                                                                                                                                                                                                   if(exe==1) {
                                                                                                                                                                                                                                     add_brother(node_aux,$7);
                                                                                                                                                                                                                                   }
                                                                                                                                                                                                                                   else if(exe==2){
                                                                                                                                                                                                                                     add_brother($5,$7);
                                                                                                                                                                                                                                   }
                                                                                                                                                                                                                               } }
         |IF LPAR Assign RPAR Statement                {$$ = add_node(NULL,$3,"If",0,0); if(($5 == NULL) || (($5 != NULL) && ($5->brother != NULL))){ node_aux = add_node(NULL,$5,"Block",0,0); add_brother($3,node_aux); node_aux1 = add_node(NULL,NULL,"Block",0,0); add_brother(node_aux,node_aux1);} else{add_brother($3,$5); node_aux1 = add_node(NULL,NULL,"Block",0,0); add_brother($5,node_aux1);}}

         |WHILE LPAR Assign RPAR Statement             {$$ = add_node(NULL,$3, "While",0,0); if(($5 == NULL) || (($5 != NULL) && ($5->brother != NULL))){node_aux = add_node(NULL,$5,"Block",0,0); add_brother($3,node_aux);} else{add_brother($3,$5);}}
         |RETURN Assign SEMICOLON                      {$$ = add_node(NULL,$2,"Return",$1->linha, $1->coluna);}
         |RETURN SEMICOLON                             {$$ = add_node(NULL,NULL,"Return",$1->linha, $1->coluna);}
         |SEMICOLON                                    {$$ = NULL;}
         |AssignmentAux SEMICOLON                      {$$ = $1;}
         |PRINT LPAR Assign RPAR SEMICOLON             {$$ = add_node(NULL,$3,"Print",0,0);}
         |PRINT LPAR STRLIT RPAR SEMICOLON             {node_aux = add_node($3->value,NULL,"StrLit",$3->linha, $3->coluna); $$ = add_node(NULL,node_aux,"Print",0,0);}
         |error SEMICOLON                              {$$ = NULL;}
;

StatementAux:                                           {$$ = NULL;}
           | Statement StatementAux                    {                 if($1!=NULL){
                                                                                 if($2!=NULL){
                                                                                       add_brother($1,$2);
                                                                                       $$ = $1;
                                                                                   }else{
                                                                                       $$ = $1;
                                                                                   }
                                                                                 }else{
                                                                                   if($2!=NULL){
                                                                                       $$ = $2;
                                                                                   }else{
                                                                                       $$ = NULL;
                                                                                   }
                                                                                 }
                                                                               }
;

Assign: Expr                                            {$$ = $1;}
     | ID ASSIGN Assign                                {node_aux = add_node($1->value,NULL,"Id",$1->linha, $1->coluna); $$ = add_node(NULL,node_aux,"Assign",$2->linha, $2->coluna); add_brother(node_aux,$3);}
     | LPAR ID ASSIGN Assign RPAR                      {node_aux = add_node($2->value,NULL,"Id",$2->linha, $2->coluna); $$ = add_node(NULL,node_aux,"Assign",$3->linha, $3->coluna); add_brother(node_aux,$4);}
;


MethodInvocation: ID LPAR Assign MethodInvocationAux RPAR {node_aux = add_node($1->value,NULL,"Id",$1->linha, $1->coluna); add_brother(node_aux,$3); add_brother($3,$4); $$ = add_node(NULL,node_aux,"Call",$1->linha, $1->coluna);}
                |ID LPAR RPAR                            {node_aux = add_node($1->value,NULL,"Id",$1->linha, $1->coluna); $$ = add_node(NULL,node_aux,"Call",$1->linha, $1->coluna);}
                |ID LPAR error RPAR                      {$$ = NULL;}
;

MethodInvocationAux:                                    {$$ = NULL;}
                |COMMA Assign MethodInvocationAux      {$$ = $2; add_brother($$,$3);}
;


AssignmentAux: ID ASSIGN Assign                         {node_aux = add_node($1->value,NULL,"Id",$1->linha, $1->coluna); $$ = add_node(NULL,node_aux,"Assign",$2->linha, $2->coluna); add_brother(node_aux,$3);}
             | ParseArgs                               {$$ = $1;}
             | MethodInvocation                        {$$ = $1;}
;


ParseArgs: PARSEINT LPAR ID LSQ Assign RSQ RPAR         {node_aux = add_node($3->value,NULL,"Id",$3->linha, $3->coluna); add_brother(node_aux,$5); $$ = add_node(NULL,node_aux,"ParseArgs",$1->linha, $1->coluna);}
         |PARSEINT LPAR error RPAR                     {$$ = NULL;}
;


Expr: Expr PLUS Expr               {$$ = add_node(NULL,$1,"Add",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr MINUS Expr              {$$ = add_node(NULL,$1,"Sub",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr STAR Expr               {$$ = add_node(NULL,$1,"Mul",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr DIV Expr                {$$ = add_node(NULL,$1,"Div",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr MOD Expr                {$$ = add_node(NULL,$1,"Mod",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr AND Expr                {$$ = add_node(NULL,$1,"And",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr OR Expr                 {$$ = add_node(NULL,$1,"Or",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr XOR Expr                {$$ = add_node(NULL,$1,"Xor",$2->linha, $2->coluna);    add_brother($1,$3);}
    |Expr LSHIFT Expr             {$$ = add_node(NULL,$1,"Lshift",$2->linha, $2->coluna); add_brother($1,$3);}
    |Expr RSHIFT Expr             {$$ = add_node(NULL,$1,"Rshift",$2->linha, $2->coluna); add_brother($1,$3);}
    |Expr EQ Expr                 {$$ = add_node(NULL,$1,"Eq",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr GE Expr                 {$$ = add_node(NULL,$1,"Ge",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr GT Expr                 {$$ = add_node(NULL,$1,"Gt",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr LE Expr                 {$$ = add_node(NULL,$1,"Le",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr LT Expr                 {$$ = add_node(NULL,$1,"Lt",$2->linha, $2->coluna);     add_brother($1,$3);}
    |Expr NE Expr                 {$$ = add_node(NULL,$1,"Ne",$2->linha, $2->coluna);     add_brother($1,$3);}
    |MINUS Expr %prec NOT         {$$ = add_node(NULL,$2,"Minus",$1->linha, $1->coluna);}
    |NOT Expr %prec NOT           {$$ = add_node(NULL,$2,"Not",$1->linha, $1->coluna);}
    |PLUS Expr %prec NOT          {$$ = add_node(NULL,$2,"Plus",$1->linha, $1->coluna);}
    |LPAR Assign RPAR             {$$ = $2;}
    |MethodInvocation             {$$ = $1;}
    |ParseArgs                    {$$ = $1;}
    |ID                           {$$ = add_node($1->value,NULL,"Id",$1->linha, $1->coluna);}
    |ID DOTLENGTH                 {node_aux = add_node($1->value,NULL,"Id",$1->linha, $1->coluna); $$ = add_node(NULL,node_aux,"Length",$2->linha, $2->coluna);}
    |INTLIT                       {$$ = add_node($1->value,NULL,"DecLit",$1->linha, $1->coluna);}
    |REALLIT                      {$$ = add_node($1->value,NULL,"RealLit",$1->linha, $1->coluna);}
    |BOOLLIT                      {$$ = add_node($1->value,NULL,"BoolLit",$1->linha, $1->coluna);}
    |LPAR error RPAR              {$$ = NULL;}

%%
