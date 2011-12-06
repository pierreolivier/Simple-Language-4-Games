%{
extern int lineNumber;
void yyerror(const char * msg);
#include "progY.hpp"
%}

%option noyywrap

integer	[0-9]+
real	[0-9]+\.[0-9]*|\.[0-9]+
ident	[a-zA-Z_][0-9a-zA-Z_]*

%%
"start"	{ return(START); }
"end"	{ return(END); }
":="	{ return(ASSIGN); }
";"		{ return(SEMICOLON); }
{ident}	{ sprintf(yylval.str,"%s",yytext); return(IDENT); } // %type<str> IDENT
{real}	{ sscanf(yytext,"%lf",&yylval.real); return(REAL); } // %type<real> REAL
{integer} { sscanf(yytext,"%d",&yylval.integer); return(INTEGER); } // %type<integer> INTEGER
"\n"	{ ++lineNumber; }
[ \t]+	{ /* nothing to be done */ }
.		{ char msg[0x20]; sprintf(msg,"lexical error <%s>",yytext); yyerror(msg); }
%%

