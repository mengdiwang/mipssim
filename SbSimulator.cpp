//
//  SbSimulator.cpp
//  MISPsim
//
//  Created by Mengdi Wang on 13-5-22.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#include "SbSimulator.h"

#include "sbplSimulator.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>

#ifdef DEBUG
#include <assert.h>
#include <iostream>
#endif

bool PSBSim::IF_st(InstDecoder &instdec)
{
    std::vector<ScoreBoard> &preisqueue = sboards[PREISSUE];
    bool isbreak = false;
    bool isbranch = false;
    bool jump;
    bool stalled = false;;
    
    for(int i=0; i<2; i++)
    {
        Inst inst;
        //branch is fetched, the next instrcton is discarded immediately
        if(!isbranch && pc<instdec.GetInsts().size() && preisqueue.size()<4)
        {
            inst = instdec.GetInsts()[pc];
            
            switch (inst.type) {
                case JR:
                case BEQ:
                case BLTZ:
                case BGTZ:
                case J:
                    isbranch = true;
                    if(ChkRAW(inst.rs, inst.rt, (int)preisqueue.size()))
                    {
                        waitstr = GetCodeDisplaySb("", inst, "");
                        execstr = "";
                        stalled = true;
                    }
                    else
                    {
                        pc ++;
                        CodeExec(inst, pc, jump);
                        execstr = GetCodeDisplaySb("", inst, "");
                        waitstr = "";
                    }
                    break;
                case NOP:
                    pc++;
                    waitstr = "";
                    execstr = "";
                    break;
                case BREAK:
                    isbreak = true;
                    pc++;
                    waitstr = "";
                    execstr = "BREAK";
                    break;
                default:
                    //other instuctions treat as obmit ifqueue and push to the pre issue queue directly
                    inst.cycle = cycle;
                    
                    ScoreBoard sb;
                    sb.fi = inst.rd;
                    sb.fj = inst.rs;
                    sb.fk = inst.rt;
                    sb.cycle = cycle;
                    
                    preisqueue.push_back(sb);
                    pc++;
                    waitstr = "";
                    execstr = "";
                    break;
            }
            if(i==0 && isbranch && preisqueue.size()<3)
            {
                if(instdec.GetInsts()[pc].type==BREAK)
                    isbreak = true;
            }
        }
    }
    return isbreak;
}

void PSBSim::Run(InstDecoder &instdec)
{
    bool isbreak = false;
    
    std::stringstream stepoutput;
    
    SetMem(instdec.GetDatas());
    
    while(true)
    {
        if(isbreak)
            break;
        
        isbreak = IF_st(instdec);
        
        ISSUE_st();
        
        Exec_st();
        
        WB_st();
        
        //---------------------------------------------------------
        //step output
        //---------------------------------------------------------
        SStream(stepoutput, cycle, *this);
        //---------------------------------------------------------
#ifdef DEBUG
        OStream(std::cout, cycle, *this);
        
        if(cycle > 1000)
            break;
#endif
        //---------------------------------------------------------
        cycle ++;
    }
    output.append(stepoutput.str());
}

bool PSBSim::SBChkhzd(ScoreBoard sb, int pos)
{
    bool canissued = false;
    
    bool WARhazard = false;
    bool WAWhazard = false;
    bool RAWhazard = false;
    bool HasunisSW = false;
    bool isbusy    = true;
    
    std::vector<ScoreBoard> &preisqueue = sboards[PREISSUE];
    
    switch (sb.type) {
            //ALUB
        case SLL:
        case SRL:
        case SRA:
        case MUL:
        case MULI:
            isbusy    = sb.busy;
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rt, pos);
            
            if(!isbusy && !WAWhazard && !WAWhazard && !RAWhazard && buffers[PREALUB].size()<2 && sb.cycle<cycle)//structure hazard and in cycle
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                sb.cycle = cycle;
                sboards[PREALUB].push_back(sb);
                canissued = true;
            }
            
            break;
            
            //MEM
        case LW://LW
            isbusy    = sb.busy;
            HasunisSW = ChkNoSW(pos);
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rd, pos);//for MEM instructions, all the source registers are ready at the end of the previous cycle.
            if(!isbusy && !WAWhazard && !WAWhazard && !RAWhazard && buffers[PREMEM].size()<2 && sb.cycle<cycle)//structure hazard and in cycle
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                sb.cycle = cycle;
                sboards[PREMEM].push_back(sb);
                canissued = true;
            }
            break;
        case SW://sw memory[base+offset] <- rt, rt read
            isbusy    = sb.busy;
            HasunisSW = ChkNoSW(pos);
            RAWhazard = ChkRAW(inst.rs, inst.rd, pos);//for MEM instructions, all the source registers are ready at the end of the previous cycle.
            if(!isbusy && !WAWhazard && !WAWhazard && !RAWhazard && buffers[PREMEM].size()<2 && sb.cycle<cycle)//structure hazard and in cycle
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                
                sb.cycle = cycle;
                sboards[PREMEM].push_back(sb);
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
#ifdef DEBUG
            assert(0);
#endif
            break;
            
        default://ALU
            isbusy    = sb.busy;
            WARhazard = ChkWAR(inst.rd, pos);
            WAWhazard = ChkWAW(inst.rd, pos);
            RAWhazard = ChkRAW(inst.rs, inst.rt, pos);
            
            if(!isbusy && !WAWhazard && !WAWhazard && !RAWhazard && buffers[PREALU].size()<2 && sb.cycle<cycle)
            {
                preisqueue[pos].type = NIL; //delete pos at pre-issue buffer
                sb.cycle = cycle;
                sboards[PREALU].push_back(sb);
                canissued = true;
            }
            
            break;
    }
    
    if(sb.type!=SW)
        result[sb.rd] = sb.type;
    if(canissued)
    {
        sb.busy = busy;
        sb.qj = result[sb.rs];
        sb.qk = result[sb.rt];
        if(sb.qj!=NIL)
            sb.rj = false;
        else
            sb.rj = true;
        if(sb.qk!=NIL)
            sb.rk = false;
        else
            sb.rk = true;
    }
    
    return canissued;
}

void PSBSim::ISSUE_st()
{
    std::vector<ScoreBoard> &preisqueue = sboards[PREISSUE];
    
    int issuecount = 0;
    for(int i=0 ;i<preisqueue.size(); i++)
    {
        if(SBChkhzd(preisqueue[i], i))
            issuecount++;
        
        if(issuecount>=2)
            break;
    }
}

void PSBSim::Exec_st()
{

}

void PSBSim::WB_st()
{

}

bool PSBSim::SBChkRAW(int rd, int pos)
{
    return false;
}

bool PSBSim::SBChkWAR(int rd, int pos)
{
    return false;
}

bool PSBSim::SBChkWAW(int rd, int pos)
{
//    std::vector<ScoreBoard> &preisqueue = sboards[PREISSUE];
//    for(int i=0; (i<pos) && (i<preisqueue.size()); i++)
//    {
//        if(preisqueue[i].type!=NIL && preisqueue[i].rd == rd)
//        {
//            return true;
//        }
//    }
    
    if(result[rd]!=NIL)
        return true;
    return false;
}