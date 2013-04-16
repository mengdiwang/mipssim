//
//  instDecoder.h
//  mispsim
//
//  Created by Mengdi Wang on 13-4-14.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//
//--------------------------------------------------------------------------
#ifndef mispsim_instDecoder_h
#define mispsim_instDecoder_h
//--------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
//--------------------------------------------------------------------------
const int ADDBASE       = 64;
const int INSTLENGTH    = 32;
const std::string CAT1  = "01";
const std::string CAT2  = "11";
//--------------------------------------------------------------------------
const std::string typestr[28] = {"J", "JR", "BEQ", "BLTZ", "BGTZ", "BREAK", "SW", "LW", "SLL", "SRL", "STA", "NOP",
                        "ADD", "SUB", "MUL", "AND", "OR", "XOR", "NOR", "SLT", "ADD", "AND", "ORI", "XORI", "SUB", "MUL", "NOR", "SLT"};
//--------------------------------------------------------------------------
enum insttype
{
    NIL     = -1,
    J 		= 0,
    JR 		= 1,
    BEQ 	= 2,
    BLTZ	= 3,
    BGTZ	= 4,
    BREAK	= 5,
    SW		= 6,
    LW		= 7,
    SLL		= 8,
    SRL		= 9,
    SRA		= 10,
    NOP		= 11,
    ADD		= 12,
    SUB		= 13,
    MUL		= 14,
    AND		= 15,
    OR		= 16,
    XOR		= 17,
    NOR		= 18,
    SLT		= 19,
    ADDI	= 20,
    ANDI	= 21,
    ORI		= 22,
    XORI	= 23,
    SUBI    = 24,
    MULI    = 25,
    NORI    = 26,
    SLTI    = 27
};
//--------------------------------------------------------------------------
class Inst
{
public:
    Inst():address(0),rs(0),rt(0),rd(0),other(0)
    {
        memset(code, 0, sizeof(code));
    }
    std::string OutType();
    
public:
    unsigned int address;
    unsigned int other;
    insttype type;
    unsigned short rs;
    unsigned short rt;
    unsigned short rd;
    unsigned short sa;
    //short imvalue;
    char code[32+2];
};
//--------------------------------------------------------------------------
class Data
{
public:
    Data():data(0),address(0)
    {
        memset(code, 0, sizeof(code));
    }
public:
    int data;
    unsigned int address;
    char code[32+2];
};
//--------------------------------------------------------------------------
class InstDecoder {
public:
    InstDecoder()
    {
    };
    
    int Decode(const char *inFile);
    int Output(const char *outFile);
    const std::vector<Inst> &GetInsts()
    {
        return insts;
    }
    
    const std::vector<Data> &GetDatas()
    {
        return datas;
    }
    
private:
    int ParseFile(std::string inFile);
    //int AddC1type(std::string inststr, Inst &inst, short &isvalid);
    int AddC2type(std::string inststr, Inst &inst, short &isvalid);
    int AddMISP(std::string inststr, Inst &inst, short &isvalid);
    int ADDMISPSpec(std::string inststr, Inst &inst, short &isvalid);
    
private:
    std::vector<Inst> insts;
    std::vector<Data> datas;
};
//--------------------------------------------------------------------------
char* GetCodeDisplay(Inst &inst);
std::string GetCodeType(Inst &inst);
//--------------------------------------------------------------------------

#endif
