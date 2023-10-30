#include <stdio.h>
#include <stdlib.h>

int main (int argc, char*argv[]){
	printf("Enter input filename");
	char name[20];
	scanf("%s", name);

	FILE *in = fopen(name, "r");
	if (in == NULL){
		printf("Couldnt find file\n");
		exit(0);
	}

	FILE *out = fopen("out.c", "w");

	char c = getc(in);
	int prev_space = 0;

	while (c!= EOF){
		
		if (c == ' '){
			if (prev_space == 0){
				putc(c,out);
				prev_space =1;
			}
		} 

		else if ( c == '\t'){
			if (prev_space == 0){
				putc(' ',out);
				prev_space =1;
			}
		}

		else{
			prev_space = 0;
			putc(c, out);
		}

		c = getc(in);
	}
	fclose (in);
	fclose(out);
	exit(0);
}