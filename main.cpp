//
//  main.cpp
//  mispsim
//
//  Created by Mengdi Wang on 13-4-14.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "instDecoder.h"
#include "instSimulator.h"
//test
//#include <unistd.h>


int main(int argc, const char * argv[])
{
	if(argc < 1)
	{
		printf("Usage: MIPSsim <input>\n");
		return 1;
	}
	
    //char * dir = getcwd(NULL, 0); // Platform-dependent, see reference link below
    //printf("Current dir: %s\n", dir);
    
    InstDecoder decoder;

    decoder.Decode(argv[1]);
	decoder.Output("disassembly.txt");
    
    InstSim simulator;
    simulator.Run(decoder);
    
    simulator.Output("simulation.txt");
    
	return 0;
}

