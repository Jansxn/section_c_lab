%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* lastToken;  // Global variable to store the last token
int yylex();
void yyerror(const char *s);
%}

%union {
    char *str;
}

%token <str> VALID_IDENTIFIER INTEGER_LITERAL FLOATING_POINT_LITERAL CHARACTER_LITERAL BOOLEAN_LITERAL STRING_LITERAL
%token ADD SUB MUL DIV EQ NE LT GT LE GE AND OR NOT
%token CLASS INT DOUBLE CHAR BOOLEAN VOID RETURN STATIC PUBLIC PRIVATE PROTECTED
%token <str> LETTER DIGIT

%left '+' '-'
%left '*' '/'
%left EQ NE LT GT LE GE AND OR

%%

program: class_declaration { printf("Success!\n"); }
    ;

class_declaration: access_specifier CLASS VALID_IDENTIFIER '{' class_body '}'
    ;

access_specifier: PUBLIC
    | PRIVATE
    | PROTECTED
    | /* ε */
    ;

class_body: class_members
    ;

class_members: class_member
    | class_member class_members
    ;

class_member: field_declaration
    | method_declaration
    ;

field_declaration: access_specifier type field_name_list ';'
    ;

field_name_list: VALID_IDENTIFIER
    | VALID_IDENTIFIER ',' field_name_list
    ;

method_declaration: access_specifier method_header '(' parameters ')' block
    ;


method_header: type VALID_IDENTIFIER
    | STATIC type VALID_IDENTIFIER
    ;

type: INT
    | DOUBLE
    | CHAR
    | BOOLEAN
    | VOID
    | VALID_IDENTIFIER
    ;

parameters: parameter_list
    | /* ε */
    ;

parameter_list: type VALID_IDENTIFIER
    | type VALID_IDENTIFIER ',' parameter_list
    ;

block: '{' statements '}'
    ;

statements: statement
    | statement statements
    ;

statement: variable_declaration
    | expression_statement
    | return_statement
    | assignment_statement
    ;

variable_declaration: type VALID_IDENTIFIER '=' expression ';'
    | type VALID_IDENTIFIER ';'
    ;

expression_statement: expression ';'
    ;

return_statement: RETURN expression ';'
    ;

expression: operations
    | operand
    ;

assignment_statement: variable '=' operations ';'
    ;

operations: operand ADD operand
    | operand SUB operand
    | operand MUL operand
    | operand DIV operand
    | operand EQ operand
    | operand NE operand
    | operand LT operand
    | operand GT operand
    | operand LE operand
    | operand GE operand
    | operand AND operand
    | operand OR operand
    ;

operand: variable
    | literal
    ;

variable: VALID_IDENTIFIER
    ;

literal: INTEGER_LITERAL
    | FLOATING_POINT_LITERAL
    | CHARACTER_LITERAL
    | BOOLEAN_LITERAL
    | STRING_LITERAL
    ;

%%
void yyerror(const char *s) {
    if (lastToken != NULL) {
        fprintf(stderr, "Error: %s. Last token: %s\n", s, lastToken);
    } else {
        fprintf(stderr, "Error: %s. Last token not available.\n", s);
    }
    exit(EXIT_FAILURE);
}
int main() {
    yyparse();
    return 0;
}
