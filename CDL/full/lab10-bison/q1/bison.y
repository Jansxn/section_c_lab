%{
#include<stdio.h>
#include<stdlib.h>
%}

%name parse
%token NUMBER ID NL DT

%%
declarations: DT IL ';' declarations {printf("Declaration valid\n"); exit(0);}
| {printf("Empty");};
IL: ID | ID ',' IL
;
%%

int yyerror(char *msg)
{
	printf("Invalid Expression\n");
	exit(0);
}

void main ()
{
	printf("Enter the expression\n");
	yyparse();
}