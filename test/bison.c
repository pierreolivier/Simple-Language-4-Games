%{
	#include <stdio.h>

	int yylex(void);
	void yyerror(const char * msg);
	int lineNumber;
	extern FILE * yyin;
%}

//%token START END ASSIGN SEMICOLON IDENT REAL INTEGER
%token NUM ID
%token TYPE ASSIGN COLON
%token BRACKET_START BRACKET_END ACCOLADE_START ACCOLADE_END COMMA

%union { char str[0x100]; double num; }
%type<str> ID
%type<num> NUM

%start program

%%
/*
program : START instList END { printf("program\n"); }
;

instList : instList inst
	| instruct
;

inst : IDENT ASSIGN expr_number SEMICOLON { printf("assign %s with %f\n", $1, $3); }
        | IDENT ASSIGN expr_str SEMICOLON { printf("assign %s with %s\n", $1, $3); }
;

expr_number : INTEGER { printf("integer %f\n", $1); sprintf($$,"i:%d",$1); }
	| REAL { printf("real %f\n", $1); sprintf($$,"r:%g",$1); }
;
expr_str : IDENT { printf("ident %s\n", $1); sprintf($$,"s:%s",$1); }
;*/
program :       instructions { printf("program\n"); }
;
instructions :  instructions instruction
                | instruction
;
instruction :   function
                | allocation
;
function :      TYPE ID BRACKET_START BRACKET_END ACCOLADE_START instructions ACCOLADE_END {printf("function\n");}
                | TYPE ID BRACKET_START args BRACKET_END ACCOLADE_START instructions ACCOLADE_END {printf("function with args\n");}
                | ID BRACKET_START BRACKET_END COLON
                | ID BRACKET_START parameters BRACKET_END COLON
;
allocation :    ID ASSIGN ID COLON
                | ID ASSIGN NUM COLON
                | ID ASSIGN ID BRACKET_START BRACKET_END COLON
                | ID ASSIGN ID BRACKET_START parameters BRACKET_END COLON
;
args :          TYPE ID
                | args COMMA TYPE ID
;
parameters :    ID
                | parameters COMMA ID
;
%%



void yyerror(const char * msg)
{
        printf("line %d: %s\n", lineNumber, msg);
}

int main(int argc,char ** argv)
{
        if(argc>1) yyin=fopen(argv[1],"r"); // check result !!!
        lineNumber=1;
        if(!yyparse()) printf("Success\n");
        return(0);
}

