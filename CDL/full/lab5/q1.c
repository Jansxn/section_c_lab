/*
S-> a | > | (T)
T-> ST'
T-> ,ST' | e
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S(char *str, int * ind);
int T(char *str, int * ind);
int T_(char *str, int * ind);

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
	char lookahead = str[*ind + 1];
	// printf("%c", lookahead);
	
	if (lookahead == 'a'){
		++(*ind);
		return 1;
	}
	else if (lookahead == '>'){
		++(*ind);
		return 1;
	}
	else if (lookahead == '('){
		lookahead = str[(++(*ind)) + 1];

		if (T(str, ind) == 1){
			// printf("%c", lookahead);
			lookahead = str[*ind + 1];
			if (lookahead == ')') {
				// printf("%c", lookahead);
				++(*ind);
				return 1;
			}
			else return 0;
		}
		else return 0;
	}
	else return 0;
}

int T(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (S(str, ind) == 1){
		if (T_(str, ind) == 1){
			return 1;
		}
	}
	return 0;
}

int T_(char *str, int * ind){
	char lookahead = str[*ind + 1];

	if (lookahead == ','){
		lookahead = str[(++(*ind)) + 1];

		if (S(str, ind) == 1){
			if (T_(str, ind) == 1){
				return 1;
			}
			else return 0;
		}
		else return 0;
	}
	else{
		return 1;
	}
}