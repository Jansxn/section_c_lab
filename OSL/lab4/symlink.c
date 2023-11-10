#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	if(symlink("test.txt", "symlink.txt")){
		printf("Error\n");
		return 1;
	}
	else printf("Success\n");
	// unlink("test.txt");
	return 0;
}