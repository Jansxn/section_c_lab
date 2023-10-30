#include <stdio.h>
#include <stdlib.h>
int main(){
	printf("Enter filename: ");
	char name[100];
	scanf("%s", name);
	int a = 3+2-c*x/2;
	FILE *in = fopen(name, "r");
	if (in == NULL){
		printf("File doesnt exists\n");
		exit(0);
	}
	FILE *out = fopen("out.c", "w");
	char c = getc(in);
	while (c!=EOF){
		if (c == '#'){
			while (c!='\n' && c!=EOF){
				c = getc(in);
			}
			if (c >= '\n') c = getc(in);
			continue;
		}
		putc(c, out);
		c = getc(in);
	}
	fclose (in);
	fclose (out);
	exit(0);
}