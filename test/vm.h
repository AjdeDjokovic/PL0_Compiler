//
// Created by 62358 on 2020/5/26.
//

#ifndef TEST_VM_H
#define TEST_VM_H

#define MAX_STACK_HEIGHT 2000

void executeCycle();

int base();

void opr();

void printStackFrame();

void printIr();

void printReg();

void printStack();

char* insStr[] = {"","LIT","LOD","STO","CAL","INT","JMP","JPC","OPR"};
char* oprStr[] = {"RET","ADD","SUB","MUL","DIV","GT","LT","GE","LE","NE","EQ","WRITE","READ","MINUS","ODD"};

#endif //TEST_VM_H
