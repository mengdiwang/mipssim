//
//  instDecoder.cpp
//  mispsim
//
//  Created by Mengdi Wang on 13-4-14.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

//#define TEST

#include "instDecoder.h"
#include <memory.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#ifdef TEST
#include <iostream>
#endif

int GetInstIDB(int instl, int insth, std::string inst);
int GetInstIDB4(std::string inststr);
int GetInstIDB6(std::string inststr);
int GetInstIDB6R(std::string inststr);


Inst &DecJ(std::string inststr, Inst &inst)
{
    inst.other = (GetInstIDB(6, INSTLENGTH-1, inststr)) << 2;
    return inst;
}

Inst &DecJR(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    return inst;
}

Inst &DecBEQ(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << IMMOFFSET;
    return inst;
}

Inst &DecBLTZ(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << IMMOFFSET;
    return inst;
}

Inst &DecBGTZ(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << IMMOFFSET;
    return inst;
}

//Inst &DecBREAK(std::string inststr, Inst &inst){}

Inst &DecSW(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr);
    return inst;
}

Inst &DecLW(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr);
    return inst;
}

Inst &DecSpecialShift(std::string inststr, Inst &inst)
{
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.rd = GetInstIDB(16, 20, inststr);
    inst.sa = GetInstIDB(21, 25, inststr);
    return inst;
}

Inst &DecSpecailA(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.rd = GetInstIDB(16, 20, inststr);
    return inst;
}

Inst &DecSLL(std::string inststr, Inst &inst)
{
    return DecSpecialShift(inststr, inst);
}

Inst &DecSRL(std::string inststr, Inst &inst)
{
    return DecSpecialShift(inststr, inst);
}

Inst &DecSRA(std::string inststr, Inst &inst)
{
    return DecSpecialShift(inststr, inst);
}

//Inst &DecNOP(std::string inststr, Inst &inst){}

Inst &DecADD(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecSUB(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecMUL(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecAND(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecOR(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecXOR(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecNOR(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecSLT(std::string inststr, Inst &inst)
{
    return DecSpecailA(inststr, inst);
}

Inst &DecSpecialAI(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.rt = GetInstIDB(11, 15, inststr);
    inst.other = GetInstIDB(16, 31, inststr);
    return inst;
}

Inst &DecADDI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

Inst &DecSUBI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

Inst &DecMULI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

Inst &DecANDI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

Inst &DecNORI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

Inst &DecSLTI(std::string inststr, Inst &inst)
{
    return DecSpecialAI(inststr, inst);
}

std::string GetCodeDisplay(char sip, Inst &inst)
{
    char tmp[100] = {0};
    std::stringstream ss;
    switch(inst.type)
    {
        case insttype(J)://J
            sprintf(tmp, "%c#%u", sip, inst.other);
            break;
        case insttype(JR)://JR
            sprintf(tmp, "%cR%u", sip, inst.rs);
            break;
        case insttype(BEQ)://BEQ
            sprintf(tmp, "%cR%u, R%u, #%u", sip, inst.rs, inst.rt, inst.other);
            break;
        case insttype(BLTZ)://BLTZ
        case insttype(BGTZ)://BGTZ
            sprintf(tmp, "%cR%u, #%u", sip, inst.rs, inst.other);
            break;
        case insttype(SW)://SW
        case insttype(LW)://LW
            sprintf(tmp, "%cR%u, %u(R%u)", sip, inst.rt, inst.other, inst.rs);
            break;
        case insttype(SLL)://SLL
        case insttype(SRL)://SRL
        case insttype(SRA)://SRA
            sprintf(tmp, "%cR%u, R%u, #%u", sip, inst.rd, inst.rt, inst.sa);
            break;
        case insttype(ADD)://ADD
        case insttype(SUB)://SUB
        case insttype(MUL)://MUL
        case insttype(AND)://AND
        case insttype(OR)://OR
        case insttype(XOR)://XOR
        case insttype(NOR)://NOR
        case insttype(SLT)://SLT
            sprintf(tmp, "%cR%u, R%u, R%u", sip, inst.rd, inst.rs, inst.rt);
            break;
        case insttype(ADDI)://ADDI
        case insttype(ANDI)://ANDI
        case insttype(SUBI)://SUBI
        case insttype(MULI)://MULI
        case insttype(NORI)://NORI
        case insttype(SLTI)://SLTI
            sprintf(tmp, "%cR%u, R%u, #%u", sip, inst.rt, inst.rs, inst.other);
            break;
        default:
            break;
    }

	ss<<tmp;
	
    return ss.str();
}

std::string GetCodeType(Inst &inst)
{
    if(inst.type < sizeof(typestr))
        return typestr[inst.type];
    return "";
}

int GetInstIDB4(std::string inststr)
{
    return GetInstIDB(2, 5, inststr);
}

int GetInstIDB6(std::string inststr)
{
    return GetInstIDB(0, 5, inststr);
}

int GetInstIDB6R(std::string inststr)
{
    return GetInstIDB(26, 31, inststr);
}

int GetData(std::string code)
{
    unsigned int tmp = 0x00000000;
    for(int i=INSTLENGTH-1; i>=0; i--)
    {
        if(code[i] == '1')
            tmp |= 1<<(INSTLENGTH-i-1);
    }
    
    return signed(tmp);
}
//----------------------------------------------------------------
int InstDecoder::Decode(const char *inFile)
{
    std::string fileName = (std::string) inFile;
    int ret = ParseFile(fileName);
    
    if(ret < 0)
        return -1;
    else
        return 1;
}

int InstDecoder::ParseFile(std::string inFile)
{
    std::ifstream instream(inFile.c_str());
    std::string line;
    int address = ADDBASE;
    
    if(instream.is_open())
    {
        bool isbreak = false;
        
        int i=0;
        
        while(std::getline(instream,line))
        {
            i++;
            if(i==8)
                i = 8;
            
            if(isbreak)
            {
                Data data;
                
                data.address = address;
                memcpy(data.code, line.c_str(), INSTLENGTH);
                data.data = GetData(line);
                datas.push_back(data);
            }
            else
            {
                Inst inst;
            
                if(GetInstIDB6(line)==0)
                {
                    ADDMISPSpec(line, inst, isbreak);
                }
                else
                {
                    AddMISP(line, inst);
                }
            
                memcpy(inst.code, line.c_str(), INSTLENGTH);
                inst.address = address;
                insts.push_back(inst);
            
            }
            
            address += 4;
        }
        
        instream.close();
    }
    else
    {
        return -1;
    }
    
    return 0;
}

//get ID bit of instruction
int GetInstIDB(int instl, int insth, std::string inststr)
{
    int idb = 0;
    int pow = 0;
    for(int i=insth; i>=instl; i--)
    {
        idb += ((inststr[i]-'0') << pow);
        pow ++;
    }
    
    return idb;
}

int InstDecoder::AddMISP(std::string inststr, Inst &inst)
{
    int idb = GetInstIDB6(inststr);
    
    switch (idb) {
        case IDJ://J
            inst.type = insttype(J);
            DecJ(inststr, inst);
            break;
        case IDBEQ://BEQ
            inst.type = insttype(BEQ);
            DecBEQ(inststr, inst);
            break;
        case IDBLTZ://BLTZ
            inst.type = insttype(BLTZ);
            DecBLTZ(inststr, inst);
            break;
        case IDBGTZ://BGTZ
            inst.type = insttype(BGTZ);
            DecBGTZ(inststr, inst);
            break;
        case IDSW://SW
            inst.type = insttype(SW);
            DecSW(inststr, inst);
            break;
        case IDLW://LW
            inst.type = insttype(LW);
            DecLW(inststr, inst);
            break;
        case IDMUL://MUL
            inst.type = insttype(MUL);
            DecMUL(inststr, inst);
            break;
        case IDADDI://ADDI
            inst.type = insttype(ADDI);
            DecADDI(inststr, inst);
            break;
        case IDSUBI://SUBI
            inst.type = insttype(SUBI);
            DecSUBI(inststr, inst);
            break;
        case IDMULI://MULI
            inst.type = insttype(MULI);
            DecMULI(inststr, inst);
            break;
        case IDANDI://ANDI
            inst.type = insttype(ANDI);
            DecANDI(inststr, inst);
            break;
        case IDNORI://NORI
            inst.type = insttype(NORI);
            DecNORI(inststr, inst);
            break;
        case IDSLTI://SLTI
            inst.type = insttype(SLTI);
            DecSLTI(inststr, inst);
            break;
            
        default:
            break;
    }
    
    return 0;
}

int InstDecoder::ADDMISPSpec(std::string inststr, Inst &inst, bool &isbreak)
{
    //SPECIL:JR|BREAK|SLL|SRL|SRA|NOP|ADD|SUB|AND|OR|XOR|NOR|SLT LAST 6 bit !!!
    int RIDB = GetInstIDB6R(inststr);
    
    switch (RIDB) {
        case IDJR://JR
            inst.type = insttype(JR);
            DecJR(inststr, inst);
            break;
        case IDBREAK://BREAK
            inst.type = insttype(BREAK);
            isbreak = true;
            break;
        case IDSLLNOP://SLL | NOP(all 0)
            if (GetInstIDB(0, INSTLENGTH-1, inststr) == 0)
            {
                inst.type = insttype(NOP);
            }
            else
            {
                inst.type = insttype(SLL);
                DecSLL(inststr, inst);
            }
            break;
        case IDSRL://SRL
            inst.type = insttype(SRL);
            DecSRL(inststr, inst);
            break;
        case IDSRA://SRA
            inst.type = insttype(SRA);
            DecSRA(inststr, inst);
            break;
        case IDADD://ADD
            inst.type = insttype(ADD);
            DecADD(inststr, inst);
            break;
        case IDSUB://SUB
            inst.type = insttype(SUB);
            DecSUB(inststr, inst);
            break;
        case IDAND://AND
            inst.type = insttype(AND);
            DecAND(inststr, inst);
            break;
        case IDOR://OR
            inst.type = insttype(OR);
            DecOR(inststr, inst);
            break;
        case IDXOR://XOR
            inst.type = insttype(XOR);
            DecXOR(inststr, inst);
            break;
        case IDNOR://NOR
            inst.type = insttype(NOR);
            DecNOR(inststr, inst);
            break;
        case IDSLT://SLT
            inst.type = insttype(SLT);
            DecSLT(inststr, inst);
            break;
        default:
            break;
    }
        
    return 0;
}

int instoffset[6] = {0, 6, 11, 16, 21, 26};
int instlen [6] ={6,5,5,5,5,6};

int InstDecoder::Output(std::string fileName)
{
    std::ofstream outstream(fileName.c_str());

    if(outstream.is_open())
    {
        for(int i=0; i<insts.size(); i++)
        {
            for(int j=0; j<6; j++)
            {
                for(int k=0; k<instlen[j]; k++)
                {
                    outstream << insts[i].code[instoffset[j]+k];
#ifdef TEST
                    std::cout << insts[i].code[instoffset[j]+k];
                    //printf("%c", insts[i].code[instoffset[j]+k]);
#endif
                }
                outstream << (j==5?'\t':' ');
#ifdef TEST
                std::cout << (j==5?'\t':' ');
                //printf(" ");
#endif
            }
            
            outstream << insts[i].address << '\t' << typestr[insts[i].type].c_str()
            << ((insts[i].type==-1)? "" : GetCodeDisplay(' ',insts[i])) << std::endl;
            
#ifdef TEST
            std::cout << insts[i].address << '\t' << typestr[insts[i].type].c_str()
            << ((insts[i].type==-1)? "" : GetCodeDisplay(' ',insts[i])) << std::endl;
            //printf("%d\t%s %s\n", insts[i].address, typestr[insts[i].type].c_str(), insts[i].type==-1?"":GetCodeDisplay(insts[i]));
#endif
        }
        
        for(int i=0; i<datas.size(); i++)
        {
            outstream /*<< std::left << std::setw(38)*/ << datas[i].code << '\t' << datas[i].address << '\t' << datas[i].data << std::endl;
#ifdef TEST
            std::cout << std::left << std::setw(38) << datas[i].code << datas[i].address << '\t' << datas[i].data << std::endl;
            //printf("%-38s%d\t%d\n", datas[i].code, datas[i].address, datas[i].data);
#endif
        }
    }
    else
    {
        return -1;
    }
    
    
    outstream.close();
    return 0;
}

