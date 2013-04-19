mipssim
=======

This is the README for mipssim curriculum project
-------------------------------------------------
Copyright by mengdi wang

mipssim is a curriculum project of Computer Architecture: Spring 2013 @ECNU

INSTALLATION
============

building and installing
------------------------
simply 'make' to build MIPSsim

'make clean' to clean object files.

running
-------
./MIPSsim input
+ input  : found input instruction files, e.g. sample.txt
+ output : disassembly.txt, simulation.txt	

Layout of the source tree
=========================

core directories
----------------
+ mipssim/            		Source of MIPSsim, disambler and simulator
+ mipssim/test/       		Source of TestGen, generating test data
+ mipssim/testdata/   		3 group of test data
+ mipssim/instDecoder.h   
+ mipssim/instDecoder.cpp 	Instruction disassebly component.
+ mipssim/instSimulator.hc
+ mipssim/instSimulator.cpp	Instruction simulation component.
+ mipssim/constdef.h		Definition of some constant numbers.
+ mipssim/main.cpp			main running file.


GitHub
======
https://github.com/mengdiwang/mipssim.git
