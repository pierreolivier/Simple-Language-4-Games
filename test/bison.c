%{
	#include <stdio.h>

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
program :       instructions { printf("program\n"); }
;
instructions :  instructions instruction
                | instruction
;
instruction :   function
                | allocation
                | condition
;
function :      TYPE ID BRACKET_START BRACKET_END ACCOLADE_START instructions ACCOLADE_END { printf("function\n"); }
                | TYPE ID BRACKET_START args BRACKET_END ACCOLADE_START instructions ACCOLADE_END { printf("function with args\n"); }
                | ID BRACKET_START BRACKET_END COLON
                | ID BRACKET_START parameters BRACKET_END COLON
;
allocation :    ID ASSIGN ID BRACKET_START BRACKET_END COLON
                | ID ASSIGN ID BRACKET_START parameters BRACKET_END COLON
                | ID ASSIGN expr COLON { printf("%f\n", $3); }
;
condition:      IF BRACKET_START expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | IF BRACKET_START expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END ELSE ACCOLADE_START instructions ACCOLADE_END
                | WHILE BRACKET_START expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END
                | FOR BRACKET_START ID ASSIGN NUM COLON expr COLON ID ASSIGN expr BRACKET_END ACCOLADE_START instructions ACCOLADE_END

;/*
boolean_expr:
                BOOLEAN { $$ = $1; printf("bool: %d\n", $$);}//expr {if($1 >= 1) $$ = 1; else $$ = 0; }
;*/
expr :          NUM {$$ = $1;}
                | ID {$$ = 1;}
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

