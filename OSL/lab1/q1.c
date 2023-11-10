#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
	int out = open ("output.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (out<0) printf("Failure in connecting\n");
	char hi[20] = "HEllo Worladsdd!sads";
	for (int i =0; i<20; i++){
		if (hi[i] == '\0') printf("Hel;lo");
		else write(out, &hi[i], 1);
	}
	close (out);
	return 0;
}
