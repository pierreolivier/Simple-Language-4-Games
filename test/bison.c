%{
	#include <stdio.h>
	#include "parser.h"

	int yylex(void);
	void yyerror(const char * msg);
	int lineNumber;
	extern FILE * yyin;
%}
//%token START END ASSIGN SEMICOLON IDENT REAL INTEGER

%token BOOLEAN NUM ID
%token TYPE ASSIGN COLON
%token BRACKET_START BRACKET_END ACCOLADE_START ACCOLADE_END COMMA
%token IF ELSE WHILE FOR
%token PRINT
%left EQ NE GE LE G L
%left '+' '-'
%left '*' '/'


%union {
    char str[0x100];
    double num;
    int boolean;
}
%type<str> ID
%type<num> NUM expr
%type<boolean> BOOLEAN boolean_expr

%start program

%%
program :       instructions
;
instructions :  instructions instruction
                | instruction
;
instruction :   function
		| declaration
                | allocation
                | condition
                | predef_functions
;
function :      TYPE ID BRACKET_START BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | TYPE ID BRACKET_START args BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | ID BRACKET_START BRACKET_END COLON
                | ID BRACKET_START parameters BRACKET_END COLON
;
declaration :	TYPE ID COLON { declaration($2); }

;
allocation :    ID ASSIGN ID BRACKET_START BRACKET_END COLON
                | ID ASSIGN ID BRACKET_START parameters BRACKET_END COLON
                | ID ASSIGN expr COLON { if(!allocation($1, $3)) { char msg[0x20]; sprintf(msg,"var not exists \"%s\"",$1);  yyerror(msg); } }
;
condition:      IF BRACKET_START boolean_expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | IF BRACKET_START boolean_expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END ELSE ACCOLADE_START instructions ACCOLADE_END
                | WHILE BRACKET_START boolean_expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | FOR BRACKET_START ID ASSIGN NUM COLON boolean_expr COLON ID ASSIGN expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END
;
boolean_expr:
		expr { if($1 >= 1) $$ = 1; else $$ = 0; }
;
expr :		NUM {$$ = $1;}
		| BOOLEAN { $$ = (double) $1; }
                | ID {$$ = get_value($1);}
                | expr '+' expr {$$ = $1 + $3;}
                | expr '-' expr {$$ = $1 - $3;}
                | expr '*' expr {$$ = $1 * $3;}
                | expr '/' expr {$$ = $1 / $3;}
                | expr L expr   {$$ = $1 < $3;}
                | expr G expr   {$$ = $1 > $3;}
                | expr GE expr  {$$ = $1 >= $3;}
                | expr LE expr  {$$ = $1 <= $3;}
                | expr NE expr  {$$ = $1 != $3;}
                | expr EQ expr  {$$ = $1 == $3;}
                | BRACKET_START expr BRACKET_END {$$ = $2;}
;
args :          TYPE ID
                | args COMMA TYPE ID
;
parameters :    ID
                | parameters COMMA ID
;

predef_functions :
		PRINT BRACKET_START BRACKET_END COLON { printf("\n"); }
		| PRINT BRACKET_START ID BRACKET_END COLON { printf("%f\n", get_value($3)); }
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
