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
#include <sstream>

class SbInstSim:public InstSim
{
public:
    SbInstSim():pc(0),cycle(1)
    {
        for(int i=0; i<REGISTERNUM; i++)
            result[i] = NIL;
    }
    
    void Run(InstDecoder &instdec);
    
protected:
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
    
protected:
    int pc;
    int cycle;
    std::vector<Inst> buffers[8];
    std::queue<ExecData> postqueues[3];
    //int quecycle[8];
    int result[REGISTERNUM];
    std::string waitstr;
    std::string execstr;
    
    
};
//
//struct ScoreBoarding
//{
//    ScoreBoarding():op(NIL),fi(0),fj(0),fk(0),qj(0),qk(0),busy(false),rj(false),rk(false)
//    {
//    
//    }
//   
//    insttype op;
//    int fi;
//    int fj;
//    int fk;
//    int qj;
//    int qk;
//    bool busy;
//    bool rj;
//    bool rk;
//};

std::string GetCodeDisplaySb(std::string sip, Inst &inst, std::string sipl);
void OStream(std::ostream &outs, int cycle, SbInstSim &sim);
void SStream(std::stringstream &outs, int cycle, SbInstSim &sim);

#endif
