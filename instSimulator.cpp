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

#ifdef TEST
#include <iostream>
#endif


const int STEPSTRLEN = 500;

inline int GetInstIndex(int address)
{
    return (address - ADDBASE) >> 2;
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
            case 0://J
                jump = true;
                codeidx = GetInstIndex(i.other);
                break;
            case 1://JR
                jump = true;
                codeidx = GetInstIndex(r[i.rs]);
                break;
            case 2://BEQ
            {
                if(r[i.rs] == r[i.rt])
                    codeidx += i.other >> 2;
            }
                break;
            case 3://BLTZ
            {
                if(r[i.rs] < 0)
                    codeidx += i.other >> 2;
            }
                break;
            case 4://BGTZ
            {
                if(r[i.rs] > 0)
                    codeidx  += i.other >> 2;
            }
                break;
            case 5://BREAK
                isBreak = true;
                break;
            case 6://SW
                SetMembyAddr(r[i.rs] + i.other, r[i.rt]); break;
            case 7://LW rt <- memory[r[base] + offset]
            {
                int data = 0;
                if(GetMembyAddr(r[i.rs] + i.other, data))
                    r[i.rt] = data;
            }
                break;
            case 8://SLL
                r[i.rd] = r[i.rt] << i.sa; break;
            case 9://SRL
                r[i.rd] = (signed)((unsigned)r[i.rt] >> i.sa); break; //Logic right shift does not remain sign bit
            case 10://SRA
                r[i.rd] = r[i.rt] >> i.sa; break;
            case 11://NOP
                break;
            case 12://ADD
                r[i.rd] = r[i.rs] + r[i.rt]; break;
            case 13://SUB
                r[i.rd] = r[i.rs] - r[i.rt]; break;
            case 14://MUL
                r[i.rd] = r[i.rs] * r[i.rt]; break;
            case 15://AND
                r[i.rd] = r[i.rs] & r[i.rt]; break;
            case 16://OR
                r[i.rd] = r[i.rs] | r[i.rt]; break;
            case 17://XOR
                r[i.rd] = r[i.rs] ^ r[i.rt]; break;
            case 18://NOR
                r[i.rd] = ~(r[i.rs] | r[i.rt]); break;
            case 19://SLT
                r[i.rd] = (r[i.rs] < r[i.rs])?1:0; break;
            case 20://ADDI
                r[i.rt] = r[i.rs] + i.other; break;
            case 21://ANDI
                r[i.rt] = r[i.rs] & i.other; break;
            case 24://SUBI
                r[i.rt] = r[i.rs] - i.other; break;
            case 25://MULI
                r[i.rt] = r[i.rs] * i.other; break;
            case 26://NORI
                r[i.rt] = ~(r[i.rs]|i.other); break;
            case 27://SLTI
                r[i.rt] = (r[i.rs] < i.other)?1:0; break;
            default:
                break;
        }
        
        //---------------------------------------------------------
        //step output
        //---------------------------------------------------------
        
        stepoutput << "--------------------\n";
        stepoutput << "Cycle:"<< cycle << "\t" << i.address << "\t" << GetCodeType(i) << '\t' <<(std::string)GetCodeDisplay(i) << "\n\nRegisters";
        
        stepoutput << "\nR00:";
        for(int j=0; j<(REGISTERNUM>>1); j++)
            stepoutput << '\t' << r[j];
        
        stepoutput << "\nR16:";
        for(int j=REGISTERNUM>>1; j<REGISTERNUM; j++)
            stepoutput << '\t' << r[j];
        
        stepoutput << "\n\nData\n";
        int memsize = (mem.size() * 4.0 + 16) / 32;
        for(int j=0; j<memsize; j++)
        {
            stepoutput << mem[j<<3].address << ':';
            for(int k=0; k<8; k++)
            {
                int memidx = (j<<3)+k;
                if(memidx < mem.size() << 2)
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