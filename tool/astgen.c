#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


char exprtokens[][256] = 
    {"Binary   : Expr left, Token operator, Expr right",
    "Grouping : Expr expression", 
    "Literal  : Object value",
    "Unary    : Token operator, Expr right" };


int main(int argc,char *argv[])
{
    FILE *fp;
    fp = fopen("./out.txt","wb");
    // expr(fp);

}

void genast(FILE *fp)
{
    char *type = exprtokens;
    

}  

/*
        expression  → literal
                    |   unary
                    | binary
                    | grouping ;

        literal     → NUMBER | STRING | "true" | "false" | "nil" ;
        grouping    → "(" expression ")" ;
        unary       → ( "-" | "!" ) expression ;
        binary      → expression operator expression ;
        operator    → "==" | "!=" | "<" | "<=" | ">" | ">=" | "+"  | "-"  | "*" | "/" ;
*/

/*

      "Binary   : Expr left, Token operator, Expr right",
      "Grouping : Expr expression",
      "Literal  : Object value",
      "Unary    : Token operator, Expr right"
*/