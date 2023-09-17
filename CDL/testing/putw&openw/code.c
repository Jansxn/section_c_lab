#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE * fp = fopen("in.c", "w+");

	putw(2, fp);
	putw(3, fp);
	putw(2048*2, fp);
	putw(1, fp);
	fclose (fp);
	return 0;
}