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

class SbInstSim:public InstSim
{
public:
    SbInstSim():pc(0),stalled(false)
    {
        for(int i=0; i<REGISTERNUM; i++)
        {
            isrready[i] = true;
        }
    }
    
    void Run(InstDecoder &instdec);
    void IF_st(InstDecoder &instdec);
    void ISSUE_st();
    void Exec_st();
    void WB_st();
    friend void OStream(std::ostream &outs, int cycle, SbInstSim &sim);
    friend void SStream(std::stringstream &outs, int cycle, SbInstSim &sim);
    
private:
    int pc;
    InstBuffer buffers[8];
    bool isrready[REGISTERNUM];
    bool stalled;
};

#endif
