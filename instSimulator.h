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
#include "constdef.h"
#include <vector>

class InstSim
{
public:
    InstSim():MemBase(0),output("")
    {
        memset(r, 0, sizeof(r));
    }
    
    int Run(InstDecoder &instdec);
    int Output(std::string fileName);
    
protected:
    void SetMem(const std::vector<Data> &mm)
    {
        mem = mm;
        MemBase = mem[0].address;
    }
    
    int SetMembyAddr(int address, int data)
    {
		int idx = (address-MemBase)>>2;
		if(idx >= 0 && idx < (signed)mem.size())
        {
            mem[(address-MemBase)>>2].data = data;
            return 1;
        }
        return 0;
    }
    
    int GetMembyAddr(int address, int &data)
    {
        int idx = (address-MemBase)>>2;
        if(idx >= 0 && idx < (signed)mem.size())
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

protected:
    int MemBase;
    int r[REGISTERNUM];
    std::vector<Data> mem;
    std::string output;
};

#endif
