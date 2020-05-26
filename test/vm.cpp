//
// Created by 62358 on 2020/5/26.
//

#include <cstdio>
#include <cstring>
#include "data.h"
#include "vm.h"

int sp;     //栈顶
int bp;     //基址
int pc;     //程序地址寄存器

int codeLength;
instruction arrayStruct[CODE_SIZE];         //存放读入目标代码


instruction ir;     //指令寄存器

int stack[MAX_STACK_HEIGHT];

void vm(char *codefilename) {
    pc = 0;
    bp = 0;
    sp = -1;
    memset(stack,0, sizeof(stack));

    FILE *ifp;
    ifp = fopen(codefilename, "r");
    codeLength = 0;

    while (fscanf(ifp, "%d %d %d", &arrayStruct[codeLength].ins, &arrayStruct[codeLength].l,
                  &arrayStruct[codeLength].m) != EOF) {
        codeLength++;
    }

//    printReg();
//    printStack();

    for (; pc < codeLength - 1;) {
        ir = arrayStruct[pc++];

//        printIr();

        executeCycle();

//        printReg();
//        printStack();
    }

}

void executeCycle() {
    switch (ir.ins) {
        case LIT:
            stack[++sp] = ir.m;
            break;
        case LOD:
            stack[++sp] = stack[base() + ir.m];
            break;
        case STO:
            stack[base() + ir.m] = stack[sp--];
            break;
        case CAL:
            stack[sp + 1] = base();         //SL
            stack[sp + 2] = bp;             //DL
            stack[sp + 3] = pc;             //返回地址

            bp = sp + 1;
            pc = ir.m;
            break;
        case INT:
            sp = sp + ir.m;
            break;
        case JMP:
            pc = ir.m;
            break;
        case JPC:
            if (!stack[sp--])
                pc = ir.m;
            break;
        case OPR:
            opr();
            break;
        default:
            printf("Illegal INS!\n");
    }
}

void opr() {
    int temp;
    switch (ir.m) {
        case ADD:
            temp = stack[sp - 1] + stack[sp];
            stack[--sp] = temp;
            break;
        case SUB:
            temp = stack[sp - 1] - stack[sp];
            stack[--sp] = temp;
            break;
        case DIV:
            temp = stack[sp - 1] / stack[sp];
            stack[--sp] = temp;
            break;
        case MINUS:
            stack[sp] = -stack[sp];
            break;
        case MUL:
            temp = stack[sp - 1] * stack[sp];
            stack[--sp] = temp;
            break;
        case EQ:
            temp = (stack[sp - 1] == stack[sp]);
            stack[--sp] = temp;
            break;
        case NE:
            temp = (stack[sp - 1] != stack[sp]);
            stack[--sp] = temp;
            break;
        case GE:
            temp = (stack[sp - 1] >= stack[sp]);
            stack[--sp] = temp;
            break;
        case GT:
            temp = (stack[sp - 1] > stack[sp]);
            stack[--sp] = temp;
            break;
        case LE:
            temp = (stack[sp - 1] <= stack[sp]);
            stack[--sp] = temp;
            break;
        case LT:
            temp = (stack[sp - 1] < stack[sp]);
            stack[--sp] = temp;
            break;
        case ODD:
            temp = stack[sp] % 2;
            stack[sp] = temp;
            break;
        case WRITE:
            printf("%d\n", stack[sp--]);
            break;
        case READ:
            scanf("%d", &stack[++sp]);
            break;
        case 0:
            sp = bp - 1;
            pc = stack[sp + 3];
            bp = stack[sp + 2];
            break;
        default:
            printf("Illegal OPR!\n");
    }
}

int base() {
    int l = ir.l;
    int SL = bp;

    while (l > 0) {
        SL = stack[SL];
        l--;
    }
    return SL;
}

void printStackFrame() {
    printf("pc:%d\t", pc);
    printf("bp:%d\t", bp);
    printf("sp:%d\t", sp);

    printf("stack:(");
    int max = sp >= bp ? sp : bp;
    for (int i = 0; i <= max; i++)
        if (i)
            printf(" %d", stack[i]);
        else
            printf("%d", stack[i]);
    printf(")\n");

    printf("ir:(%d %d %d)\n", ir.ins, ir.l, ir.m);
}

void printIr()
{
    printf("ir:(%d %d %d)\t", ir.ins, ir.l, ir.m);
    if(ir.ins == OPR)
        printf("(%s %d %s)\n", insStr[ir.ins], ir.l, oprStr[ir.m]);
    else
        printf("(%s %d %d)\n", insStr[ir.ins], ir.l, ir.m);
}

void printReg()
{
    printf("pc:%d\t", pc);
    printf("bp:%d\t", bp);
    printf("sp:%d\t", sp);
}

void printStack()
{
    printf("stack:(");
    int max = sp >= bp ? sp : bp;
    for (int i = 0; i <= max; i++)
        if (i)
            printf(" %d", stack[i]);
        else
            printf("%d", stack[i]);
    printf(")\n");
}