%{ 
	#include <stdio.h>
	FILE * out;
%}

%%
[0-9]* {int x = atoi(yytext); fprintf(yyout, "%X", x);}
%%

int yywrap() {return 1;}

int main(){
	yyin = fopen("in.c", "r");
	yyout = fopen("out.c", "w");
	yylex();
	printf("DONE!\n");
	return 0;
}