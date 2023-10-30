#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "getNextToken.h"
#define MAX_ROWS 200

int main(){

    FILE *fd = fopen ("in.c","r");

    if (fd == NULL){
        printf("File not found");
        exit(0);
    }
    curr_ind=0, ind =0;
    row =1, col =1;
    trav_ind = 0;

    RDParser(fd);

    return 0;
}