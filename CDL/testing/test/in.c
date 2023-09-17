That takes C program as input, recognizes all the keywords and prints them in upper case.*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


int main(){
	FILE *f1;
	char c,word[100];
	char keyword[32][100]={"auto", "break", "case", "char", "const", 
	"continue","default","do", "double", "else", "enum", "extern", "float",
	 "for", "goto", "if", "int", "long", "register", "return", "short", 
	 "signed", "sizeof", "static", "struct", "switch", "typedef", "union", 
	 "unsigned", "void", "volatile", "while"};

	f1=fopen("q1in.c","r");
	if(f1==NULL){
		printf("File not found\n");
		exit(1);
	}

	c=fgetc(f1);
	word[0]=c;
	int i=1;

	while(c!=EOF){
		while(isalpha(c)!=0){
			//word[i++]=c;
			c=fgetc(f1);
			word[i++]=c;
		}
		word[i]='\0';
		i=0;
		c=fgetc(f1);
		for(int j=0;j<32;j++){
			if(strcmp(word,keyword[j])==0){
				for(int k=0;k<strlen(keyword[j]);k++){
					printf("%c",toupper(keyword[j][k]));
				}
				printf("\n");
			}
		}
	}
	fclose(f1);
	return 0;
}