%{ 
	#include <stdio.h>
	FILE * out;
%}

%%
\/\/.* {fprintf(yyout, "%s", yytext);}
\/\*(.|\n)*\*\/ {fprintf(yyout, "%s", yytext);}
[A-Z] {char x = yytext[0]+32; fprintf(yyout, "%c", x);}
[a-z] {char x = yytext[0]-32; fprintf(yyout, "%c", x);}
%%

int yywrap() {return 1;}

int main(){
	yyin = fopen("in.c", "r");
	yyout = fopen("out.c", "w");
	yylex();
	printf("DONE!\n");
	return 0;
}