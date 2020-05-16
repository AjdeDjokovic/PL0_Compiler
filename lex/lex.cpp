//
// Created by 62358 on 2020/4/18.
//

#include <cstdio>
#include <cstring>
#include <cctype>
#include "data.h"
#include "lex.h"

const char *reservedWords[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do",
                               "read", "write", "odd"};
const char specialSymbols[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';', ':'};
const char *strPrint[] = {"", "", "", "", "+", "-", "*", "/", "odd", "=", "<>", "<", "<=", ">", ">=", "(", ")", ",",
                          ";", ".", ":=", "begin", "end", "if", "then", "while", "do", "call", "const", "var",
                          "procedure", "write", "read", "else"};

tokenStruct lexList[5000];
int lexListIndex = 0;

int reserve(char *str) {
    for (int i = 0; i < ReservedWordsLength; i++)
        if (!strcmp(str, reservedWords[i]))
            return i;
    return -1;
}

int speial(char c) {
    for (int i = 0; i < SpecialSymbolsLength; i++)
        if (c == specialSymbols[i])
            return i;
    return -1;
}

void lex(char *infilename, char *outfilename) {
    FILE *ifp;
    FILE *ofp;

    ifp = fopen(infilename, "r");
    ofp = fopen(outfilename, "w");

    //检察错误
    int errorHolder;
    //读入字符
    int c;
    int comments = 0;
    //是否需要读入下一个字符,1表示已经读入且不需要读入，0表示需要读入
    int lookAhead = 1;

    c = fgetc(ifp);

    while (c != EOF) {
        //处理空白字符
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            c = fgetc(ifp);
            lookAhead = 1;
            continue;
        }
        if (isalpha(c)) {
            //临时存放输入字符串
            char characterString[StrLength];
            memset(characterString, 0, sizeof(characterString));

            //读入位置
            int index = 0;
            characterString[index++] = c;
            lookAhead = 1;

            while (isalpha(c = fgetc(ifp)) || isdigit(c)) {
                if (index >= StrLength - 1) {
                    printf("Error 3: Name too long.\n");
                    //读入多余字符,超过StrLength-1个字符予以忽略
                    while (isalpha(c = fgetc(ifp)) || isdigit(c));
                    errorHolder = 1;
                    break;
                }
                characterString[index++] = c;
            }

            //出现错误，不接受输入单词
            if (errorHolder == 1) {
                errorHolder = 0;
                continue;
            }

            switch (reserve(characterString)) {
                //const
                case 0:
                    lexList[lexListIndex].tokenID = constsym;
                    break;
                    //var
                case 1:
                    lexList[lexListIndex].tokenID = varsym;
                    break;
                    //procedure
                case 2:
                    lexList[lexListIndex].tokenID = procsym;
                    break;
                    //call
                case 3:
                    lexList[lexListIndex].tokenID = callsym;
                    break;
                    //begin
                case 4:
                    lexList[lexListIndex].tokenID = beginsym;
                    break;
                    //end
                case 5:
                    lexList[lexListIndex].tokenID = endsym;
                    break;
                    //if
                case 6:
                    lexList[lexListIndex].tokenID = ifsym;
                    break;
                    //then
                case 7:
                    lexList[lexListIndex].tokenID = thensym;
                    break;
                    //else
                case 8:
                    lexList[lexListIndex].tokenID = elsesym;
                    break;
                    //while
                case 9:
                    lexList[lexListIndex].tokenID = whilesym;
                    break;
                    //do
                case 10:
                    lexList[lexListIndex].tokenID = dosym;
                    break;
                    //read
                case 11:
                    lexList[lexListIndex].tokenID = readsym;
                    break;
                    //write
                case 12:
                    lexList[lexListIndex].tokenID = writesym;
                    break;
                    //odd
                case 13:
                    lexList[lexListIndex].tokenID = oddsym;
                    break;
                default:
                    lexList[lexListIndex].tokenID = identsym;
                    strcpy(lexList[lexListIndex].name, characterString);
                    break;
            }
            lexListIndex++;
        } else if (isdigit(c)) {
            int number = c - '0';
            int d;
            int place = 1;
            lookAhead = 1;

            while (isdigit(c = fgetc(ifp))) {
                if (place > NumMaxLength) {
                    printf("Error 2: Number too long.\n");
                    while (isdigit(c = fgetc(ifp)));
                    errorHolder = 1;
                    break;
                }
                d = c - '0';
                number = 10 * number + d;
                place++;
            }

            //常数不能含有单词
            if (isalpha(c)) {
                printf("Error 1: Variable does not start with letter.\n");
                while (isalpha(c = fgetc(ifp)) || isdigit(c));
                continue;
            }

            //处理常数过长
            if (errorHolder == 1) {
                errorHolder = 0;
                continue;
            }

            lexList[lexListIndex].tokenID = numbersym;
            lexList[lexListIndex++].numberValue = number;
        } else {
            lookAhead = 0;
            switch (speial(c)) {
                //+
                case 0:
                    lexList[lexListIndex++].tokenID = plussym;
                    break;
                    //-
                case 1:
                    lexList[lexListIndex++].tokenID = minussym;
                    break;
                    //*
                case 2:
                    lexList[lexListIndex++].tokenID = multsym;
                    break;
                    //注释/*  */
                case 3:
                    c = fgetc(ifp);
                    lookAhead = 1;
                    if (c == '*') {
                        comments = 1;
                        lookAhead = 0;
                        c = fgetc(ifp);
                        while (comments) {
                            if (c == '*') {
                                c = fgetc(ifp);
                                if (c == '/')
                                    comments = 0;
                            } else
                                c = fgetc(ifp);
                        }
                    } else
                        lexList[lexListIndex++].tokenID = slashsym;
                    break;
                    //(
                case 4:
                    lexList[lexListIndex++].tokenID = lparentsym;
                    break;
                    //)
                case 5:
                    lexList[lexListIndex++].tokenID = rparentsym;
                    break;
                    //=
                case 6:
                    lexList[lexListIndex++].tokenID = eqsym;
                    break;
                    //,
                case 7:
                    lexList[lexListIndex++].tokenID = commasym;
                    break;
                    //.
                case 8:
                    lexList[lexListIndex++].tokenID = periodsym;
                    break;
                    //<> <= <
                case 9:
                    c = fgetc(ifp);
                    if (c == '>') {
                        lexList[lexListIndex++].tokenID = neqsym;
                        lookAhead = 0;
                    } else if (c == '=') {
                        lexList[lexListIndex++].tokenID = leqsym;
                        lookAhead = 0;
                    } else {
                        lexList[lexListIndex++].tokenID = lessym;
                        lookAhead = 1;
                    }
                    break;
                    //>= >
                case 10:
                    c = fgetc(ifp);
                    if (c == '=') {
                        lexList[lexListIndex++].tokenID = geqsym;
                        lookAhead = 0;
                    } else {
                        lexList[lexListIndex++].tokenID = gtrsym;
                        lookAhead = 1;
                    }
                    break;
                    //;
                case 11:
                    lexList[lexListIndex++].tokenID = semicolonsym;
                    break;
                    //:=
                case 12:
                    c = fgetc(ifp);
                    if (c == '=') {
                        lexList[lexListIndex++].tokenID = becomessym;
                        lookAhead = 0;
                    } else {
                        lookAhead = 1;
                        printf("Error 4: Invalid symbols :.\n");
                    }
                    break;
                default:
                    lookAhead = 0;
                    printf("Error 4: Invalid symbols %c.\n", c);
                    break;
            }
        }
        if (!lookAhead) {
            c = fgetc(ifp);
            lookAhead = 1;
        }
    }

//    fprintf(ofp, "%d", lexList[0].tokenID);
//    if (lexList[0].tokenID == 2)
//        fprintf(ofp, "|%s", lexList[0].name);
//    else if (lexList[0].tokenID == 3)
//        fprintf(ofp, "|%d", lexList[0].numberValue);
//    for (int i = 1; i < lexListIndex; i++) {
//        fprintf(ofp, "|%d", lexList[i].tokenID);
//        if (lexList[i].tokenID == 2) {
//            fprintf(ofp, "|%s", lexList[i].name);
//        } else if (lexList[i].tokenID == 3) {
//            fprintf(ofp, "|%d", lexList[i].numberValue);
//        }
//    }

    int id = 0, num = 0;
    for (int i = 0; i < lexListIndex; i++) {
        fprintf(ofp, "%d\t\t", lexList[i].tokenID);

        if (lexList[i].tokenID == 2)
            fprintf(ofp, "%d\t\t", id++);
        else if (lexList[i].tokenID == 3)
            fprintf(ofp, "%d\t\t", num++);
        else fprintf(ofp, "-\t\t");

        if (lexList[i].tokenID == 2)
            fprintf(ofp, "%s\t\t\n", lexList[i].name);
        else if (lexList[i].tokenID == 3)
            fprintf(ofp, "%d\t\t\n", lexList[i].numberValue);
        else
            fprintf(ofp, "%s\t\t\n", strPrint[lexList[i].tokenID]);
    }
    fclose(ifp);
    fclose(ofp);

}
