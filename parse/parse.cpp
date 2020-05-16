//
// Created by 62358 on 2020/4/28.
//

/*Clayton Walker
 COP 3402
 Due: 7/21/13
 Parser
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "data.h"
#include "parse.h"

symbol table[MAX_SYMBOL_TABLE_SIZE];
instruction code[CODE_SIZE];

int tx, cx, lev = -1;

int dx = 3;

int lexemeListIndex = 0;        //当前读入位置
tokenStruct curToken;           //最新读入tokenStruct

int blank = 0;                      //打印空格数
FILE *ofp;

void parse(char *outfilename) {
    table[0].level = -1;

    ofp = fopen(outfilename, "w");

    program();

    for (int i = 0; i < 30; i++)
        for (i = 0; i < 30; i++) {
            printf("kind:%d\tval:%d\tlevel:%d\taddr:%d\t\tname:%s\n", table[i].kind, table[i].val, table[i].level,
                   table[i].addr, table[i].name);
        }
}

void program() {
    printBlank();
    fprintf(ofp, "program\n");
    blank++;

    getNextToken();
    block();

    if (curToken.tokenID != periodsym)
        error(1);       //Period expected.

    printBlank();
    fprintf(ofp, ".\n");

    blank--;
}

void block() {
    printBlank();
    fprintf(ofp, "block\n");
    blank++;

    lev++;
    if (lev > MAX_LEXI_LEVELS)
        error(2);      //errors for above max lexi level

    int oldDx = dx;
    dx = 3;

    do {
        if (curToken.tokenID == constsym) {
            constDeclaration();
        }
        if (curToken.tokenID == varsym) {
            varDeclaration();
        }
        while (curToken.tokenID == procsym) {
            procedureDeclaration();
        }
    } while ((curToken.tokenID == constsym) || (curToken.tokenID == varsym) || (curToken.tokenID == procsym));

    statement();

    dx = oldDx;
    lev--;

    blank--;
}

void constDeclaration() {
    printBlank();
    fprintf(ofp, "constDeclaration\n");
    blank++;

    printBlank();
    fprintf(ofp, "const\n");

    getNextToken();
    constDefinition();
    while (curToken.tokenID == commasym) {
        printBlank();
        fprintf(ofp, ",\n");

        getNextToken();
        constDefinition();
    }
    if (curToken.tokenID == semicolonsym) {
        printBlank();
        fprintf(ofp, ";\n");

        getNextToken();
    } else {
        error(3);       //常量声明部分缺少;
    }

    blank--;
}

void constDefinition() {
    printBlank();
    fprintf(ofp, "constDefinition\n");
    blank++;

    if (curToken.tokenID == identsym) {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        getNextToken();
        if ((curToken.tokenID == eqsym) || (curToken.tokenID == becomessym)) {
            if (curToken.tokenID == becomessym)
                error(6);       //Use = instead of := in constDefinition

            printBlank();
            fprintf(ofp, ":=\n");

            getNextToken();
            if (curToken.tokenID == numbersym) {
                printBlank();
                fprintf(ofp, "%d\n", curToken.numberValue);

                enter(1);
                getNextToken();
            } else {
                error(8);       //常量定义赋值的不是整数
            }

        } else {
            error(7);           //常量未赋值
        }
    } else {
        error(5);       //常量定义缺少标识符
    }

    blank--;
}

void varDeclaration() {
    printBlank();
    fprintf(ofp, "varDeclaration\n");
    blank++;

    printBlank();
    fprintf(ofp, "var\n");

    getNextToken();
    varDefinition();
    while (curToken.tokenID == commasym) {
        printBlank();
        fprintf(ofp, ",\n");

        getNextToken();
        varDefinition();
    }
    if (curToken.tokenID == semicolonsym) {
        printBlank();
        fprintf(ofp, ";\n");

        getNextToken();
    } else {
        error(4); //Semicolon or comma missing.
    }

    blank--;
}

void varDefinition() {
    if (curToken.tokenID == identsym) {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        enter(2);
        getNextToken();
    } else
        error(9);
}

void procedureDeclaration() {
    printBlank();
    fprintf(ofp, "procedureDeclaration\n");
    blank++;

    procedureHead();
    block();

    if (curToken.tokenID == semicolonsym) {
        printBlank();
        fprintf(ofp, ";\n");

        getNextToken();
    } else
        error(33);                  //; not find in procedureDeclaration()

    blank--;
}

void procedureHead() {
    printBlank();
    fprintf(ofp, "procedureHead\n");
    blank++;

    printBlank();
    fprintf(ofp, "procedure\n");

    getNextToken();
    if (curToken.tokenID == identsym) {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        enter(3);
        getNextToken();

        if (curToken.tokenID == semicolonsym) {
            printBlank();
            fprintf(ofp, ";\n");

            getNextToken();
        } else
            error(32);                  //; not find in procedureHead()
    } else
        error(31);                      //identsym not find in procedureHead()

    blank--;
}

void statement() {
    printBlank();
    fprintf(ofp, "statement\n");
    blank++;

    //赋值语句
    if (curToken.tokenID == identsym) {
        assignment();
    } else if (curToken.tokenID == callsym) {
        callProcedure();
    } else if (curToken.tokenID == ifsym) {
        conditionStatement();
    } else if (curToken.tokenID == beginsym) {
        compoundStatement();
    } else if (curToken.tokenID == whilesym) {
        loopStatement();
    } else if (curToken.tokenID == writesym) {
        writeStatement();
    } else if (curToken.tokenID == readsym) {
        readStatement();
    }

    blank--;
}

void assignment() {
    printBlank();
    fprintf(ofp, "assignment\n");
    blank++;

    printBlank();
    fprintf(ofp, "%s\n", curToken.name);

    int i = position();
    if (i == 0)
        error(10);          //identsym not find in table in assignment
    else if (table[i].kind != 2)
        error(11);          //identsym in assignment must be var

    getNextToken();
    if (curToken.tokenID == becomessym) {
        printBlank();
        fprintf(ofp, ":=\n");

        getNextToken();
        expression();
    } else
        error(12);          //becomessym not find in assignment

    blank--;
}

void callProcedure() {
    printBlank();
    fprintf(ofp, "callProcedure\n");
    blank++;

    printBlank();
    fprintf(ofp, "call\n");

    getNextToken();
    if (curToken.tokenID != identsym)
        error(13);          //identsym not find in callProcedure
    else {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        int i = position();
        if (i == 0)
            error(14);      //identsym not find in table in callProcedure
        else if (table[i].kind != 3)
            error(15);      //identsym called must be procedure
        else {
            getNextToken();
        }
    }

    blank--;
}

void conditionStatement() {
    printBlank();
    fprintf(ofp, "conditionStatement\n");
    blank++;

    printBlank();
    fprintf(ofp, "if\n");

    getNextToken();
    condition();

    if (curToken.tokenID == thensym) {
        printBlank();
        fprintf(ofp, "then\n");

        getNextToken();
    } else
        error(16);                  //then not find in conditionStatement

    statement();

    blank--;
}

void compoundStatement() {
    printBlank();
    fprintf(ofp, "compoundStatement\n");
    blank++;

    printBlank();
    fprintf(ofp, "begin\n");

    getNextToken();

    statement();
    while (curToken.tokenID == semicolonsym) {
        printBlank();
        fprintf(ofp, ";\n");

        getNextToken();
        statement();
    }

    if (curToken.tokenID == endsym) {
        printBlank();
        fprintf(ofp, "end\n");

        getNextToken();
    } else {
        error(17);                  //end not find in compoundStatement()
    }

    blank--;
}

void loopStatement() {
    printBlank();
    fprintf(ofp, "loopStatement\n");
    blank++;

    printBlank();
    fprintf(ofp, "while\n");

    getNextToken();

    condition();

    if (curToken.tokenID == dosym) {
        printBlank();
        fprintf(ofp, "do\n");

        getNextToken();
    } else
        error(18);                      //do not find in loopStatement()

    statement();

    blank--;
}

void writeStatement() {
    printBlank();
    fprintf(ofp, "writeStatement\n");
    blank++;

    printBlank();
    fprintf(ofp, "write\n");

    getNextToken();

    if (curToken.tokenID == lparentsym) {
        printBlank();
        fprintf(ofp, "(\n");

        getNextToken();
        expression();

        while (curToken.tokenID == commasym) {
            printBlank();
            fprintf(ofp, ",\n");

            getNextToken();
            expression();
        }

        if (curToken.tokenID == rparentsym) {
            printBlank();
            fprintf(ofp, ")\n");

            getNextToken();
        } else
            error(21);              //) not find in writeStatement()
    } else
        error(20);                  //( not find in writeStatement()

    blank--;
}

void readStatement() {
    printBlank();
    fprintf(ofp, "readStatement\n");
    blank++;

    printBlank();
    fprintf(ofp, "read\n");

    getNextToken();

    if (curToken.tokenID == lparentsym) {
        printBlank();
        fprintf(ofp, "(\n");

        getNextToken();
        identifier();

        while (curToken.tokenID == commasym) {
            printBlank();
            fprintf(ofp, ",\n");

            getNextToken();
            identifier();
        }

        if (curToken.tokenID == rparentsym) {
            printBlank();
            fprintf(ofp, ")\n");

            getNextToken();
        } else
            error(25);              //) not find in readStatement()
    } else
        error(24);                  //( not find in readStatement()

    blank--;
}

void identifier() {
    if (curToken.tokenID == identsym) {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        int i = position();
        if (i == 0)
            error(22);                      //identifier is not find in table in identifier()
        else if (table[i].kind != 2)
            error(23);                      //identifier is not var in identifier()

        getNextToken();
    } else {
        error(19);                          //identifier not find in identifier()
    }
}

void condition() {
    printBlank();
    fprintf(ofp, "condition\n");
    blank++;

    if (curToken.tokenID == oddsym) {
        printBlank();
        fprintf(ofp, "odd\n");

        getNextToken();
        expression();
    } else {
        expression();

        if ((curToken.tokenID != eqsym) && (curToken.tokenID != neqsym) && (curToken.tokenID != lessym) &&
            (curToken.tokenID != leqsym) && (curToken.tokenID != gtrsym) && (curToken.tokenID != geqsym))
            error(26);                  //relationSwitch not find in condition()
        else {
            printBlank();
            fprintf(ofp, "%s\n", strPrint[curToken.tokenID]);

            getNextToken();
            expression();
        }
    }

    blank--;
}

void expression() {
    printBlank();
    fprintf(ofp, "expression\n");
    blank++;

    if (curToken.tokenID == plussym || curToken.tokenID == minussym) {
        printBlank();
        fprintf(ofp, "%s\n", strPrint[curToken.tokenID]);

        getNextToken();
        term();
    } else {
        term();
    }

    while (curToken.tokenID == plussym || curToken.tokenID == minussym) {
        printBlank();
        fprintf(ofp, "%s\n", strPrint[curToken.tokenID]);

        getNextToken();
        term();
    }

    blank--;
}

void term() {
    printBlank();
    fprintf(ofp, "term\n");
    blank++;

    factor();
    while (curToken.tokenID == multsym || curToken.tokenID == slashsym) {
        printBlank();
        fprintf(ofp, "%s\n", strPrint[curToken.tokenID]);

        getNextToken();
        factor();
    }

    blank--;
}

void factor() {
    printBlank();
    fprintf(ofp, "factor\n");
    blank++;

    if (curToken.tokenID == identsym) {
        printBlank();
        fprintf(ofp, "%s\n", curToken.name);

        int i = position();
        if (i == 0)
            error(27);                  //identsym not find in table in factor()
        else if (table[i].kind == 3)
            error(28);                  //identsym in factor() cannot be procedure

        getNextToken();
    } else if (curToken.tokenID == numbersym) {
        printBlank();
        fprintf(ofp, "%d\n", curToken.numberValue);

        getNextToken();
    } else if (curToken.tokenID == lparentsym) {
        printBlank();
        fprintf(ofp, "(\n");

        getNextToken();
        expression();

        if (curToken.tokenID == rparentsym) {
            printBlank();
            fprintf(ofp, ")\n");

            getNextToken();
        } else
            error(29);                      //) not find in factor()
    } else
        error(30);                          //factor() cannot be right

    blank--;
}

void getNextToken() {
    curToken.tokenID = lexList[lexemeListIndex].tokenID;

    //var
    if (curToken.tokenID == 2)
        strcpy(curToken.name, lexList[lexemeListIndex].name);
    else if (curToken.tokenID == 3)
        curToken.numberValue = lexList[lexemeListIndex].numberValue;

    lexemeListIndex++;
}

void printBlank() {
    for (int i = 0; i < blank; i++)
        fprintf(ofp, "\t");
}

void enter(int kind) {
    tx++;

    strcpy(table[tx].name, curToken.name);
    table[tx].kind = kind;

    //const
    if (kind == 1)
        table[tx].val = curToken.numberValue;
    else if (kind == 2) {
        table[tx].level = lev;
        table[tx].addr = dx;
        dx++;
    } else if (kind == 3)
        table[tx].level = lev;
}

int position() {
    int curLev = lev;

    for (int i = tx; i > 0; i--) {
        if (table[i].kind == 3 && table[i].level == curLev - 1)
            curLev--;

        if (table[i].level > curLev)
            continue;

        if (strcmp(table[i].name, curToken.name) == 0)
            return i;
    }
    return 0;
}

void error(int errorCase) {
    switch (errorCase) {
        case 1:
            printf("Error 1: ");
            printf("Period(.) expected in end of program\n");               //program不以句号结尾
            break;
        case 2:
            printf("Error 2: ");
            printf("Level is larger than the maximum allowed lexicographical levels!\n");   //层数过大
            break;
        case 3:
            printf("Error 3: ");
            printf("Semicolon(;) missing in constDeclaration.\n");     //常量声明部分缺少;
            break;
        case 4:
            printf("Error 4: ");
            printf("Semicolon(;) missing in varDeclaration.\n");        //变量声明部分缺少;
            break;
        case 5:
            printf("Error 5: ");
            printf("Identifier missing in constDefination.\n");         //常量定义缺少标识符
            break;
        case 6:
            printf("Error 6: ");
            printf("Use = instead of := in constDefination.\n");
            break;
        case 7:
            printf("Error 7: ");
            printf("Constant not assigned in constDefination.\n");      //常量定义未赋值
            break;
        case 8:
            printf("Error 8: ");
            printf("Constant not assigned by num in constDefination.\n");    //常量定义赋值的不是整数
            break;
        case 9:
            printf("Error 9: ");
            printf("Identifier missing in varDefination.\n");               //变量定义缺少标识符
            break;
        case 10:
            printf("Error 10: ");
            printf("identsym not find in table in assignment.\n");          //赋值语句中变量没有在表中找到
            break;
        case 11:
            printf("Error 11: ");
            printf("identsym in assignment must be var.\n");                //赋值语句中必须为变量
            break;
        case 12:
            printf("Error 12: ");
            printf("becomessym not find in assignment.\n");                 //赋值语句中没有:=
            break;
        case 13:
            printf("Error 13: ");
            printf("identsym not find in callProcedure.\n");                //过程调用必须有过程名
            break;
        case 14:
            printf("Error 14: ");
            printf("identsym not find in table in callProcedure.\n");       //过程名没有找到
            break;
        case 15:
            printf("Error 15: ");
            printf("identsym called must be procedure in callProcedure().\n");                 //调用的过程不是过程
            break;
        case 16:
            printf("Error 16: ");
            printf("then not find in conditionStatement().\n");                //条件语句中没有then
            break;
        case 17:
            printf("Error 17: ");
            printf("end not find in compoundStatement().\n");                   //复合语句中没有end
            break;
        case 18:
            printf("Error 18: ");
            printf("do not find in loopStatement().\n");                        //循环语句中没有找到do
            break;
        case 19:
            printf("Error 19: ");
            printf("identifier not find in identifier().\n");                   //identifier()没有标识符
            break;
        case 20:
            printf("Error 20: ");
            printf("( not find in writeStatement().\n");                        //写语句中没找到(
            break;
        case 21:
            printf("Error 21: ");
            printf(") not find in writeStatement().\n");                        //写语句中没找到)
            break;
        case 22:
            printf("Error 22: ");
            printf("identifier is not find in table in identifier().\n");       //identifier()中标识符查表没有找到
            break;
        case 23:
            printf("Error 23: ");
            printf("identifier is not var in identifier().\n");                 //identifier()标识符不是var
            break;
        case 24:
            printf("Error 24: ");
            printf("( not find in readStatement().\n");                         //读语句中没有(
            break;
        case 25:
            printf("Error 25: ");
            printf(") not find in readStatement().\n");                         //读语句中没有)
            break;
        case 26:
            printf("Error: 26 ");
            printf("relationSwitch not find in condition().\n");                //条件中没有关系运算符
            break;
        case 27:
            printf("Error: 27 ");
            printf("identsym not find in table in factor().\n");                //因子 标识符没有在表中
            break;
        case 28:
            printf("Error: 28 ");
            printf("identsym in factor() cannot be procedure.\n");              //因子 标识符不能为过程
            break;
        case 29:
            printf("Error: 29 ");
            printf(") not find in factor().\n");              //因子 )没有
            break;
        case 30:
            printf("Error: 30 ");
            printf("factor() cannot be right.\n");              //因子 不匹配
            break;
        case 31:
            printf("Error: 31 ");
            printf("identsym not find in procedureHead().\n");              //过程头中标识符找不到
            break;
        case 32:
            printf("Error: 32 ");
            printf("; not find in procedureHead().\n");              //过程头中 ;找不到
            break;
        case 33:
            printf("Error: 33 ");
            printf("; not find in procedureDeclaration().\n");              //过程说明部分 ;找不到
            break;
        default:
            break;
    }
    //stops program when error occurs
    printf("%d\t%s\n", curToken.tokenID, strPrint[curToken.tokenID]);
    exit(1);
}

