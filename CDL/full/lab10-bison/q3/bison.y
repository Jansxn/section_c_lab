%{
    #include<stdio.h>
    #include<stdlib.h>
%}

%token num ID NL

%%
input : /* epsilon */
    |input line { printf("Valid Expression\n"); exit(0);}
line : NL
    | exp NL
exp : num
    | exp exp '+'
    | exp exp '-'
    | exp exp '*'
    | exp exp '/'
    | exp exp '^'
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