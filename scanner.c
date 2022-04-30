#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#include "functions.h"


typedef struct token tok;

static const char keywords[][10] = {
            "and",
            "class",
            "else",
            "false",
            "for", 
            "fun", 
            "if",  
            "nil",
            "or", 
            "print",
            "return",
            "super", 
            "this",
            "true",
            "var"
        };

static const int v[] = {AND,CLASS,ELSE,FALSE,FOR,FUN,IF,NIL,OR,PRINT,RETURN,SUPER,THIS,TRUE,VAR};

int grow_struct(tok *old,int oldsize)
{
    
    const int totalSize = oldsize + 1024;
    tok *newtokens = (tok *)realloc((void *)old,totalSize *sizeof(tok *));
    return oldsize + 1024;
};

int scantokens(tok * tokens,char *buf)
{   
    char *curpos = buf,*eoflexeme,err[256];
    int count=0,linecount=0,offset = 0;
    while (*curpos != NULL)
    {
        ;
        if((offset = scantoken(tokens,curpos,&count,&linecount,err)) < 0)
        {
            strcpy(err,"Error while parsing");
            error(linecount,err);
            return -3;
        }
        curpos+=offset;
    }
    
    // enum TokenType eof = EOF1;
    // strcpy(eoflexeme,"");
    // ++count;
    // addToken(tokens,eof,eoflexeme,NULL,NULL,*count);
    return 0;

    
}

int scantoken(tok *tokens,char *curpos,int *count,int *line,char *err)
{
    char *bufstart = curpos;
    switch(*curpos)
    {
        // Ignore whitespace.
        case ' ':
        case '\r':
        case '\t':
            curpos++;
            break;
        case '\n':
            curpos++;
            *line++;
            break;
        case '(': addSToken(tokens,LEFT_PAREN,*line,count); curpos++;break;
        case ')': addSToken(tokens,RIGHT_PAREN,*line,count); curpos++;break;
        case '{': addSToken(tokens,LEFT_BRACE,*line,count); curpos++;break;
        case '}': addSToken(tokens,RIGHT_BRACE,*line,count); curpos++;break;
        case ',': addSToken(tokens,COMMA,*line,count); curpos++;break;
        case '.': addSToken(tokens,DOT,*line,count); curpos++;break;
        case '-': addSToken(tokens,MINUS,*line,count); curpos++;break;
        case '+': addSToken(tokens,PLUS,*line,count);curpos++; break;
        case ';': addSToken(tokens,SEMICOLON,*line,count); curpos++;break;
        case '*': addSToken(tokens,STAR,*line,count); curpos++;break;
        case '!':
           { if (*(curpos+1) == '=') {addSToken(tokens,BANG_EQUAL,*line,count); curpos+=2;}
            else addSToken(tokens,BANG,*line,count);
            break;}
        case '=':
          {  if (*(curpos+1) == '=') {addSToken(tokens,EQUAL_EQUAL,*line,count); curpos+=2;}
            else addSToken(tokens,EQUAL,*line,count);
            break;}
        case '<':
            { if (*(curpos+1) == '=') {addSToken(tokens,LESS_EQUAL,*line,count); curpos+=2;}
            else addSToken(tokens,LESS,*line,count);
            break;}
        case '>':
       {     if (*(curpos+1) == '=') {addSToken(tokens,GREATER_EQUAL,*line,count); curpos+=2;}
            else addSToken(tokens,GREATER,*line,count);
            break;}
        case '/':
            {// A comment goes until the end of the line.
            if (*(curpos+1) == '/') while (*(++curpos) != '\n' && *(curpos+1) != '\0');
            else if (*(curpos+1) == '*')
            {
                curpos++;
                int clevel=1;
                while(clevel)
                {
                    if(*curpos == '/')
                    {
                        if(*(curpos-1)=='*') clevel--;
                        else if(*(curpos+1)=='*') clevel++;

                    }
                    else if (*curpos == NULL)
                    {
                        return -1;
                    }
                    curpos++;
                }
            }
            else  {addSToken(tokens,SLASH,*line,count);curpos++;}
            
            break;}
        case '"': 
            {char *string = add_const_string(++curpos,line,err);
            if(string == NULL) return -1;
            addValToken(STRING,string,tokens,*line,count);
            break;}
        default:
            if(isDigit(curpos))
            {
                //todo : handle errors
                float *f = get_number_literal(&curpos,line,err);
                if(f == NULL) return -1;
                addValToken(NUMBER,f,tokens,*line,count);
            }
            else if(isAlpha(curpos))
            {
                char *id;
                int iden = 0;
                id = get_identifier(&curpos,err);
                if(check_keyword(id)) iden = 1;
                addKeyToken(id,iden,tokens,*line,count);
            }
            else
            {
                return -1;
            }
    }
    int len = curpos - bufstart;
    return len;
}


char * add_const_string(char *curpos,int *line,char *err)
{
    char *pos = curpos;
    while(*pos != '"')
    {
        if(*pos=='\n') *line++;
        else if(*pos=='\0')
        {
            strcpy(err,"Unterminated string.");
            error(*line, err);
            return NULL;
        }
        pos++;
    }
    int len = pos-curpos;
    char *string = (char *)calloc(0,len+1);
    strncpy(string,curpos,len);
    pos++;
    curpos = pos;
    return string;
}

float *get_number_literal(char **start,int *line,char *err)
{
    char *curpos = *start,*strval;
    while(isDigit(curpos) && *curpos++ != '\0' );
    if(*curpos == '.')
    {
        while(isDigit(++curpos));
    }
    //todo : check if decimals are there after point
    int len = (int)(curpos - *start);
    strval = (char *)malloc(len);
    strncpy(strval,*start,len);
    *start = curpos;
    float f = atof(strval),*f1;
    f1 = &f;
    return f1;

}

char * get_identifier(char **start,char *err)
{
    char *curpos = *start,*strval;
    while(isAlphaNumeric(++curpos));
    int len = curpos - *start;
    strval = (char *)malloc(len);
    strncpy(strval,*start,len);
    *start = curpos;
    return strval;
}

//increment index here for now... refactor later?
void addSToken(tok *tokens,enum TokenType t,int line,int *index)
{
    tokens[*index].type = t;
    tokens[*index].val = NULL;
    tokens[*index].line = line;
    (*index)++;
}

void addValToken(enum TokenType t,void *ptr,tok *tokens,int line,int *index)
{
    tokens[*index].type = t;
    tokens[*index].val = ptr;
    tokens[*index].line = line;
    (*index)++;
}

void addKeyToken(char *id,int identityflag,tok *tokens,int line,int *index)
{
    if (identityflag)  tokens[*index].type = IDENTIFIER;
    int len = strlen(id);
    tokens[*index].lexeme = (char *)malloc(len);
    strcpy(tokens[*index].lexeme,id);
    tokens[*index].line = line;
    (*index)++;
    // *index = *index + 1;
}

// tok * addToken(tok *tokens,enum TokenType t,char *lexeme,void * item,int linecount,int index)
// {
//     tokens[index].type = t;
//     strcpy(tokens[index].lexeme,lexeme);
//     tokens[index].val = item;
//     tokens[index].line = linecount;
//     return tokens;
// }

int check_keyword(char *id)
{
    int i,result;
    lowercase(id);
    for(i=0;i<15;i++)
    {
        
        result = strcmp(id,keywords[i]);
        // printf("comparing %s with %s result : %d\n",id,keywords[i],result);
        if(!result)
            {
                return i+1;
            }
    }
    return 0;
}

int isDigit(char * buf)
{
    if(*buf >= '0' && *buf <= '9')
        return 1;
    return 0;
}

int isAlpha(char *curpos)
{
    int c = *curpos;
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

int isAlphaNumeric(char *curpos)
{
    int c = *curpos;
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
            c == '_';
}

void lowercase(char *p)
{
    for ( ; *p; ++p) *p = tolower(*p);
}