//
//  instSimulator.cpp
//  MISPsim
//
//  Created by Mengdi Wang on 13-4-16.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#define TEST

#include "instSimulator.h"
#include <sstream>
#include <fstream>
#include <cmath>

#ifdef TEST
#include <iostream>
#endif

inline int GetInstIndex(int address)
{
    return (address - ADDBASE) >> IMMOFFSET;
}

int InstSim::Run(InstDecoder &instdec)
{
    int ret = 0;
    int cycle = 1;
    int codeidx = 0;
    bool isBreak = false;
    std::stringstream stepoutput;
    
    SetMem(instdec.GetDatas());
    
    while(true)
    {
        if(isBreak)
            break;
        
        bool jump = false;
        
        Inst i= instdec.GetInsts()[codeidx];
        switch (i.type)
        {
            case insttype(J)://J
                jump = true;
                codeidx = GetInstIndex(i.other);
                break;
            case insttype(JR)://JR
                jump = true;
                codeidx = GetInstIndex(r[i.rs]);
                break;
            case insttype(BEQ)://BEQ
            {
                if(r[i.rs] == r[i.rt])
                    codeidx += i.other >> 2;
            }
                break;
            case insttype(BLTZ)://BLTZ
            {
                if(r[i.rs] < 0)
                    codeidx += i.other >> 2;
            }
                break;
            case insttype(BGTZ)://BGTZ
            {
                if(r[i.rs] > 0)
                    codeidx  += i.other >> 2;
            }
                break;
            case insttype(BREAK)://BREAK
                isBreak = true;
                break;
            case insttype(SW)://SW
                SetMembyAddr(r[i.rs] + i.other, r[i.rt]); break;
            case insttype(LW)://LW rt <- memory[r[base] + offset]
            {
                int data = 0;
                if(GetMembyAddr(r[i.rs] + i.other, data))
                    r[i.rt] = data;
            }
                break;
            case insttype(SLL)://SLL
                r[i.rd] = r[i.rt] << i.sa; break;
            case insttype(SRL)://SRL
                r[i.rd] = (signed)((unsigned)r[i.rt] >> i.sa); break; //Logic right shift does not remain sign bit
            case insttype(SRA)://SRA
                r[i.rd] = r[i.rt] >> i.sa; break;
            case insttype(NOP)://NOP
                break;
            case insttype(ADD)://ADD
                r[i.rd] = r[i.rs] + r[i.rt]; break;
            case insttype(SUB)://SUB
                r[i.rd] = r[i.rs] - r[i.rt]; break;
            case insttype(MUL)://MUL
                r[i.rd] = r[i.rs] * r[i.rt]; break;
            case insttype(AND)://AND
                r[i.rd] = r[i.rs] & r[i.rt]; break;
            case insttype(OR)://OR
                r[i.rd] = r[i.rs] | r[i.rt]; break;
            case insttype(XOR)://XOR
                r[i.rd] = r[i.rs] ^ r[i.rt]; break;
            case insttype(NOR)://NOR
                r[i.rd] = (signed)(~((unsigned)r[i.rs] | (unsigned)r[i.rt])); break;
            case insttype(SLT)://SLT
                r[i.rd] = (r[i.rs] < r[i.rs])?1:0; break;
            case insttype(ADDI)://ADDI
                r[i.rt] = r[i.rs] + i.other; break;
            case insttype(ANDI)://ANDI
                r[i.rt] = r[i.rs] & i.other; break;
            case insttype(SUBI)://SUBI
                r[i.rt] = r[i.rs] - i.other; break;
            case insttype(MULI)://MULI
                r[i.rt] = r[i.rs] * i.other; break;
            case insttype(NORI)://NORI
                r[i.rt] = (signed)(~((unsigned)r[i.rs] | (unsigned)i.other)); break;
            case insttype(SLTI)://SLTI
                r[i.rt] = (r[i.rs] < i.other)?1:0; break;
            default:
                break;
        }
        
        //---------------------------------------------------------
        //step output
        //---------------------------------------------------------
        
        stepoutput << "--------------------\n";
        stepoutput << "Cycle:"<< cycle << "\t" << i.address << "\t" << GetCodeType(i) << '\t' << GetCodeDisplay(i) << "\n\nRegisters";
        
        stepoutput << "\nR00:";
        for(int j=0; j<(REGISTERNUM>>1); j++)
            stepoutput << '\t' << r[j];
        
        stepoutput << "\nR16:";
        for(int j=REGISTERNUM>>1; j<REGISTERNUM; j++)
            stepoutput << '\t' << r[j];
        
        stepoutput << "\n\nData\n";
        int memsize = std::ceil((mem.size() + 0.0) / 8);
        for(int j=0; j<memsize; j++)
        {
            stepoutput << mem[j<<3].address << ':';
            for(int k=0; k<8; k++)
            {
                int memidx = (j<<3)+k;
                if(memidx < mem.size())
                    stepoutput <<'\t' << mem[memidx].data;
            }
            stepoutput << '\n';
        }
        stepoutput << '\n';
        
        //---------------------------------------------------------
        if(!jump)
            codeidx++;
        
        cycle ++;
       
    }
    
#ifdef TEST
    std::cout<< stepoutput.str() << std::endl;
#endif
    output.append(stepoutput.str());
    return ret;
}

int InstSim::Output(std::string fileName)
{
    std::ofstream outstream(fileName.c_str());
    
    if(outstream.is_open())
    {
        outstream << output << std::flush;
    }
    outstream.close();
    
    return 0;
}
