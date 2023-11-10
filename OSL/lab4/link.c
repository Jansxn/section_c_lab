#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	if(link("test.txt", "link.txt")){
		printf("Error\n");
		return 1;
	}
	else printf("Success\n");
	return 0;
}