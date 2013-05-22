//
//  SbSimulator.h
//  MISPsim
//
//  Created by Mengdi Wang on 13-5-22.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#ifndef __MISPsim__SbSimulator__
#define __MISPsim__SbSimulator__

#include <vector>
#include "sbplSimulator.h"

struct ScoreBoard
{
    ScoreBoard():type(NIL),fi(0),fj(0),fk(0),qj(0),qk(0),busy(false),rj(false),rk(false),cycle(0),rt(1024),rd(1024),rs(1024)
    {
        
    }
    
    insttype type;
    int fi;
    int fj;
    int fk;
    int qj;
    int qk;
    int cycle;
    int rd;
    int rs;
    int rt;
    bool busy;
    bool rj;
    bool rk;
};


class PSBSim:public SbInstSim
{
public:
    void Run(InstDecoder &instdec);
    
protected:
    bool IF_st(InstDecoder &instdec);
    void ISSUE_st();
    void Exec_st();
    void WB_st();
    bool SBChkhzd(ScoreBoard sb, int pos);
    bool SBChkWAR(int rd, int pos);
    bool SBChkWAW(int rd, int pos);
    bool SBChkRAW(int rd, int pos);
    
protected:
    std::vector<ScoreBoard> sboards[8];
};

#endif /* defined(__MISPsim__SbSimulator__) */
