//
// Created by 62358 on 2020/4/18.
//

#ifndef COMPILE_DATA_H
#define COMPILE_DATA_H

#define StrLength 15
#define CODE_SIZE 1000

#define MAX_LEXI_LEVELS 3
#define MAX_SYMBOL_TABLE_SIZE 1000


#define MAX_STRPRINT_LENGTH 34

enum token_type
{
    nulsym = 1,
    identsym,       //标识符
    numbersym,      //常数
    plussym,        //+
    minussym,       //-
    multsym,        //*
    slashsym,       ///
    oddsym,         //odd
    eqsym,          //=
    neqsym,         //#
    lessym,         //<
    leqsym,         //<=
    gtrsym,         //>
    geqsym,         //>=
    lparentsym,     //(
    rparentsym,     //)
    commasym,       //,
    semicolonsym,   //;
    periodsym,      //.
    becomessym,     //:=
    beginsym,       //begin
    endsym,         //end
    ifsym,          //if
    thensym,        //then
    whilesym,       //while
    dosym,          //do
    callsym,        //call
    constsym,       //const
    varsym,         //var
    procsym,        //procedure
    writesym,       //write
    readsym,        //read
    elsesym         //else
};

const extern char *strPrint[MAX_STRPRINT_LENGTH];


struct tokenStruct
{
    token_type tokenID;
    int numberValue;
    char name[StrLength];
};

extern tokenStruct lexList[5000];
extern int lexListIndex;

void lex(char * infilename,char *outfilename);

enum ins_type
{
    LIT = 1,
    LOD ,
    STO,
    CAL,
    INT,
    JMP,
    JPC,
    OPR
};

enum op_type
{
    ADD = 1,
    SUB,
    MUL,
    DIV,
    GT,
    LT,
    GE,
    LE,
    NE,
    EQ,
    WRITE,
    READ,
    MINUS,
    ODD
};

struct instruction{
    ins_type ins; //Opcode
    int l;  //L
    int m;  //M
};

extern instruction code[CODE_SIZE];

void parse(char *parsefilename,char *tablefilename,char *codefilename);

void vm(char *codefilename);



#endif //COMPILE_DATA_H
