/*Catarina Proença 2016232360
Catarina Moreira 2016232565*/


%X LINECOMMENT
%X BLOCKCOMMENT
%X STRLITSTATE

%{
  #include <string.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include "y.tab.h"
  #include "functions_tree.h"

  int line_num = 1;
  int comment_line=1;
  int comment_col=1;
  int col_num = 1;
  int message = 0;
  int yacc_aux = 0;
  int flag_tree = 0;
  int flag_s = 1;

  extern node* root;
%}

NUMERO  [0-9]+|([0-9]+"_"*[0-9]+("_"*[0-9]+)*)+
EXPOENTE ("e"|"E")("+"|"-")?(([0-9]+)|([0-9]+"_"*[0-9]+("_"*[0-9]+)*)+)
SEQUENCIA [a-zA-Z0-9_$]*
ID [A-Za-z_$][a-zA-Z0-9_$]*
STR [^\n|\r|\"]|" "
NEWLINE \n|\r|(\r\n)
OUTSEQ \\f|\\n|\\r|\\t|\\\\|\\\"
ESC (f|n|r|t|\\|\")
LETRA [A-Za-a_$]

BOOLLIT "true"|"false"
AND   "&&"
ASSIGN  "="
STAR  "*"
COMMA ","
DIV   "/"
EQ    "=="
GE    ">="
GT    ">"
LBRACE "{"
LE    "<="
LPAR  "("
LSQ   "["
LT    "<"
MINUS "-"
MOD   "%"
NE    "!="
NOT   "!"
OR    "||"
PLUS  "+"
RBRACE  "}"
RPAR ")"
RSQ   "]"
SEMICOLON ";"
ARROW "->"
LSHIFT "<<"
RSHIFT ">>"
XOR  "\^"
PRINT "System.out.print"
PARSEINT  "Integer.parseInt"
BOOL  "boolean"
CLASS "class"
DOTLENGTH ".length"
DOUBLE "double"
ELSE  "else"
IF    "if"
INT   "int"
STRING  "String"
PUBLIC "public"
RETURN  "return"
STATIC "static"
VOID "void"
WHILE "while"
ESPACO  " "|"\f"|"\t"
RESERVED "null"|"++"|"--"|"char"|"volatile"|"Integer"|"System"|"abstract"|"continue"|"switch"|"assert"|"default"|"package"|"synchronized"|"do"|"goto"|"private"|"switch"|"new"|"for"|"if"|"default"|"this"|"private"|"throw"|"protected"|"implements"|"break"|"throws"|"public"|"import"|"byte"|"transient"|"instanceof"|"enum"|"case"|"try"|"short"|"extends"|"catch"|"interface"|"final"|"volatil"|"strictfp"|"long"|"finally"|"super"|"native"|"float"|"const"

%%
{CLASS}     {col_num+=yyleng;if(message==1){printf("CLASS\n");}if(yacc_aux) return CLASS;}
{RETURN}    {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("RETURN\n");}if(yacc_aux) return RETURN;}
{PUBLIC}    {col_num+=yyleng;if(message==1){printf("PUBLIC\n");}if(yacc_aux) return PUBLIC;}
{STATIC}    {col_num+=yyleng;if(message==1){printf("STATIC\n");}if(yacc_aux) return STATIC;}
{INT}       {col_num+=yyleng;if(message==1){printf("INT\n");}if(yacc_aux) return INT;}
{STRING}    {col_num+=yyleng;if(message==1){printf("STRING\n");}if(yacc_aux) return STRING;}
{VOID}      {col_num+=yyleng;if(message==1){printf("VOID\n");}if(yacc_aux) return VOID;}
{WHILE}     {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("WHILE\n");}if(yacc_aux) return WHILE;}
{IF}        {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("IF\n");}if(yacc_aux) return IF;}
{DOUBLE}    {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("DOUBLE\n");}if(yacc_aux) return DOUBLE;}
{BOOL}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("BOOL\n");}if(yacc_aux) return BOOL;}
{ELSE}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("ELSE\n");}if(yacc_aux) return ELSE;}
{PARSEINT}  {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("PARSEINT\n");}if(yacc_aux) return PARSEINT;}
{DOTLENGTH} {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("DOTLENGTH\n");}if(yacc_aux) return DOTLENGTH;}
{PRINT}     {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("PRINT\n");}if(yacc_aux) return PRINT;}


({NUMERO}"."({NUMERO}|{EXPOENTE}|{NUMERO}{EXPOENTE})?)|("."{NUMERO}{EXPOENTE}?)|({NUMERO}{EXPOENTE})  {col_num+=yyleng;if(message==1){printf("REALLIT(%s)\n",yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext));return REALLIT;}}
("0")|([1-9]+[0-9]*)+|([1-9]+"_"*[0-9]+("_"+[0-9]+)*)+                                                {col_num+=yyleng;if(message==1){printf("INTLIT(%s)\n",yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext));return INTLIT;}}
(\"([^\n\r\"\\]|(\\f|\\n|\\r|\\t|\\\\|\\\"))*\")                                                      {col_num+=yyleng;if(message==1){printf("STRLIT(%s)\n",yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext));return STRLIT;}}


{RESERVED}    {col_num+=yyleng;if(message==1){printf("RESERVED(%s)\n",yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext)); return RESERVED;}}
{BOOLLIT}     {col_num+=yyleng;if(message==1){printf("BOOLLIT(%s)\n", yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext));return BOOLLIT;}}
{ID}          {col_num+=yyleng;if(message==1){printf("ID(%s)\n", yytext);}if(yacc_aux){yylval.value=create_token(yytext,line_num, col_num-strlen(yytext));return ID;}}
{LBRACE}      {col_num+=yyleng;if(message==1){printf("LBRACE\n");}if(yacc_aux) return LBRACE;}
{RBRACE}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("RBRACE\n");}if(yacc_aux) return RBRACE;}
{LPAR}        {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("LPAR\n");}if(yacc_aux) return LPAR;}
{RPAR}        {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("RPAR\n");}if(yacc_aux) return RPAR;}
{EQ}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("EQ\n");}if(yacc_aux) return EQ;}
{STAR}        {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("STAR\n");}if(yacc_aux) return STAR;}
{XOR}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("XOR\n");}if(yacc_aux) return XOR;}
{MINUS}       {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("MINUS\n");}if(yacc_aux) return MINUS;}
{ARROW}       {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("ARROW\n");}if(yacc_aux) return ARROW;}
{SEMICOLON}   {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("SEMICOLON\n");}if(yacc_aux) return SEMICOLON;}
{LSQ}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("LSQ\n");}if(yacc_aux) return LSQ;}
{RSQ}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("RSQ\n");}if(yacc_aux) return RSQ;}
{ASSIGN}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("ASSIGN\n");}if(yacc_aux) return ASSIGN;}
{AND}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("AND\n");}if(yacc_aux) return AND;}
{COMMA}       {col_num+=yyleng;if(message==1){printf("COMMA\n");}if(yacc_aux) return COMMA;}
{DIV}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("DIV\n");}if(yacc_aux) return DIV;}
{GE}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("GE\n");}if(yacc_aux) return GE;}
{GT}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("GT\n");}if(yacc_aux) return GT;}
{LE}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("LE\n");}if(yacc_aux) return LE;}
{LT}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("LT\n");}if(yacc_aux) return LT;}
{MOD}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("MOD\n");}if(yacc_aux) return MOD;}
{NE}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("NE\n");}if(yacc_aux) return NE;}
{NOT}         {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("NOT\n");}if(yacc_aux) return NOT;}
{OR}          {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("OR\n");}if(yacc_aux) return OR;}
{PLUS}        {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("PLUS\n");}if(yacc_aux) return PLUS;}
{LSHIFT}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("LSHIFT\n");}if(yacc_aux) return LSHIFT;}
{RSHIFT}      {col_num+=yyleng;yylval.value=create_token(NULL,line_num, col_num-strlen(yytext));if(message==1){printf("RSHIFT\n");}if(yacc_aux) return RSHIFT;}
{ESPACO}      {col_num+=yyleng;}
{NEWLINE}     {col_num=1;line_num+=1;}


"//"                        {BEGIN LINECOMMENT;col_num+=yyleng;}
<LINECOMMENT>{NEWLINE}      {BEGIN 0;col_num=1; line_num+=1;}
<LINECOMMENT><<EOF>>        {BEGIN 0;}
<LINECOMMENT>.              {col_num+=yyleng;}//qualquer caracter excepto \n

"/*"                        {BEGIN BLOCKCOMMENT;comment_line=line_num;comment_col=col_num;col_num+=yyleng;}
<BLOCKCOMMENT>{NEWLINE}     {col_num=1; line_num +=1;}
<BLOCKCOMMENT>"*/"          {BEGIN 0;col_num+=yyleng;}
<BLOCKCOMMENT><<EOF>>       {BEGIN 0;printf("Line %d, col %d: unterminated comment\n",comment_line, comment_col);}
<BLOCKCOMMENT>.             {col_num+=yyleng;}//qualquer caracter excepto \n


"\""                      {BEGIN STRLITSTATE; comment_col=col_num;comment_line=line_num;col_num+=yyleng;}
<STRLITSTATE>{OUTSEQ}     {col_num+=yyleng;}
<STRLITSTATE>((\\.)|(\\)) {printf("Line %d, col %d: invalid escape sequence (%s)\n",line_num,col_num,yytext);col_num+=yyleng;}
<STRLITSTATE>{NEWLINE}    {BEGIN 0;printf("Line %d, col %d: unterminated string literal\n",comment_line,comment_col);col_num=1;line_num+=1;}
<STRLITSTATE>"\""         {BEGIN 0;col_num+=yyleng;}
<STRLITSTATE><<EOF>>      {BEGIN 0;printf("Line %d, col %d: unterminated string literal\n",comment_line,comment_col);col_num=1;line_num+=1;}
<STRLITSTATE>.            {col_num+=yyleng;}

.                         {printf("Line %d, col %d: illegal character (%s)\n",line_num,col_num,yytext);col_num+=yyleng;}

%%

void yyerror (char *s) {
     flag_tree = 1;
     if(flag_s == 1)
      printf ("Line %d, col %d: %s: %s\n",line_num,(int)(col_num-strlen(yytext)), s, yytext);
}

int main(int argc, char *argv[]){
  if(argc>1){
    if(strcmp(argv[1],"-l")==0){
        message = 1;
        yacc_aux = 0;
        yylex();
    }
    else if(strcmp(argv[1],"-e1")==0){
        message = 0;
        yacc_aux = 0;
        yylex();
    }
    else if(strcmp(argv[1],"-e2")==0){
      message = 0;
      yacc_aux = 1;
      flag_tree = 1;
      yyparse();
      yylex();
    }
    else if(strcmp(argv[1],"-t")==0){
      yacc_aux = 1;
      yyparse();
      if(flag_tree == 0)
        print_tree(root,0);

    }
    else if(strcmp(argv[1], "-s")==0){
      yacc_aux = 1; //ou message = 1;

      flag_s = 0;
      yyparse();
      sym_table(root);

      //Começa a Symbol Table
      check_program(root);
      annotate_tree(root);

      print_erros();
      print_sym_table();
      print_tree(root,0);

      freeErrorList(erros);
      freeSymbols(table);
      freeTree(root);
    }
  } 
  else{
    message = 0;
    yacc_aux = 1;
    yyparse();
    yylex();

    if(flag_tree!=1){
      sym_table(root);
      check_program(root);
      annotate_tree(root);

      print_erros();

      freeErrorList(erros);
      freeSymbols(table);
      freeTree(root);
    }
  }
  return 0;
}

int yywrap()
{
  return 1;
}
