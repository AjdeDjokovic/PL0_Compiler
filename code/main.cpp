#include <iostream>

#include "data.h"
#include <cstring>

void lextest() {
    char c1[30] = "input\\test0.txt";
    char c2[30] = "lexoutput\\lexoutput0.txt";
    char c3[30] = "lextest\\lexoutput0.txt";

    for (int i = 0; i < 10; i++) {
        c1[10] = i + '0';
        c2[19] = i + '0';
        c3[17] = i + '0';

        lex(c1, c2);

        FILE *file1 = fopen(c2, "r");
        FILE *file2 = fopen(c3, "r");
        while (fgetc(file1) != fgetc(file2)) {

            printf("false");
            return;
        }
        printf("true\n");
        fclose(file1);
        fclose(file2);
    }
}

int main() {

    char *c1[6] = {"input\\PL0_code.in", "input\\PL0_code0.in", "input\\PL0_code1.in", "input\\PL0_code2.in",
                   "input\\PL0_code3.in","input\\PL0_code4.in"};
    char *c2[6] = {"lex\\PL0_code.out", "lex\\PL0_code0.out", "lex\\PL0_code1.out",
                   "lex\\PL0_code2.out", "lex\\PL0_code3.out","lex\\PL0_code4.out"};
    char *c3[6] = {"parse\\PL0_code.out", "parse\\PL0_code0.out", "parse\\PL0_code1.out", "parse\\PL0_code2.out",
                   "parse\\PL0_code3.out","parse\\PL0_code4.out"};
    char *c4[6] = {"code\\PL0_code.out", "code\\PL0_code0.out", "code\\PL0_code1.out", "code\\PL0_code2.out",
                   "code\\PL0_code3.out","code\\PL0_code4.out"};

    for(int i = 0;i < 6;i++)
    {
        lex(c1[i], c2[i]);
        parse(c3[i],c4[i]);
    }

//    lex(c1[4], c2[4]);
//    parse(c3[4]);

//    char c2[30] = "parserout.txt";
//    char c3[30] = "parserout11.txt";
//    FILE *file1 = fopen(c2, "r");
//    FILE *file2 = fopen(c3, "r");
//    while(fgetc(file1) != fgetc(file2))
//    {
//
//        printf("false");
//        return 0;
//    }
    return 0;
}