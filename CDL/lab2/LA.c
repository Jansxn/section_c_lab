#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct token{
	char *lex;
	int row;
	int col;
} token;

token generateToken(char *lex, int row, int col){
	token t;
	t.lex = calloc (50, sizeof(char));
	t.lex = strdup(lex);
	t.row = row;
	t.col = col;
	return t;
}

void printToken(token t){
	printf("\n< %s, %d, %d >\n", t.lex, t.row, t.col);
}

void detectOperators(FILE *fd){
	char c = fgetc(fd);
	int row =1, col =1;

	char ar_op[5] = {'+', '-', '/', '*', '%'};
	char rel_op[4] = {'>', '<', '=', '!'};
	char log_op[2] = {'&', '|'};
	char special[11] = {',', '<', '>', '(', ')', '[', ']', '{', '}', ';', '#'};

	char buf[50];
	buf[0] = '\0';
	int ind = 0;

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
						col =0;
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

		if (c == ' ' || c == '\t') flag = 1;

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
	}
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

	detectOperators(fd);
	return 0;
}