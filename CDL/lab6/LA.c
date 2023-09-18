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
int detectOperators(FILE *fd);
void printSymbolTable();

void programerror(){
    printf("Error!\n");
    exit(0);
}


void getNextToken(FILE *fd, char * lex){
    detectOperators(fd);
    // printSymbolTable();
    // printf("%s\n", symbol_table[curr_ind-1].lex);
    if (strcmp(symbol_table[curr_ind-1].lex, lex) != 0) programerror();
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
        if (str[i]-'0'>=0 && str[i]-'9'<=9){
            continue;
        }
        else return 0;
    }
    return 1;
}

int assign_stat(FILE *fd){
    char * str = symbol_table[curr_ind-1].lex;

    if (checkID(str)){
        getNextToken(fd, "=");

        detectOperators(fd);
        str = symbol_table[curr_ind-1].lex;

        if(checkID(str) || checkNum(str)){
            getNextToken(fd, ";");
            return 1;
        }
    }

    return 0;
}

int identifier_list(FILE *fd){
    if(detectOperators(fd) == 0) return 0;
    char * str = symbol_table[curr_ind-1].lex;
    // printf("%s", str);

    if(!checkID(str)) return 0;

    if(detectOperators(fd) == 0) return 0;
    str = symbol_table[curr_ind-1].lex;

    if (strcmp(str, ",") == 0){
        if (identifier_list(fd)) return 1;
    }
    return 1;
}

int data_type(char * lex){
    if ((strcmp(lex, "int") != 0) && strcmp(lex, "char") != 0) return 0;
    return 1;
}

int declarations(FILE *fd){
    detectOperators(fd);
    // printf("%s\n", symbol_table[curr_ind-1].lex);
    char * lookahead = symbol_table[curr_ind-1].lex;

    if (data_type(lookahead)){
        // printf("%d\n", data_type(lookahead));
        if (identifier_list(fd)){
            char * str = symbol_table[curr_ind-1].lex;
            // printf("%s\n", str);

            if (strcmp(str, ";")!=0) programerror();
            if(declarations(fd)==0) return 0;
            return 1;
        }
        else return 0;
    }
    // printf("%s", lookahead);
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
        if(assign_stat(fd)){
            getNextToken(fd, "}");
            return 1;
        }
        else programerror();
    }
    else programerror();
    return 0;
}

void RDParser(FILE *fd){
    if (program(fd)) printf("Success");
    else programerror();
}

token generateToken(char *lex, int row, int col){
    token t;
    t.lex = calloc (50, sizeof(char));
    t.lex = strdup(lex);
    t.row = row;
    t.token_type = not_id;
    t.col = col;
    t.ind = curr_ind;
    symbol_table[curr_ind++] = t;
    return t;
}

void printToken(token t){
    // printf("\n< %s, %d, %d >\n", t.lex, t.row, t.col);
}

void printSymbolTable(){
    for (int i =0 ; i<curr_ind; i++){
        printf("\n%d. ID: %s", curr_ind, symbol_table[i].lex);
    }
}

int detectOperators(FILE *fd){
    char c = fgetc(fd);

    char ar_op[5] = {'+', '-', '/', '*', '%'};
    char rel_op[4] = {'>', '<', '=', '!'};
    char log_op[2] = {'&', '|'};
    char special[11] = {',', '<', '>', '(', ')', '[', ']', '{', '}', ';', '#'};

    char buf[50];
    buf[0] = '\0';
    if (c == EOF) return 0;

    while (c!=EOF){
        int flag =0;
        
        if (c == '\n' || c == '\r'){
            row++;
            col =1;
            c = fgetc(fd);
            continue;
        }

        else if(c == '/'){
            c = fgetc(fd);
            col++;

            if (c == '*'){
                c = fgetc(fd);
                col++;

                while (c != '*'){
                    if (c == '\n' || c == '\r'){
                        row++;
                        col =1;
                    }
                    c = fgetc(fd);
                    col++;  
                }
                c = fgetc(fd);
                col++;
                flag = 1;

            }

            else if (c == '/'){
                while (c!='\n') c = fgetc(fd);
                row++;
            }
        }

        else if (c == '#'){
            c = fgetc(fd);
            col++;

            while(c!='\n'){
                col++;
                c = fgetc(fd);
            }
            continue;
        }

        if (c == ' ' || c == '\t'){
            c = fgetc(fd);
            continue;
        }

        //Checking for arithmetic operators
        if (flag == 0){
            for (int i= 0; i<5; i++){
                if (c == ar_op[i]){
                    buf[ind++] = c;
                    buf[ind] = '\0';

                    token t = generateToken(buf, row, col);
                    printToken(t);

                    buf[0] = '\0';
                    ind =0;
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0){
            //Checking for relational operators
            for (int i=0 ; i<4; i++){
                if (c == rel_op[i]){
                    int init_col = col;
                    buf[ind++] = c;
                    c = fgetc(fd);
                    if (c == '='){
                        buf[ind++] = c;
                        col++;
                    }
                    else{
                        fseek(fd, -1, SEEK_CUR);
                    }
                    buf[ind] = '\0';

                    token t = generateToken(buf, row, init_col);
                    printToken(t);

                    buf[0] = '\0';
                    ind =0;
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0){
            //Checking for logical operators
            for (int i=0 ; i<2; i++){
                if (c == log_op[i]){
                    int init_col = col;
                    buf[ind++] = c;
                    c = fgetc(fd);
                    if (c == log_op[i]){
                        buf[ind++] = c;
                        col++;
                    }
                    else{
                        fseek(fd, -1, SEEK_CUR);
                    }
                    buf[ind] = '\0';

                    token t = generateToken(buf, row, init_col);
                    printToken(t);

                    buf[0] = '\0';
                    ind =0;
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0){
            //Checking for arithmetic operators
            for (int i= 0; i<11; i++){
                if (c == special[i]){
                    buf[ind++] = c;
                    buf[ind] = '\0';

                    token t = generateToken(buf, row, col);
                    printToken(t);

                    buf[0] = '\0';
                    ind = 0;
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0){
            //String literals
            if (c == '"' || c == '\''){
                token t = generateToken("\"", row, col);
                printToken(t);

                c = fgetc(fd);
                int init_col = ++col;
                
                while (c != '"' && c != '\''){
                    buf[ind++] = c;
                    c = fgetc(fd);
                    col++;
                }
                buf[ind] = '\0';

                t = generateToken(buf, row, init_col);
                printToken(t);

                t = generateToken("\"", row, col);
                printToken(t);

                buf[0] = '\0';
                ind = 0;
                flag = 1;
            }
        }

        if (flag == 0){
            //Keywords, Numbers, Identifiers
            char keywords[32][100] = {"auto", "break", "case", "char", "const", "continue","default","do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
            int init_col = col;

            //Word Generation
            while (isalpha(c)!=0 || c == '_' || (c-'0'>=0 && c-'0'<=9)){
                buf[ind++] = c;
                c = fgetc(fd);
                flag = 1;
                col++;
            }
            buf[ind] = '\0';

            if (flag == 1) {
                fseek(fd, -1, SEEK_CUR);
                col--;
            }
            flag = 0;

            //Keywords
            for (int i =0 ; i<32; i++){
                if (strcmp(keywords[i], buf) == 0){
                    token t = generateToken(buf, row, init_col);
                    printToken(t);

                    buf[0] = '\0';
                    ind = 0;
                    flag = 1;
                    break;
                }
            }
            
            //Check for identifiers
            if (flag == 0 && (isalpha(buf[0]) !=0 || buf[0]== '_')){ 
                token t = generateToken(buf, row, init_col);
                printToken(t);
                flag = 1;
            }

            //Check for numbers
            if (flag == 0){
                int bad = 0;
                for (int i= 0; i<strlen(buf); i++){
                    if (buf[i]-'0'<=0 && buf[i]-'0'>=9){
                        bad = 1;
                        break;
                    }
                }
                if (bad == 0){
                    token t = generateToken(buf, row, init_col);
                    printToken(t);
                    flag = 1;
                }
            }

            buf[0] = '\0';
            ind = 0;
            flag = 1;
        }

        c = fgetc(fd);
        col++;
        fseek(fd, -1, SEEK_CUR);
        return 1;
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
    // printf("%d", symbol_table[*i].no_args);
    *i = j;
}

int main(){
    // printf("Enter filename\n");
    // char filename[50];
    // scanf("%s", filename);

    FILE *fd = fopen ("in.c","r");

    if (fd == NULL){
        printf("File not found");
        exit(0);
    }
    curr_ind=0, ind =0;
    row =1, col =1;

    RDParser(fd);
    // if (str)

    // for (int i = 0; i<curr_ind && curr_ind>1; i++){
    //     int key;

    //     if (((key = isKeyword(symbol_table[i].lex))>=0)){
    //         i++;
    //         if (strcmp(symbol_table[i+1].lex,"(")==0){
    //             symbol_table[i].return_type = key;
    //             symbol_table[i].token_type = function;
    //             countArguments(&i);
    //         }
    //         else{
    //             symbol_table[i].token_type = id;
    //             symbol_table[i].data_type = key;
    //             i++;

    //             while ((strcmp(symbol_table[i].lex,",")==0)){
    //                 i++;
    //                 symbol_table[i].data_type = key;
    //                 symbol_table[i].token_type = id;
    //                 i++;
    //             }
    //         }
    //     }
    // }

    // printSymbolTable();

    return 0;
}
