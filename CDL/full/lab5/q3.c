/*
S-> aAcBe
A-> bA'
A'-> bA'/e
B-> d
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S(char *str, int * ind);
int A(char *str, int * ind);
int A_(char *str, int * ind);
int B(char *str, int * ind);

int main(){
	printf("Input string: ");
	char str[25];
	scanf("%s", str);
	str[strlen(str)] = '$';

	int ind = -1;

	if (S(str, &ind) == 1){
		printf("%c", str[ind]);
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
	char lookahead = str[*ind + 1];

	if (lookahead == 'a'){
		(*ind)++;
		if (A(str, ind)){
			lookahead = str[*ind + 1];
			if (lookahead == 'c'){
				(*ind)++;
				if(B(str, ind)){
					lookahead = str[*ind + 1];
					if (lookahead == 'e'){
						(*ind)++;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int A(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == 'b'){
		lookahead = str[(++(*ind)) + 1];
		if (A_(str, ind) == 1){
			return 1;
		}
		return 0;
	}

	else return 0;
}

int A_(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == 'b'){
		lookahead = str[(++(*ind)) + 1];
		if (A_(str, ind) == 1){
			return 1;
		}
		return 1;
	}

	else return 1;
}

int B(char *str, int * ind){
	char lookahead = str[*ind + 1];
	if (lookahead == 'd'){
		++(*ind);
		return 1;
	}
	return 0;
}