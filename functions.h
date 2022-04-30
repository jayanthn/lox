#include "tokentype.h"

// Core Functions
int readPrompt();
int readFile(char *);
int runBuf(tok * ,char *);
int grow_struct(tok *,int);

//scanner functions
// tok *addToken(tok *,enum TokenType,char *,void *,int,int);
void addSToken(tok *,enum TokenType ,int ,int *);
void addKeyToken(char *,int ,tok *,int ,int *);
void addValToken(enum TokenType,void *,tok *,int ,int *);
char *add_const_string(char *,int *,char *);

//Error loggers
void error(int,char*);
void report(int, char*,char*);

//tokenizer
int scantokens(tok *,char *);
int scantoken(tok *,char *,int *,int *,char*);


//helper functions  
void lowercase(char *);
int isAlpha(char *);
int isAlphaNumeric(char *);
int isDigit(char *);
int check_keyword(char *);
float *get_number_literal(char **,int *,char *);
char *get_identifier(char **,char *);

