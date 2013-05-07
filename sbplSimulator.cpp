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
#include <assert.h>
#include <iostream>
#endif

void OStream(std::ostream &outs, int cycle, SbInstSim &sim);
void SStream(std::stringstream &outs, int cycle, SbInstSim &sim);

std::string GetCodeDisplaySb(char sip, Inst &inst, char sipl)
{
    char tmp[100] = {0};
    std::stringstream ss;
    switch(inst.type)
    {
        case insttype(J)://J
            sprintf(tmp, "#%u%c", inst.other, sipl);
            break;
        case insttype(JR)://JR
            sprintf(tmp, "R%u%c", inst.rs, sipl);
            break;
        case insttype(BEQ)://BEQ
            sprintf(tmp, "R%u, R%u, #%d%c", inst.rs, inst.rt, inst.other, sipl);
            break;
        case insttype(BLTZ)://BLTZ
        case insttype(BGTZ)://BGTZ
            sprintf(tmp, "R%u, #%d%c", inst.rs, inst.other, sipl);
            break;
        case insttype(SW)://SW
        case insttype(LW)://LW
            sprintf(tmp, "R%u, %d(R%u)%c", inst.rd, inst.other, inst.rs, sipl);
            break;
        case insttype(SLL)://SLL
        case insttype(SRL)://SRL
        case insttype(SRA)://SRA
            sprintf(tmp, "R%u, R%u, #%u%c", inst.rd, inst.rt, inst.sa, sipl);
            break;
        case insttype(ADD)://ADD
        case insttype(SUB)://SUB
        case insttype(MUL)://MUL
        case insttype(AND)://AND
        case insttype(OR)://OR
        case insttype(XOR)://XOR
        case insttype(NOR)://NOR
        case insttype(SLT)://SLT
            sprintf(tmp, "R%u, R%u, R%u%c", inst.rd, inst.rs, inst.rt, sipl);
            break;
        case insttype(ADDI)://ADDI
        case insttype(ANDI)://ANDI
        case insttype(SUBI)://SUBI
        case insttype(MULI)://MULI
        case insttype(NORI)://NORI
        case insttype(SLTI)://SLTI
            sprintf(tmp, "R%u, R%u, #%d%c", inst.rd, inst.rs, inst.other, sipl);
            break;
        default:
            break;
    }
    
    ss<<sip;
    ss<<typestr[inst.type] << '\t';
	ss<<tmp;
	
    return ss.str();
}

void SbInstSim::IF_st(InstDecoder &instdec)
{
    std::vector<Inst> &ifqueue = buffers[IFUNIT];
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    
    if(ifstate == 1)
    {
        Inst inst = ifqueue[0];
        if(!ChkRAW(inst.rs, inst.rt, preisqueue.size()))
        {
            ifstate = 2;
            //JUMP??
            return;
        }
    }
    else if(ifstate == 2)
    {
        //jump
        bool jump;
        CodeExec(ifqueue[0], pc, jump);
        
        ifqueue.erase(ifqueue.begin());//?clear
        //ifqueue.pop_front();
        
        ifstate = 0;
        return;
    }
    else if(ifstate == 4)
    {
#ifdef TEST
        assert(0);
#endif
        return;//
    }
        
    for(int i=0; i<2; i++)
    {
        Inst inst;
        if(!ifstate && preisqueue.size()<4 && ifqueue.size()<2)
        {
            inst = instdec.GetInsts()[pc++];
            
            switch (inst.type) {
                case JR:
                case BEQ:
                case BLTZ:
                case BGTZ:
                    inst.cycle = cycle;
                    ifqueue.push_back(inst);
                    //quecycle[IFUNIT] = cycle;
                    if(ChkRAW(inst.rs, inst.rt, preisqueue.size()))
                    {
                        ifstate = 1;//stall
                    }
                    else
                    {
                        ifstate = 2;//execute
                    }
                    break;
                case J:
                    inst.cycle = cycle;
                    ifqueue.push_back(inst);
                    //quecycle[IFUNIT] = cycle;
                    ifstate = 2;
                    break;
                case NOP:
                    break;
                case BREAK:
                    ifstate = 4;
                    break;
                default:
                    //other instuctions treat as obmit ifqueue and push to the pre issue queue directly
                    inst.cycle = cycle;
                    preisqueue.push_back(inst);
                    ///quecycle[PREISSUE] = cycle;
                    break;
            }
            if(i==0 && ifstate && preisqueue.size()<4 && ifqueue.size()<2)
            {
                if(instdec.GetInsts()[pc+1].type==BREAK)
                    ifstate = 4;
            }
        }
    }
}

//write after read.
//In the earlier pre-issue queue, return true when there happens (INST Rx, Rrd, Rx) or (INST Rx, Rx, Rrd)
bool SbInstSim::ChkWAR(int rd, int pos)
{
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    
    for(int i=0; (i<pos) && (i<preisqueue.size()); i++)
    {
        if(preisqueue[i].type!=NIL && (preisqueue[i].rs == rd || preisqueue[i].rt == rd))
        {
            return true;
        }
    }
    
    return false;
}

bool SbInstSim::ChkWAW(int rd, int pos)
{
    //check not-issued instructions
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    for(int i=0; (i<pos) && (i<preisqueue.size()); i++)
    {
        if(preisqueue[i].type!=NIL && preisqueue[i].rd == rd)
        {
            return true;
        }
    }
    
    //check issued but not finished instructions
/*
    std::vector<Inst> &execqueue = buffers[EXEC];
    for(int i=0; i<execqueue.size(); i++)
    {
        if(execqueue[i].type!=NIL && execqueue[i].rd == rd)
        {
            return true;
        }
    }
*/
    if(result[rd] != NIL)
        return true;
    
    return false;
}

bool SbInstSim::ChkRAW(int rj, int rk, int pos)
{
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    std::vector<Inst> &execqueue = buffers[EXEC];
    
    for(int i=0; (i<pos) && (i<preisqueue.size()); i++)
    {
        if((preisqueue[i].type!=NIL && preisqueue[i].rd==rj) || (preisqueue[i].type!=NIL && preisqueue[i].rd==rk))
        {
            return true;
        }
    }
 
/*
    for(int i=0; (i<pos) && (i<execqueue.size()); i++)
    {
        if((execqueue[i].type!=NIL && execqueue[i].rd==rj)||(execqueue[i].type!=NIL && execqueue[i].rd==rk))
        {
            return true;
        }
    }
*/
    if((rj>0 && result[rj]!=NIL) || (rk>0 && result[rk]!=NIL))
    
    return false;
}


bool SbInstSim::ChkNoSW(int pos)
{
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    
    for(int i=0; (i<pos) && (i<preisqueue.size()); i++)
    {
        if(preisqueue[i].type==SW)
            return true;
    }
    return false;
}

bool SbInstSim::Chkhzd(Inst inst, int pos)
{
    bool WARhazard = false;
    bool WAWhazard = false;
    bool RAWhazard = false;
    bool canissued = false;
    bool HasunisSW = false;
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    
    switch (inst.type) {
        case SLL:
        case SRL:
        case SRA:
        case MUL:
        case MULI://ALUB
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rt, pos);
            
            if(!WAWhazard && !WAWhazard && !RAWhazard && buffers[PREALU].size()<2 && inst.cycle<cycle)
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                inst.cycle = cycle;
                buffers[PREALUB].push_back(inst);
                //quecycle[PREALUB] = cycle;
                canissued = true;
            }
            
            break;
            
        case LW:
        case SW://MEM ????
            HasunisSW = ChkNoSW(pos);
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rt, pos);
            if(!WAWhazard && !WAWhazard && !RAWhazard && buffers[PREALU].size()<2 && inst.cycle<cycle)
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                inst.cycle = cycle;
                buffers[PREMEM].push_back(inst);
                //quecycle[PREMEM] = cycle;
                canissued = true;
            }
            break;
            
        case J:
        case JR:
        case BEQ:
        case BLTZ:
        case BGTZ:
        case NOP:
        case BREAK://WRONG
#ifdef TEST
            assert(0);
#endif
            break;
            
        default://ALU
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rt, pos);
            
            if(!WAWhazard && !WAWhazard && !RAWhazard && buffers[PREALU].size()<2 && inst.cycle<cycle)
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                inst.cycle = cycle;
                buffers[PREALU].push_back(inst);
                //quecycle[PREALU] = cycle;
                canissued = true;
            }
            
            break;
    }
    
    if(canissued)
    {
        //buffers[EXEC].push_back(inst);
        result[inst.rd] = inst.type;
    }
    
    return canissued;
}

void SbInstSim::ISSUE_st()
{
    std::vector<Inst> &preisqueue = buffers[PREISSUE];
    
    int issuecount = 0;
    for(int i=0; i<preisqueue.size(); i++)
    {
        if(Chkhzd(preisqueue[i], i))
            issuecount ++;
    
        if(issuecount==2)
            break;
    }
    
    std::vector<Inst> tmp;
    for(int i=0; i<preisqueue.size(); i++)
    {
        if(preisqueue[i].type!=NIL)
            tmp.push_back(preisqueue[i]);
    }
    
    preisqueue.clear();
    for(int i=0; i<tmp.size(); i++)
    {
        preisqueue.push_back(tmp[i]);
    }
    //std::remove_if(preisqueue.begin(), preisqueue.end(), );
}

void SbInstSim::Exec_st()
{
    int codeidx = 0;
    bool jump = 0;
    if(buffers[PREALU].size()>0)
    {
        Inst inst = buffers[PREALU].front();
        if(inst.cycle < cycle)
        {
            buffers[PREALU].erase(buffers[PREALU].begin());
        
            CodeExec(inst, codeidx, jump);
            
            inst.cycle = cycle;
            buffers[POSTALU].push_back(inst);
            //quecycle[POSTALU] = cycle;
        }
        
    }
    if(buffers[PREALUB].size()>0)
    {
        Inst inst = buffers[PREALUB].front();
        if(inst.cycle < cycle)
        {
            buffers[PREALUB].erase(buffers[PREALUB].begin());
        
            CodeExec(inst, codeidx, jump);
        
            inst.cycle = cycle;
            buffers[POSTALUB].push_back(inst);
            //quecycle[POSTALUB] = cycle;
        }
    }
    if(buffers[PREMEM].size()>0)
    {
        Inst inst = buffers[PREMEM].front();
        if(inst.cycle < cycle)
        {
            buffers[PREMEM].erase(buffers[PREMEM].begin());
        
            CodeExec(inst, codeidx, jump);
        
            if(inst.type == LW)
            {
                inst.cycle = cycle;
                buffers[POSTMEM].push_back(inst);
                //quecycle[POSTMEM] = cycle;
            }
            else
            {
                result[inst.rd] = NIL;
            }
        }
    }
    
}

void SbInstSim::WB_st()
{
    if(buffers[POSTALU].size()>0)
    {
        Inst inst = buffers[POSTALU].front();
        if(inst.cycle<cycle)
        {
            buffers[POSTALU].clear();
            result[inst.rd] = NIL;
        }
    }
    if(buffers[POSTALUB].size()>0)
    {
        Inst inst = buffers[POSTALUB].front();
        if(inst.cycle<cycle)
        {
            buffers[POSTALUB].clear();
            result[inst.rd] = NIL;
        }
    }
    if(buffers[POSTMEM].size()>0)
    {
        Inst inst = buffers[POSTMEM].front();
        if(inst.cycle < cycle)
        {
            buffers[POSTMEM].clear();
            result[inst.rd] = NIL;
        }
    }
}

void SbInstSim::Run(InstDecoder &instdec)
{
    int ret = 0;
    int codeidx = 0;
    std::stringstream stepoutput;
    
    SetMem(instdec.GetDatas());
    
    while(true)
    {
        if(ifstate==4)
            break;
        
        IF_st(instdec);
        
        ISSUE_st();
        
        Exec_st();
        
        WB_st();

        //---------------------------------------------------------
        //step output
        //---------------------------------------------------------
        //SStream(stepoutput, cycle, *this);
        //---------------------------------------------------------
#ifdef TEST
        OStream(std::cout, cycle, *this);
        //std::cout << "--------------------------" << std::endl;
        if(cycle > 100)
            break;
#endif
        //---------------------------------------------------------        
        cycle ++;
    }
}

void SStream(std::stringstream &outs, int cycle, SbInstSim &sim)
{
    outs << "--------------------\n";
    outs << "Cycle:"<< cycle << "\n\n";
    
    outs << "IF Unit:\n";
    outs << "\tWaiting Instruction:";
    if(sim.ifstate==1)
        outs<<  GetCodeDisplaySb('[', sim.buffers[IFUNIT][0], ']');
    outs << std::endl;
        
    outs << "\tExecuted Instruction:";
    if(sim.ifstate==2)
        outs<< GetCodeDisplaySb('[', sim.buffers[IFUNIT][0], ']');
    outs << std::endl;
    
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
    outs << "\tWaiting Instruction:";
    if(sim.ifstate==1)
        outs<<  GetCodeDisplaySb('[', sim.buffers[IFUNIT][0], ']');
    outs << std::endl;
    
    outs << "\tExecuted Instruction:";
    if(sim.ifstate==2)
        outs<< GetCodeDisplaySb('[', sim.buffers[IFUNIT][0], ']');
    outs << std::endl;
    
    outs << "Pre-Issue Buffer:\n";
    outs << "\tEntry 0:";
    if(sim.buffers[PREISSUE].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREISSUE][0], ']');
    outs << std::endl;
    
    outs << "\tEntry 1:";
    if(sim.buffers[PREISSUE].size()>1)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREISSUE][1], ']');
    outs << std::endl;
    
    outs << "\tEntry 2:";
    if(sim.buffers[PREISSUE].size()>2)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREISSUE][2], ']');
    outs << std::endl;
    
    outs << "\tEntry 3:";
    if(sim.buffers[PREISSUE].size()>3)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREISSUE][3], ']');
    outs << std::endl;
    
    
    outs << "Pre-ALU Queue:" << std::endl;
    outs << "\tEntry 0:";
    if(sim.buffers[PREALU].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREALU][0], ']');
    outs << std::endl;
    
    outs << "\tEntry 1:";
    if(sim.buffers[PREALU].size()>1)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREALU][1], ']');
    outs << std::endl;
    
    outs << "Post-ALU Buffer:";
    if(sim.buffers[POSTALU].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[POSTALU][0], ']');
    outs << std::endl;
    
    outs << "Pre-ALUB Queue:" << std::endl;
    outs << "\tEntry 0:";
    if(sim.buffers[PREALUB].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREALUB][0], ']');
    outs << std::endl;
    
    outs << "\tEntry 1:";
    if(sim.buffers[PREALUB].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREALUB][0], ']');
    outs << std::endl;
    
    outs << "Post-ALUB Buffer:";
    if(sim.buffers[POSTALUB].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[POSTALUB][0], ']');
    outs << std::endl;
    
    outs << "Pre-MEM Queue:" << std::endl;
    outs << "\tEntry 0:";
    if(sim.buffers[PREMEM].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREMEM][0], ']');
    outs << std::endl;
    
    outs << "\tEntry 1:";
    if(sim.buffers[PREMEM].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[PREMEM][0], ']');
    outs << std::endl;
    
    outs << "Post-MEM Buffer:";
    if(sim.buffers[POSTMEM].size()>0)
        outs<< GetCodeDisplaySb('[', sim.buffers[POSTMEM][0], ']');
    outs << std::endl;
    
    outs << "\nRegisters\n";
    
    int p = 0;
    for(int j=0; j<4; j++)
    {
        outs << "R"<< std::setfill('0') << std::setw(2) << p <<":\t";
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
