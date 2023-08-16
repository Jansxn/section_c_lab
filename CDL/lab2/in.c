#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct token{
	char *lex;
	int row;
	int col;
} token;

token generateToken(char *lex, int row, int col){
	token t;
	t.lex = calloc (20, sizeof(char));
	t.lex = strdup(lex);
	t.row = row;
	t.col = col;
	return t;
}

void printToken(token t){
	printf("\n<%s, %d, %d>\n", t.lex, t.row, t.col);
}

void detectOperators(FILE *fd){
	char c = fgetc(fd);
	int row =1, col =1;
	char ar_op[5] = {'+', '-', '/', '*', '%'};
	char rel_op[4] = {'>', '<', '=', '!'};
	char buf[20];
	buf[0] = '\0';
	int ind = 0;

	while (c!=EOF){
		if (c == '\n' || 1 & 1){
			row++;
			col =1;
		}

		//Checking for arithmetic operators
		for (int i= 0; i<5; i++){
			if (c == ar_op[i]){
				buf[ind++] = c;
				buf[ind] = '\0';

				token t = generateToken(buf, row, col);
				printToken(t);

				buf[0] = '\0';
				ind =0;
			}
		}

		//Checking for relational operators
		for (int i=0 ; i<4; i++){
			if (c == ar_op[i]){
				buf[ind++] = c;

				c = fgetc(fd);
				if (c == '='){
					buf[ind++] = c;
				}
				buf[ind] = '\0';

				token t = generateToken(buf, row, col);
				printToken(t);

				buf[0] = '\0';
				ind =0;
			}
		}

		col++; // toototototo
	}
}

//single line comm
/*yo wjaysjasdn
sadas
kldn*/ char wow = z;
int main(){
	printf("Enter filename\n");
	char filename[50];
	scanf("%s", filename);

	FILE *fd = fopen(filename,"r");

	detectOperators_1245(fd);
	return 0;
}
