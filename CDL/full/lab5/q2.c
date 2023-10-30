/*
S -> UVW
U -> (S) | aSb | d
V -> aV | 𝜖
W -> cW | 𝜖
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S(char *str, int * ind);
int U(char *str, int * ind);
int V(char *str, int * ind);
int W(char *str, int * ind);

int main(){
	printf("Input string: ");
	char str[25];
	scanf("%s", str);
	str[strlen(str)] = '$';

	int ind = -1;

	if (S(str, &ind) == 1){
		if (str[ind+1] == '$') printf("\nAccepted\n");
		else{
			printf("\nRejected\n");
		}
	}
	else{
		printf("\nRejected\n");
	}
	return 0;
}

int S(char *str, int * ind){
	if (U(str, ind) == 1){
		if (V(str, ind) == 1){
			if (W(str, ind) == 1){
				return 1;
			}
		}
	}
	return 0;
}

int U(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == '('){
		lookahead = str[(++(*ind)) + 1];
		if(S(str, ind) == 1){
			lookahead = str[((*ind)) + 1];
			// printf("%c", lookahead);
			if (lookahead == ')'){
				(*ind)++;
				return 1;
			}
			else return 0;
		}
		return 0;
	}
	else if(lookahead == 'a'){
		lookahead = str[(++(*ind)) + 1];
		if(S(str, ind) == 1){
			lookahead = str[((*ind)) + 1];
			if (lookahead == 'b'){
				(*ind)++;
				return 1;
			}
			else return 0;
		}
	}
	else if(lookahead == 'd'){
		++(*ind);
		return 1;
	}
	return 0;
}

int V(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == 'a'){
		lookahead = str[(++(*ind)) + 1];
		if (V(str, ind) == 1){
			return 1;
		}
		return 1;
	}

	else return 1;
}

int W(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == 'c'){
		lookahead = str[(++(*ind)) + 1];
		if (W(str, ind) == 1){
			return 1;
		}
		else return 1;
	}

	else return 1;
}


