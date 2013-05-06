//
//  sbplSimulator.cpp
//  MISPsim
//
//  Created by Mengdi Wang on 13-5-6.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#define TEST

#include "sbplSimulator.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>

#ifdef TEST
#include <iostream>
#endif

void OStream(std::ostream &outs, int cycle, SbInstSim &sim);
void SStream(std::stringstream &outs, int cycle, SbInstSim &sim);

void SbInstSim::IF_st(InstDecoder &instdec)
{
    if(stalled)
        return;
    
    std::deque<Inst> &ifqueue = buffers[IFUNIT].buffer;
    for(int i=0; i<2; i++)
    {
        Inst inst;
        if(!stalled && ifqueue.size()<(4-i) && ifqueue.size()<=2)
        {
            inst = instdec.GetInsts()[pc++];
            buffers[IFUNIT].buffer.push_back(inst);
        }
        
        if(inst.type==J)
        {
//            buffers[IFUNIT].state[ifqueue.size()-1]=2;//Executed Instruction
        }
        else if(inst.type==JR || inst.type==BEQ || inst.type==BLTZ || inst.type==BGTZ)
        {
            if(!isrready[inst.rs])
            {
//                buffers[IFUNIT].state[ifqueue.size()-1]=1;//Waiting Instruction
                stalled = true;
            }
            else
            {
//                buffers[IFUNIT].state[ifqueue.size()-1]=2;//Executed Instruction
            }
        }
    }
}

void SbInstSim::Exec_st()
{
    
}

void SbInstSim::WB_st()
{

}

void SbInstSim::ISSUE_st()
{}

void SbInstSim::Run(InstDecoder &instdec)
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
        
        //Inst i= instdec.GetInsts()[codeidx];
        
        IF_st(instdec);
        
        ISSUE_st();
        
        Exec_st();
        
        WB_st();
        
        
        
        //---------------------------------------------------------
        //step output
        //---------------------------------------------------------
        SStream(stepoutput, cycle, *this);
        //---------------------------------------------------------
#ifdef TEST
        OStream(std::cout, cycle, *this);

        if(cycle > 100)
            break;
#endif
        //---------------------------------------------------------
        
        if(!jump)
            codeidx++;
        
        cycle ++;

    }
}

void SStream(std::stringstream &outs, int cycle, SbInstSim &sim)
{
    outs << "--------------------\n";
    outs << "Cycle:"<< cycle << "\n\n";
    
    outs << "IF Unit:\n";
    outs << "\tWaiting Instruction:" << std::endl;
    outs << "\tExecuted Instruction:" << std::endl;
    outs << "Pre-Issue Buffer:\n";
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "\tEntry 2:" << std::endl;
    outs << "\tEntry 3:" << std::endl;
    outs << "Pre-ALU Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-ALU Buffer:" << std::endl;
    outs << "Pre-ALUB Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-ALUB Buffer:" << std::endl;
    outs << "Pre-MEM Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-MEM Buffer:" << std::endl;
    
    outs << "\nRegisters\n";
    
    int p = 0;
    for(int j=0; j<4; j++)
    {
        outs << "R"<< std::setfill('0') << std::setw(2) << (p) <<":\t";
        for(int h=0; h<REGISTERNUM>>2; h++)
        {
            outs << sim.r[p++] << ((h==((REGISTERNUM>>2)-1))?'\n':'\t');
        }
    }
    
    outs << "\nData\n";
    
    int memsize = std::ceil((sim.mem.size() + 0.0) / 8);
    for(int j=0; j<memsize; j++)
    {
        outs << sim.mem[j<<3].address << ':';
        for(int k=0; k<8; k++)
        {
            int memidx = (j<<3)+k;
            if(memidx < sim.mem.size())
                outs <<'\t' << sim.mem[memidx].data;
        }
        outs << '\n';
    }
}

void OStream(std::ostream &outs, int cycle, SbInstSim &sim)
{
    outs << "--------------------\n";
    outs << "Cycle:"<< cycle << "\n\n";
    
    outs << "IF Unit:\n";
    outs << "\tWaiting Instruction:" << std::endl;
    outs << "\tExecuted Instruction:" << std::endl;
    outs << "Pre-Issue Buffer:\n";
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "\tEntry 2:" << std::endl;
    outs << "\tEntry 3:" << std::endl;
    outs << "Pre-ALU Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-ALU Buffer:" << std::endl;
    outs << "Pre-ALUB Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-ALUB Buffer:" << std::endl;
    outs << "Pre-MEM Queue:" << std::endl;
    outs << "\tEntry 0:" << std::endl;
    outs << "\tEntry 1:" << std::endl;
    outs << "Post-MEM Buffer:" << std::endl;
    
    outs << "\nRegisters\n";
    
    int p = 0;
    for(int j=0; j<4; j++)
    {
        outs << "R"<< std::setfill('0') << std::setw(2) << (p) <<":\t";
        for(int h=0; h<REGISTERNUM>>2; h++)
        {
            outs << sim.r[p++] << ((h==((REGISTERNUM>>2)-1))?'\n':'\t');
        }
    }
    
    outs << "\nData\n";
    
    int memsize = std::ceil((sim.mem.size() + 0.0) / 8);
    for(int j=0; j<memsize; j++)
    {
        outs << sim.mem[j<<3].address << ':';
        for(int k=0; k<8; k++)
        {
            int memidx = (j<<3)+k;
            if(memidx < sim.mem.size())
                outs <<'\t' << sim.mem[memidx].data;
        }
        outs << '\n';
    }
}
