%{ 
	#include <stdio.h>
	int w=0, c=0, b=0, l=0;
%}

%%
\n {l++;c++;}
[A-Za-z0-9_]* {w++; c+=strlen(yytext);}
[ ]|\t {b++;c++;}
%%

int yywrap() {return 1;}

int main(){
	yylex();
	printf("Chars: %d, Words: %d, Lines: %d, Blanks: %d\n", c, w, l, b);
	return 0;
}