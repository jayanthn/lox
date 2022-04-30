#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "functions.h"

static int hadError=0;

int main(int argc,char *argv[])
{
    if (argc > 2)
        {
            printf("Usage : lox [filename]");
            return -1;
        }
    else if(argc == 1) readPrompt();
    else readFile(argv[1]);
    hadError = 0;
}

// char* readtillEOL()
// {
//     char *currLine, *start;
//     start = currLine;
//     size_t clen= 0;
//     while(fgets(currLine,256,stdin) != NULL)
//     {
//         currLine = realloc((void *)currLine,clen + (size_t)256);
//         currLine=currLine + 255;
//     }
//     *(++currLine)='\0';
//     return start;
// }


int readPrompt()
{
    char line[4096];
    tok *tokens = (tok *)calloc(0,sizeof(tok *) * 4096);
    if(tokens == NULL) return 1;
    while(1)
    {
        printf(">");
        gets(line);
        if(*line == '\0' || *line == '\4') break;
        printf("Line was %s\n",line);
        runBuf(tokens,line);
    }
    return 0;
}

int readFile(char *filename)
{
    FILE *fp;
    int len,ret;
    char *buf;

    fopen(filename,"rb");
    if(!fp) return -1;
    fseek(fp,0,SEEK_END);
    len = ftell(fp);
    rewind(fp);
    buf = (char *)calloc(0,(size_t)len + 1);
    int readlen = fread(buf,1,(size_t)len,fp);
    fclose(fp);
    if (readlen != len) return -1;
    tok *tokens = (tok *)calloc(0,sizeof(tok *) * 4096);
    if(tokens == NULL) return 1;
    runBuf(tokens,buf);
    return 0;
}


int runBuf(tok * tokens,char *buf)
{
    if (scantokens(tokens,buf)) return -2;
    return 0;
}

void error(int line,char *message)
{
    char where[2];
    strcpy(where,"");
    report(line,where,message);
}

void report(int line,char *where,char *message)
{
    hadError=1;
    printf("[%d] : Error %s : %s",line,where,message);

}

