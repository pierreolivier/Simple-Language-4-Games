%{
    #include "bison.tab.h"

    extern int lineNumber;
    void yyerror(const char * msg);

%}

%option noyywrap

num	[0-9]+|[0-9]+\.[0-9]*|\.[0-9]+
id	[a-zA-Z_][0-9a-zA-Z_]*

%%
"num"|"void"    {return (TYPE);}
"1"|"0"         { return (BOOLEAN); }
"if"        { return(IF); }
"else"        { return(ELSE); }
"for"       { return(FOR); }
"while"     { return(WHILE); }
"=" 	    { return(ASSIGN); }
";"		    { return(COLON); }

"("         { return (BRACKET_START); }
")"         { return (BRACKET_END); }
"{"         { return (ACCOLADE_START); }
"}"         { return (ACCOLADE_END); }
","         { return (COMMA); }

"=="        { return (EQ); }
"!="        { return (NE); }
">="        { return (GE); }
"<="        { return (LE); }
"<"         { return (L); }
">"         { return (G); }
"*"|"/"|"+"|"-"    { return *yytext; }

{num}       { sscanf(yytext,"%lf",&yylval.num); return(NUM); }
{id}	    { sprintf(yylval.str,"%s",yytext); return(ID); }

"\n"	    { ++lineNumber; }
[ \t]+	    { /* nothing to be done */ }
.		    { char msg[0x20]; sprintf(msg,"lexical error <%s>",yytext); yyerror(msg); }

%%
