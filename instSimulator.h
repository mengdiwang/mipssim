//
//  instSimulator.h
//  MISPsim
//
//  Created by Mengdi Wang on 13-4-16.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#ifndef MISPsim_instSimulator_h
#define MISPsim_instSimulator_h

#include "instDecoder.h"
#include <vector>

const int REGISTERNUM = 32;
const int MEMUNIT = 4;

class InstSim
{
public:
    InstSim():MemBase(0),output("")
    {
        memset(r, 0, sizeof(r));
    }
    
    int Run(InstDecoder &instdec);
    int Output(const char* outFile);
    
private:
    void SetMem(const std::vector<Data> &mm)
    {
        mem = mm;
        MemBase = mem[0].address;
    }
    
    int SetMembyAddr(int address, int data)
    {
        int idx = (address-MemBase)>>2;
        if(idx >= 0 && idx < mem.size())
        {
            mem[(address-MemBase)>>2].data = data;
            return 1;
        }
        return 0;
    }
    
    int GetMembyAddr(int address, int &data)
    {
        int idx = (address-MemBase)>>2;
        if(idx >= 0 && idx <mem.size())
        {
            data = mem[idx].data;
            return 1;
        }
        return 0;
    }
    
    void SetReg(int idx, int data)
    {
        r[idx] = data;
    }

private:
    int MemBase;
    int r[REGISTERNUM];
    std::vector<Data> mem;
    std::string output;
};

#endif
