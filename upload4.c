

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <time.h>


int main()
{

        //printf("Content-type: text/plain\n\n");

        printf("Content-type: text/html\n\n");


        char *szBuff;
        time_t t;   // not a primitive datatype
        time(&t);


        printf("\n current Time : %s ", ctime(&t));


        char reqdata[64 * 1024];
        size_t length;
    while ((length = fread(reqdata, 1, 64 * 1024, stdin)) > 0)
    {
        // and feed it to the parser:
       //ultipart_parser_execute(parser, reqdata, length);
        printf("%s",reqdata);

    }



"c1.c" 40L, 609C                                                                                                    1,0-1         Top
