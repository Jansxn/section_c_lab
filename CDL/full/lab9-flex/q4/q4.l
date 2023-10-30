%{ 
	#include <stdio.h>
	int s=0, p=0;
	FILE * out;
%}

%%
"scanf" {fprintf(yyout, "READ"); s++;}
"printf" {fprintf(yyout, "WRITE"); p++;}

%%

int yywrap() {return 1;}

int main(){
	yyin = fopen("in.c", "r");
	yyout = fopen("out.c", "w");
	yylex();
	printf("SCANF: %d, PRINTF: %d\n", s, p);
	return 0;
}