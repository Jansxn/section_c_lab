%{ 
	#include <stdio.h>
	int c=0, v=0;
%}

%%
[aeiouAEIOU] {v++;}
\n {printf("\nConstants:%d\nVowels:%d\n", c, v); return 0;}
[A-Za-z] {c++;}
%%

int yywrap() {return 1;}

int main(){
	yylex();
	return 0;
}