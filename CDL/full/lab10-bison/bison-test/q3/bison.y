%{
	#include <stdio.h>
	#include<stdlib.h>	
	int yylex();
	int yyerror();
%}

%token NUMBER
%left '+' '-'
%left '*' '/'
%right '='

%%
E: T {printf("Answer: %d\n", $$);} ;

T: T '+' T {$$ = $1 + $3;}
	| T '-' T {$$ = $1 - $3;}
	| T '*' T {$$ = $1 * $3;}
	| T '/' T {$$ = $1 / $3;}
	| '-' T {$$ = -$1;}
	| '(' T ')' { $$ = $2; }
	| NUMBER {$$ = $1;};
%%

int yyerror(char *msg)
{
    printf("Invalid Expression\n");
    exit(0);
}

int main(){
	yyparse();
	return 0;
}