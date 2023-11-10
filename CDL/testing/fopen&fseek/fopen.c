#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE * fp = fopen("in.txt", "w+");
	char * str = "HJello World\n!";
	for (int i =0; str[i]!='\0'; i++){
		putc(str[i], fp);
	}
	fseek(fp, 10, SEEK_SET);
	fputc('i', fp); //Replaces characters that were in place
	fputc('i', fp);
	fseek(fp, -1, SEEK_END);
	char c = getc(fp);
	while (c!=EOF){
		printf("%c", c);
		c = fgetc(fp);
	}
	return 0;
}
