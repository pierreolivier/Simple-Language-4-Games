%{ /*-------- prog.y --------*/
	#include <stdio.h>
	#include <iostream>
	using namespace std;
	int yylex(void);
	void yyerror(const char * msg); 
	int lineNumber; 
	extern FILE * yyin;
%}

%token START END ASSIGN SEMICOLON IDENT REAL INTEGER

%union { char str[0x100]; double real; int integer; }
%type<str> IDENT expr
%type<real> REAL
%type<integer> INTEGER

%start program

%%
program : START instList END { cerr << "program" << endl; }
;

instList : instList inst
	| inst
;

inst : IDENT ASSIGN expr SEMICOLON { cerr << "assign " << $1 << " with " << $3 << endl; }
;

expr : INTEGER { cerr << "integer " << $1 << endl; sprintf($$,"i:%d",$1); }
	| REAL { cerr << "real " << $1 << endl; sprintf($$,"r:%g",$1); }
	| IDENT { cerr << "ident " << $1 << endl; sprintf($$,"s:%s",$1); }
;
%%

void yyerror(const char * msg)
{
        cerr << "line " << lineNumber << ": " << msg << endl;
}

int main(int argc,char ** argv)
{
        if(argc>1) yyin=fopen(argv[1],"r"); // check result !!!
        lineNumber=1;
        if(!yyparse()) cerr << "Success" << endl;
        return(0);
}

