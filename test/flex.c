%{
    #include "bison.tab.h"

    extern int lineNumber;
    void yyerror(const char * msg);

%}

%option noyywrap

num	[0-9]+|[0-9]+\.[0-9]*|\.[0-9]+
id	[a-zA-Z_][0-9a-zA-Z_]*

%%
"num"|"void" {return (TYPE);}
"=" 	{ return(ASSIGN); }
";"		{ return(COLON); }
{num}   { sscanf(yytext,"%lf",&yylval.num); return(NUM); }
{id}	{ sprintf(yylval.str,"%s",yytext); return(ID); }
"("     { return (BRACKET_START); }
")"     { return (BRACKET_END); }
"{"     { return (ACCOLADE_START); }
"}"     { return (ACCOLADE_END); }
","     { return (COMMA); }


"\n"	{ ++lineNumber; }
[ \t]+	{ /* nothing to be done */ }
.		{ char msg[0x20]; sprintf(msg,"lexical error <%s>",yytext); yyerror(msg); }

%%
