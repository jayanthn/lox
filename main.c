#include <stdio.h>
#include <strings.h>


int test()
{
    char msg[6][256] = {"Hello", "C", "World", "from", "VS Code", "and the C++ extension!"};
    int i;
    for (i=0; i<6;i++)
    {
        printf("%s ",msg[i]);
    }

    return 0;

}