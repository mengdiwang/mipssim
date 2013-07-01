mipssim
=======

This is the README for mipssim curriculum project2 pipelined MIPS
-----------------------------------------------------------------
Copyright by Mengdi Wang, student No.51121500030

mipssim is a curriculum project of Computer Architecture: Spring 2013 @ECNU
This project is mainly realized by C/C++ codes.

INSTALLATION
============

building and installing
------------------------
simply 'make' to build MIPSsim

'make clean' to clean object files.

Notes:
CRLF is used as default line end char.To changed the end char to LF.
You just need to remove comment of CFLAGS:
let	"CFLAGS=-c #-DLF" becomes "CFLAGS=-c -DLF"
then make clean, make 
	
running
-------
./MIPSsim <input> <output>
<input>  : found input instruction files, e.g. sample.txt
<output> : output file name of the simulation, e.g. simulation.txt	

Layout of the source tree
=========================

core directories
----------------
+ mipssim/            		Source of MIPSsim, disambler and simulator
+ mipssim/test/       		Source of TestGen, generating test data
+ mipssim/testdata/   		4 group of test data, tX.txt is the test file, sX.txt is the result
+ mipssim/testgen/			test data generation and some examples
+ mipssim/instDecoder.h   
+ mipssim/instDecoder.cpp 	Instruction disassebly component.
+ mipssim/instSimulator.hc
+ mipssim/instSimulator.cpp	Instruction simulation component.
+ mipssim/constdef.h		Definition of some constant numbers.
+ mipssim/main.cpp			main running file.
+ mipssim/sbplSimulator.h
+ mipssim/sbplSimulator.cpp Scoreboarding simulation compnent



GitHub
======
https://github.com/mengdiwang/mipssim.git
