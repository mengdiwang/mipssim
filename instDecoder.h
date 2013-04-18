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
#include <memory.h>
#include <vector>
#include <string>
#include "constdef.h"

class Inst
{
public:
    Inst():address(0),other(0),rs(0),rt(0),rd(0),sa(0)
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
    int Output(std::string fileName);
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
    int AddMISP(std::string inststr, Inst &inst);
    int ADDMISPSpec(std::string inststr, Inst &inst, bool &isbreak);
    
private:
    std::vector<Inst> insts;
    std::vector<Data> datas;
};
//--------------------------------------------------------------------------
std::string GetCodeDisplay(Inst &inst);
std::string GetCodeType(Inst &inst);
//--------------------------------------------------------------------------

#endif
