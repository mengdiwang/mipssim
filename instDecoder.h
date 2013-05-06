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
std::string GetCodeDisplay(char sip, Inst &inst);
std::string GetCodeDisplaySb(char sip, Inst &inst, char sipl);
std::string GetCodeType(Inst &inst);
//--------------------------------------------------------------------------

#endif
