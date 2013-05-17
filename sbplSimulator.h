//
//  sbplSimulator.h
//  MISPsim
//
//  Created by Mengdi Wang on 13-5-6.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#ifndef MISPsim_sbplSimulator_h
#define MISPsim_sbplSimulator_h

#include "constdef.h"
#include "instSimulator.h"
#include "instDecoder.h"
#include <ostream>
#include <vector>
#include <queue>

class SbInstSim:public InstSim
{
public:
    SbInstSim():pc(0),cycle(1)
    {
        for(int i=0; i<REGISTERNUM; i++)
            result[i] = NIL;
        
        memset(quecycle, 0, sizeof(quecycle));
    }
    
    void Run(InstDecoder &instdec);
    
private:
    bool IF_st(InstDecoder &instdec);
    void ISSUE_st();
    void Exec_st();
    void WB_st();
    
    bool Chkhzd(Inst inst, int pos);//not reference to prevent side effect
    bool ChkWAR(int rd, int pos);
    bool ChkWAW(int rd, int pos);
    bool ChkRAW(int rj, int rk, int pos);
    bool ChkNoSW(int pos);
    bool PLCodeExec(Inst i, int &val);
    
    friend void OStream(std::ostream &outs, int cycle, SbInstSim &sim);
    friend void SStream(std::stringstream &outs, int cycle, SbInstSim &sim);
    
private:
    int pc;
    int cycle;
    std::vector<Inst> buffers[8];
    std::queue<ExecData> postqueues[3];
    int quecycle[8];
    int result[REGISTERNUM];
    std::string waitstr;
    std::string execstr;
};

std::string GetCodeDisplaySb(std::string sip, Inst &inst, std::string sipl);

#endif
