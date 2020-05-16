#include <iostream>

#include "data.h"
#include <cstring>
void lextest()
{
    char c1[30] = "input\\test0.txt";
    char c2[30] = "lexoutput\\lexoutput0.txt";
    char c3[30] = "lextest\\lexoutput0.txt";

    for(int i = 0;i < 10;i++)
    {
        c1[10] = i + '0';
        c2[19] = i + '0';
        c3[17]= i + '0';

        lex(c1,c2);

        FILE *file1 = fopen(c2, "r");
        FILE *file2 = fopen(c3, "r");
        while(fgetc(file1) != fgetc(file2))
        {

            printf("false");
            return;
        }
        printf("true\n");
        fclose(file1);
        fclose(file2);
    }
}

int main() {

    lex("input\\test0.txt","lexoutput\\lexoutput0.txt");
    return 0;
}