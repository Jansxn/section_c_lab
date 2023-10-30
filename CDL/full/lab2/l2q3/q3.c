#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(){
	char keywords[32][100] = {"auto", "break", "case", "char", "const", "continue","default","do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

	char name[100], word[100];
	printf("Enter filename: ");

	scanf("%s", name);

	FILE *in = fopen(name, "r");
	if (in == NULL){
		printf("File doesnt exists\n");
		exit(0);
	}

	char c = getc(in);
	word[0] = c;
	int i = 1;

	while (c!=EOF){
		while (isalpha(c) != 0){
			c = getc(in);
			word[i++] = c;
			// word[i++] = c;
			// printf("%c", c);
		}
		word[i] = '\0';
		// puts(word);
		if (c == EOF) break;
		// c = getc(in);
		i=0;

		for (int j=0 ;j<32; j++){
			if (strcmp(word, keywords[j]) == 0){
				for (int k=0; k<strlen(word); k++) printf("%c", toupper(word[k]));
				printf("\n");
				break;
			}
		}
	}

	fclose (in);
	exit(0);
}