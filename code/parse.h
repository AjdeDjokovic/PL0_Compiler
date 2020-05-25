//
// Created by 62358 on 2020/4/28.
//

#ifndef COMPILE_PARSE_H
#define COMPILE_PARSE_H

#include "data.h"

/*For constants, you must store kind, name and value.
 For variables, you must store kind, name, L and M.
 For procedures, you must store kind, name, L and M.*/
struct symbol
{
    int kind;       // const = 1, var = 2, proc = 3
    char name[12];  // name up to 11 chars
    int val;        // number (ASCII value)
    int level;      // L level
    int addr;       // M address
};

void program();             //程序
void block();               //分程序
void constDeclaration();    //常量说明部分
void constDefinition();     //常量定义
void varDeclaration();      //变量说明部分
void varDefinition();       //变量定义
void procedureDeclaration();//过程说明部分
void procedureHead();       //过程首部
void statement();           //语句
void condition();           //条件
void expression();          //表达式
void term();                //项
void factor();              //因子

void assignment();          //赋值语句
void callProcedure();       //过程调用语句
void conditionStatement();  //条件语句
void compoundStatement();   //复合语句
void loopStatement();       //循环语句
void writeStatement();      //写语句
void readStatement();       //读语句
void identifier();

void enter(int kind);
void error(int errorCase);
void emit(ins_type ins, int l, int m);
int position();

void getNextToken();

void printBlank();

#endif //COMPILE_PARSE_H
