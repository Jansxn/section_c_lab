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

void getNextToken(FILE *fd, char * lex){
	detectOperators(fd);
	if (strcmp(symbol_table[ind-1].lex, lex) != 0) programerror();
}

void programerror(){
	printf("Error!\n");
	exit(0);
}

void data_type(FILE *fd){
	detectOperators(fd);
	if ((strcmp(symbol_table[ind-1].lex, "int") != 0)||strcmp(symbol_table[ind-1].lex, "char") != 0) programerror();
}

void declarations(FILE *fd){
	if (data_type(fd)){
		if (identifier_list(fd)){
			getNextToken(fd, ";");
			declarations(fd);
		}
	}
	else{
		
	}
}

int program(FILE *fd){
	getNextToken(fd, "int");
	getNextToken(fd, "main");
	getNextToken(fd, "(");
	getNextToken(fd, ")");
	getNextToken(fd, "{");

	if (declarations(fd)){

	}
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
	printf("\n< %s, %d, %d >\n", t.lex, t.row, t.col);
}

void printSymbolTable(){
	for (int i =0 ; i<curr_ind; i++){
		// printf("\n< %s, %d, %d >\n", symbol_table[i].lex, symbol_table[i].row, symbol_table[i].col);
		char keywords[8][100] = {"char", "double", "float", "int", "long", "short", "struct", "void"};
		if(symbol_table[i].token_type == not_id) continue;
		else if (symbol_table[i].token_type == function){
			printf("\n%d. Function: %s\tRT:%s\tArgs:%d\n", symbol_table[i].ind, symbol_table[i].lex, keywords[(int)symbol_table[i].return_type], symbol_table[i].no_args);
		}
		else{
			printf("\n%d. ID: %s\tDT:%s\n", symbol_table[i].ind, symbol_table[i].lex, keywords[(int)symbol_table[i].data_type]);
		}

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
	printf("Enter filename\n");
	char filename[50];
	scanf("%s", filename);

	FILE *fd = fopen (filename,"r");

	if (fd == NULL){
		printf("File not found");
		exit(0);
	}
	curr_ind=0, ind =0;
	row =1, col =1;

	while (detectOperators(fd)){
		char next;
		scanf("%c", &next);
	}

	for (int i = 0; i<curr_ind && curr_ind>1; i++){
		int key;

		if (((key = isKeyword(symbol_table[i].lex))>=0)){
			i++;
			if (strcmp(symbol_table[i+1].lex,"(")==0){
				symbol_table[i].return_type = key;
				symbol_table[i].token_type = function;
				countArguments(&i);
			}
			else{
				symbol_table[i].token_type = id;
				symbol_table[i].data_type = key;
				i++;

				while ((strcmp(symbol_table[i].lex,",")==0)){
					i++;
					symbol_table[i].data_type = key;
					symbol_table[i].token_type = id;
					i++;
				}
			}
		}
	}

	// printSymbolTable();

	return 0;
}