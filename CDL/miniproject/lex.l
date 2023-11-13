%{
#include "bison.tab.h"
extern char* lastToken;  // External declaration for the global variable
%}

letter [a-zA-Z_]
digit [0-9]

%%
"public"    { lastToken = strdup(yytext); return PUBLIC; }
"private"   { lastToken = strdup(yytext); return PRIVATE; }
"protected" { lastToken = strdup(yytext); return PROTECTED; }
"static"    { lastToken = strdup(yytext); return STATIC; }
"class"     { lastToken = strdup(yytext); return CLASS; }
"int"       { lastToken = strdup(yytext); return INT; }
"double"    { lastToken = strdup(yytext); return DOUBLE; }
"char"      { lastToken = strdup(yytext); return CHAR; }
"boolean"   { lastToken = strdup(yytext); return BOOLEAN; }
"void"      { lastToken = strdup(yytext); return VOID; }
"return"    { lastToken = strdup(yytext); return RETURN; }
"true"      { lastToken = strdup(yytext); return BOOLEAN_LITERAL; }
"false"     { lastToken = strdup(yytext); return BOOLEAN_LITERAL; }
"=="        { lastToken = strdup(yytext); return EQ; }
"!="        { lastToken = strdup(yytext); return NE; }
"<"         { lastToken = strdup(yytext); return LT; }
">"         { lastToken = strdup(yytext); return GT; }
"<="        { lastToken = strdup(yytext); return LE; }
">="        { lastToken = strdup(yytext); return GE; }
"+"         { lastToken = strdup(yytext); return ADD; }
"-"         { lastToken = strdup(yytext); return SUB; }
"*"         { lastToken = strdup(yytext); return MUL; }
"/"         { lastToken = strdup(yytext); return DIV; }
"="         { lastToken = strdup(yytext); return '='; }
"{"         { lastToken = strdup(yytext); return '{'; }
"}"         { lastToken = strdup(yytext); return '}'; }
"("         { lastToken = strdup(yytext); return '('; }
")"         { lastToken = strdup(yytext); return ')'; }
";"         { lastToken = strdup(yytext); return ';'; }
","         { lastToken = strdup(yytext); return ','; }
"'"{letter}"'"         { lastToken = strdup(yytext); return CHARACTER_LITERAL; }
"\"".*"\""        { lastToken = strdup(yytext); return STRING_LITERAL; }

{digit}+"."{digit}+    { lastToken = strdup(yytext); yylval.str = strdup(yytext); return FLOATING_POINT_LITERAL; }
[+-]?{digit}+          { lastToken = strdup(yytext); yylval.str = strdup(yytext); return INTEGER_LITERAL; }
[+-]?{digit}+"."{digit}+    { lastToken = strdup(yytext); yylval.str = strdup(yytext); return FLOATING_POINT_LITERAL; }

{letter}({letter}|{digit})*    { lastToken = strdup(yytext); yylval.str = strdup(yytext); return VALID_IDENTIFIER; }

[ \t\n\r]     /* skip whitespace */
.           { lastToken = strdup(yytext); return INTEGER_LITERAL; }

%%

int yywrap() {
    return 1;
}
