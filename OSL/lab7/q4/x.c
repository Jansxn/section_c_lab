#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"


int main(){
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	int shmid;

	srand((unsigned int)getpid());
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	
	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %X\n", (int)shared_memory);

	shared_stuff = (struct shared_use_st *)shared_memory;
	for (int i =0; i<3; i++){
		for (int j=0 ; j<3; j++) shared_stuff->mat[i][j] = '-';
	}
    shared_stuff->written_by_you = 0;
	
	while(running) {

		while(shared_stuff->written_by_you == 1) { }
		printBoard(shared_stuff->mat);
		printf("\n");
		int m, n;
		while (1){
				printf("Enter row and column:");
				scanf("%d %d", &m, &n);

				if (shared_stuff->mat[m-1][n-1] == '-'){
					shared_stuff->mat[m-1][n-1] = 'X';
					break;
				}
				else {
					printf("Place is already taken");
					continue;
				}
			}
		shared_stuff->written_by_you = 1;
		printBoard(shared_stuff->mat);
		printf("\n");
	}

	if (shmdt(shared_memory) == -1) {
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}



