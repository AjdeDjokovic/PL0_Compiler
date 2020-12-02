### PL0编译器

1. 概述

   - 山东大学编译原理课程实验
   - 词法分析lex，语法分析parse，语义分析parse，虚拟机执行vm

2. 使用

   - 进入PL0_Compiler\test\cmake-build-debug目录

   - 命令行输入

     ```shell
     #测试功能，编译执行自带的6个PL0文件
     ./test
     
     #编译执行指定的PL0文件，中间结果存储在同目录下
     ./test filename
     ```

3. 测试功能说明
	> 测试6个文件，输入文件存放在input文件夹
      词法分析文件存放在lex文件夹
      语法分析文件存放在parse文件夹
      符号表文件存放在table文件夹语义分析生成中间代码文件存放在code文件夹
      VM虚拟机运行

	> PL0_code.in文件进行乘除、求公因数、阶乘运算
      PL0_code0.in看不懂，随便输吧
      PL0_code1.in文件输入一个数b，判断b是否为0，如果为0则退出。如果b不为0则输出(b + 10) * 2
      PL0_code2.in输出1到10的乘方
      PL0_code3.in输出100以内的素数
      PL0_code4.in输入一个数b,b!=0时输出（b + 10） * 2,b=0时退出
***
### PL0 compiler

1. Overview
	* Shandong University Compilation Principle Course Experiment
	* Lexical analysis lex, syntax analysis parse, semantic analysis parse, virtual machine execution vm

2. Use
	* Enter the PL0_Compiler\test\cmake-build-debug directory
	* Command line input
		```shell
		#Test function, compile and execute the 6 PL0 files that come with it
		./test

		#Compile and execute the specified PL0 file, and the 				intermediate results are stored in the same directory
		./test filename
		```

3. Test function description
	> Test 6 files, the input files are stored in the input folder
      Lexical analysis files are stored in the lex folder
      The grammatical analysis file is stored in the parse folder
      The symbol table file is stored in the table folder, and the semantic analysis generated intermediate code files are stored in the code folder
      VM virtual machine running

	> PL0_code.in file for multiplication and division, finding common factors, factorial operations
      PL0_code0.in can’t understand, just lose
      Input a number b in the PL0_code1.in file, judge whether b is 0, and exit if it is 0. If b is not 0, output (b + 10) * 2
      PL0_code2.in outputs the power of 1 to 10
      PL0_code3.in outputs prime numbers within 100
      PL0_code4.in input a number b, output when b!=0 (b + 10) * 2, exit when b=0