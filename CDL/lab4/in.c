#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_ROWS 50

void removeWhitespaces(FILE *in, FILE *out){
  char c = fgetc(in);
  while (c!=EOF){
    if (c!='\t' &&c!='\n' && c!='\r'){
      fputc(c, out);
    }
    else if (c == '\n' || c == ' '){
      fputc(' ', out);
      c = fgetc(in);
      while (c=='\n' || c==' ') c = fgetc(in);
      continue;
    }
    c=fgetc(in);
  }
}

enum token {function, id};
enum types {character, num0, num1, num2, num3, num4, structure, empty};

typedef struct stnode{
  int ind;
  char* lex;
  enum types return_type;
  enum types data_type;
  enum token token_type;
  int no_args;
} stnode;
stnode symbol_table[MAX_ROWS];
int ind;

int isKeyword(char * t){
  char keywords[8][100] = {"char", "double", "float", "int", "long", "short", "struct", "void"};
  for (int i =0; i<8;i++){
    if (strcmp(keywords[i], t) == 0) return i;
  }
  return -1;
}

char * generateWord(FILE *fd, char *c){
  char* buf = calloc (50, sizeof(char));
  int i=0;

  while (isalpha(*c) !=0 || *c == '_' || (*c-'0'>= 0 && *c-'0'<=9)){
    buf[i++] = *c;
    *c = fgetc(fd);
  }
  buf[i] = '\0';
  return buf;
}

void insertSymbol(char *dt, int t){
  stnode st;
  st.lex = calloc (50, sizeof(char));
  st.lex = strdup(dt);

  st.return_type =t;
  st.data_type =t;
  st.ind =ind;

  symbol_table[ind] = st;
}

void countArguments(FILE * fd){
  char c = fgetc(fd);
  if (c==' ') c = fgetc(fd);

  if (c == ')') {
    symbol_table[ind].no_args = 0;
    return;
  }

  int commas = 0;

  while (c!=')'){
    if (c == ',') commas++;
    c = fgetc(fd);
  }
  symbol_table[ind].no_args = commas+1;
}

void createSymbolTable(FILE *fd){
  char c = fgetc(fd);
  while (c!=EOF){
    char * dt = generateWord(fd, &c);
    int i;
    if ((i = isKeyword(dt))>=0){
      c= fgetc(fd);
      
      char * l = generateWord(fd,&c);
      insertSymbol(l, i);
      
      if (c == ' ') c= fgetc(fd);
      
      if (c == '(') {
        symbol_table[ind].token_type = function;
        countArguments(fd);
      }
      else {
        symbol_table[ind].token_type = id;
        if (c == ' ') c= fgetc(fd);
        while (c == ','){
          
          c= fgetc(fd);
          if (c == ' ') c= fgetc(fd);

          l = generateWord(fd,&c);
          if (strlen(l)>0){
            ind++;
            insertSymbol(l, i);
            symbol_table[ind].token_type = id;
          }
          if (c == ' ') c= fgetc(fd);
        }
      }
      ind++;
    }
    c = fgetc(fd);
  }
}

int main(int argc, char* argv[]){

  if (argc!=2){
    printf("Execute as - ./q1 <file_exec>");
  }
  FILE *fd = fopen(argv[1], "r");
  FILE *out = fopen("out.txt", "w+");
  ind =1;

  removeWhitespaces(fd, out);
  fseek(out, 0, SEEK_SET);
  createSymbolTable(out);

  for (int i =1 ; i<ind; i++){
    char keywords[8][100] = {"char", "double", "float", "int", "long", "short", "struct", "void"};
    if (symbol_table[i].token_type == function){
      printf("\n%d. Function: %s\tRT:%s\tArgs:%d\n", symbol_table[i].ind, symbol_table[i].lex, keywords[(int)symbol_table[i].return_type], symbol_table[i].no_args);
    }
    else{
      printf("\n%d. ID: %s\tDT:%s\n", symbol_table[i].ind, symbol_table[i].lex, keywords[(int)symbol_table[i].data_type]);
    }
  }
  return 0;
}