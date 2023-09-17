#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE * fp = fopen("in.txt", "w+");
	char * str = "HJello World\n! IOgtj sdkfnkd\nkkanflsk";
	for (int i =0; str[i]!='\0'; i++){
		putc(str[i], fp);
	}
	fseek(fp, 10, SEEK_SET);
	putc('i', fp); //Replaces characters that were in place
	putc('i', fp);
	fseek(fp, -1, SEEK_CUR);
	char c = getc(fp);
	while (c!=EOF){
		printf("%c", c);
		c = getc(fp);
	}
	return 0;
}