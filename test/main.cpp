#include <iostream>

#include "data.h"
#include <cstring>

void test();

int main(int argc,char *argv[]) {

    //不输入命令行参数时，执行测试函数；如输入pl0文件名，则编译并执行
    if(argc == 1)
        test();
    else if(argc == 2)
    {
        //存放文件名数组
        char stringAry[4][50];
        strcpy(stringAry[0],argv[1]);
        strcpy(stringAry[1],argv[1]);
        strcpy(stringAry[2],argv[1]);
        strcpy(stringAry[3],argv[1]);

        //存放终结过程文件
        strcat(stringAry[0],"lex");
        strcat(stringAry[1],"parse");
        strcat(stringAry[2],"table");
        strcat(stringAry[3],"code");

        lex(argv[1], stringAry[0]);
        parse(stringAry[1],stringAry[2],stringAry[3]);
        vm(stringAry[3]);
    }
    return 0;
}

/*
    测试6个文件，输入文件存放在input文件夹
    词法分析文件存放在lex文件夹
    语法分析文件存放在parse文件夹
    符号表文件存放在table文件夹语义分析生成中间代码文件存放在code文件夹
    VM虚拟机运行

    PL0_code.in文件进行乘除、求公因数、阶乘运算
    PL0_code0.in看不懂，随便输吧
    PL0_code1.in文件输入一个数b，判断b是否为0，如果为0则退出。如果b不为0则输出(b + 10) * 2
    PL0_code2.in输出1到10的乘方
    PL0_code3.in输出100以内的素数
    PL0_code4.in输入一个数b,b!=0时输出（b + 10） * 2,b=0时退出
*/
void test() {
    char *c1[6] = {"input\\PL0_code.in", "input\\PL0_code0.in", "input\\PL0_code1.in", "input\\PL0_code2.in",
                   "input\\PL0_code3.in","input\\PL0_code4.in"};
    char *c2[6] = {"lex\\PL0_code.out", "lex\\PL0_code0.out", "lex\\PL0_code1.out",
                   "lex\\PL0_code2.out", "lex\\PL0_code3.out","lex\\PL0_code4.out"};
    char *c3[6] = {"parse\\PL0_code.out", "parse\\PL0_code0.out", "parse\\PL0_code1.out", "parse\\PL0_code2.out",
                   "parse\\PL0_code3.out","parse\\PL0_code4.out"};
    char *c4[6] = {"table\\PL0_code.out", "table\\PL0_code0.out", "table\\PL0_code1.out", "table\\PL0_code2.out",
                   "table\\PL0_code3.out","table\\PL0_code4.out"};
    char *c5[6] = {"code\\PL0_code.out", "code\\PL0_code0.out", "code\\PL0_code1.out", "code\\PL0_code2.out",
                   "code\\PL0_code3.out","code\\PL0_code4.out"};

    for(int i = 0;i < 6;i++)
    {
        lex(c1[i], c2[i]);
        parse(c3[i],c4[i],c5[i]);
    }

    for(int i = 0;i < 6;i++)
    {
        printf("program%d start\n",i);
        vm(c5[i]);
        printf("program%d end\n",i);
    }
}