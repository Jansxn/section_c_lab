#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_ROWS 200

enum type_of_token {not_id = -1, function, id};
enum type_of_id {character, num0, num1, num2, num3, num4, structure, empty};

typedef struct token{
    int ind;
    char *lex;
    int row;
    int col;
    enum type_of_id return_type;
    enum type_of_id data_type;
    enum type_of_token token_type;
    int no_args;
} token;

token symbol_table[MAX_ROWS];
int curr_ind;
int row, col;
int ind;
int trav_ind;

int detectOperators(FILE *fd);
void printSymbolTable();
token generateToken(char *lex, int row, int col);
char * response;

void programerror(){
    printf("Error on line %d, position %d! \n", row, col);
    if (response[0] != '\0') printf("%s\n", response);
    exit(0);
}

void getNextToken(FILE *fd, char * lex){
    detectOperators(fd);
    // printf("%s, %s\n", lex, symbol_table[trav_ind].lex);
    if (strcmp(symbol_table[trav_ind].lex, lex) != 0) {
        sprintf(response, "Expected %s, not %s", lex, symbol_table[trav_ind].lex);
        programerror();
    }
}

int checkID(char * str){
    for (int i =0; str[i]!='\0'; i++){
        if (isalpha(str[i])!=0 || str[i] == '_' || (str[i]-'0'>=0 && str[i]-'9'<=9 && i>0)){
            continue;
        }
        else return 0;
    }
    return 1;
}

int checkNum(char * str){
    for (int i =0; str[i]!='\0'; i++){
        if (str[i]-'0'>=0 && str[i]-'9'<=0){
            // printf("%d\n", str[i]-'9');
            continue;
        }
        else return 0;
    }
    return 1;
}

int relop(FILE *fd){
    char* rel[6] = {"==", "!=", "<=", ">=", "<", ">"};

    detectOperators(fd);
    char * str = symbol_table[trav_ind].lex;

    for (int i =0; i<6; i++){
        if (strcmp(str, rel[i]) == 0) return 1;
    }
    sprintf(response, "Expected relational operator");
    return 0;
}

int addop(FILE *fd){
    char * add[2] = {"+", "-"};
    detectOperators(fd);
    char * str = symbol_table[trav_ind].lex;

    for (int i =0; i<2; i++){
        if (strcmp(str, add[i]) == 0) return 1;
    }
    sprintf(response, "Expected additive operator");
    return 0;
}

int mulop(FILE *fd){
    char * mul[3] = {"*", "/", "%"};
    detectOperators(fd);
    char * str = symbol_table[trav_ind].lex;

    for (int i =0; i<3; i++){
        if (strcmp(str, mul[i]) == 0) return 1;
    }
    sprintf(response, "Expected multiplicative operator");
    return 0;
}

int factor(FILE *fd){
    detectOperators(fd);
    char * str = symbol_table[trav_ind].lex;
    if (checkNum(str) || checkID(str)){
        return 1;
    }
    sprintf(response, "Expected number or identifier");
    return 0;
}

int tprime(FILE *fd){
    if (mulop(fd)){
        if (factor(fd)){
            if (tprime(fd)) return 1;
            else return 0;
        }
        else return 0;
    }
    trav_ind--;
    // printf("%s", symbol_table[trav_ind].lex);
    return 1;
}

int term(FILE * fd){
    if (factor(fd)){
        if (tprime(fd)) return 1;
        else return 0;
    }
    return 0;
}

int seprime(FILE * fd){
    if (addop(fd)){
        if (term(fd)){
            if (seprime(fd)) return 1;
            else return 0;
        }
        else return 0;
    }
    trav_ind--;
    // printf("%s", symbol_table[trav_ind].lex);
    return 1;
}

int simple_exp(FILE * fd){
    if (term(fd)){
        if (seprime(fd)) {return 1;}
        else return 0;
    }
    return 0;
}

int eprime(FILE * fd){
    if (relop(fd)){
        if (simple_exp(fd)) return 1;
        else return 0;
    }
    trav_ind--;
    return  1;
}

int expn(FILE * fd){
    if (simple_exp(fd)){
        if (eprime(fd)) {return 1;}
        else return 0;
    }
    return 0;
}

int assign_stat(FILE *fd){
    char * str = symbol_table[trav_ind].lex;
    if (checkID(str)){
        getNextToken(fd, "=");
        if (expn(fd)) return 1;
        else return 0;
    }
    sprintf(response, "Missing ID");
    return 0;
}

int statement(FILE * fd){
    if (assign_stat(fd)){
        // printf("Here");
        getNextToken(fd, ";");
        return 1;
    }
    else return 0;
}

int statement_list(FILE * fd){
    if (statement(fd)){
        detectOperators(fd);
        if (statement_list(fd)) return 1;
        else return 0;
    }
    trav_ind--;
    return 1;
}

int identifier_list(FILE *fd){
    if(detectOperators(fd) == 0) return 0;
    char * str = symbol_table[trav_ind].lex;

    //id
    if(!checkID(str)) return 0;

    if(detectOperators(fd) == 0) return 0;
    str = symbol_table[trav_ind].lex;

    //id, identifier_list
    if (strcmp(str, ",") == 0){
        if (identifier_list(fd)) return 1;
        else return 0;
    }

    //id[number]
    if (strcmp(str, "[") == 0){
        if(detectOperators(fd) == 0) return 0;
        str = symbol_table[trav_ind].lex;

        if (checkNum(str)){
            getNextToken(fd, "]");

            //id[number], identifier_list
            if(detectOperators(fd) == 0) return 0;
            str = symbol_table[trav_ind].lex;

            if (strcmp(str, ",") == 0){
                if (identifier_list(fd)) return 1;
                else return 0;
            }

            return 1;
        }
        else return 0;
    }
    return 1;
}

int data_type(char * lex){
    if ((strcmp(lex, "int") != 0) && strcmp(lex, "char") != 0) return 0;
    return 1;
}

int declarations(FILE *fd){
    detectOperators(fd);
    char * lookahead = symbol_table[trav_ind].lex;

    if (data_type(lookahead)){
        if (identifier_list(fd)){
            char * str = symbol_table[trav_ind].lex;

            if (strcmp(str, ";")!=0) programerror();
            if(declarations(fd)==0) return 0;
            return 1;
        }
        else return 0;
    }
    return 1;
}

int program(FILE *fd){
    getNextToken(fd, "int");
    // printf("Hello");
    getNextToken(fd, "main");
    getNextToken(fd, "(");
    getNextToken(fd, ")");
    getNextToken(fd, "{");

    if (declarations(fd)){
        if(statement_list(fd)){
            getNextToken(fd, "}");
            return 1;
        }
        else programerror();
    }
    else programerror();
    return 0;
}

void RDParser(FILE *fd){
    response = calloc (40, sizeof(char));
    response[0] = '\0';
    if (program(fd)) printf("Success\n");
    else programerror();
}

void printSymbolTable(){
    for (int i =0 ; i<curr_ind; i++){
        printf("\n%d. ID: %s", curr_ind, symbol_table[i].lex);
    }
}

int isKeyword(char * t){
    char keywords[8][100] = {"char", "double", "float", "int", "long", "short", "struct", "void"};
    for (int i =0; i<8;i++){
        if (strcmp(keywords[i], t) == 0) {
            return i;
        }
    }
    return -1;
}

void countArguments(int *i){

    if (strcmp(symbol_table[*i+2].lex,")") == 0) {
        symbol_table[*i].no_args = 0;
        return;
    }

    int commas = 0;
    int j = *i+1;

    while (strcmp(symbol_table[j].lex,")") != 0){
        if (strcmp(symbol_table[j].lex,",") == 0) commas++;
        j++;
    }
    symbol_table[*i].no_args = commas+1;
    *i = j;
}
