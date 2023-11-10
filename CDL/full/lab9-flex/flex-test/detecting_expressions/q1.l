%{
#include <stdio.h>
%}

keys "int"|"char"

%%
[{keys}a] {printf("Intsad");}
%%

int yywrap(){
	printf("DOne"); return 1;
}

int main(){
	yylex();
	return 0;
}