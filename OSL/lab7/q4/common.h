struct shared_use_st {
	int written_by_you;
	char mat[3][3];
};

void verifyWin(char board[3][3]){
	char win[2] = {'X', 'O'};

	for (int k =0 ; k<2; k++){
		int i =0; // Horizontal
		for (int j =0 ; j<3; j++){
			if (board[i][j] == win[k]){
				if (board[i+1][j] == win[k] && board[i+2][j] == win[k]){
					printf("%c won", win[k]);
					return;
				}
				printf("SDakfnsadnfk");
			}

		}

		int j =0; //Vertical
		for (i =0 ; i<3; i++){
			if (board[i][j] == win[k]){
				if (board[i][j+1] == win[k] && board[i][j+2] == win[k]){
					printf("%c won", win[k]);
					return;
				}
			}
		}

		//Diagonal
		if ((board[0][0] == win[k] && board[1][1]==win[k] && board[2][2]==win[k]) || (board[0][2] == win[k] && board[1][1]==win[k] && board[2][0]==win[k])){ 
			printf("%c won", win[k]);
			return;
		}
	}
}

void printBoard(char board[3][3]){
	for (int i =0; i<3; i++){
		for (int j=0 ; j<3; j++) printf("%c", board[i][j]);
		printf("\n");
	}
	verifyWin(board);
}