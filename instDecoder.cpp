//
//  instDecoder.cpp
//  mispsim
//
//  Created by Mengdi Wang on 13-4-14.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#define TEST

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
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << 2;
    return inst;
}

Inst &DecBLTZ(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << 2;
    return inst;
}

Inst &DecBGTZ(std::string inststr, Inst &inst)
{
    inst.rs = GetInstIDB(6, 10, inststr);
    inst.other = GetInstIDB(16, INSTLENGTH-1, inststr) << 2;
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

char* GetCodeDisplay(Inst &inst)
{
    char tmp[100] = {0};
    std::stringstream ss;
    switch(inst.type)
    {
        case 0://J
            sprintf(tmp, "#%u", inst.other);
            break;
        case 1://JR
            sprintf(tmp, "R%u", inst.rs);
            break;
        case 2://BEQ
            sprintf(tmp, "R%u, R%u, #%u", inst.rs, inst.rt, inst.other);
            break;
        case 3://BLTZ
        case 4://BGTZ
            sprintf(tmp, "R%u, #%u", inst.rs, inst.other);
            break;
        case 6://SW
        case 7://LW
            sprintf(tmp, "R%u, %u(R%u)", inst.rt, inst.other, inst.rs);
            break;
        case 8://SLL
        case 9://SRL
        case 10://SRA
            sprintf(tmp, "R%u, R%u, #%u", inst.rd, inst.rt, inst.sa);
            break;
        case 12://ADD
        case 13://SUB
        case 14://MUL
        case 15://AND
        case 16://OR
        case 17://XOR
        case 18://NOR
        case 19://SLT
            sprintf(tmp, "R%u, R%u, R%u", inst.rd, inst.rs, inst.rt);
            break;
        case 20://ADDI
        case 21://ANDI
        case 24://SUBI
        case 25://MULI
        case 26://NORI
        case 27://SLTI
            sprintf(tmp, "R%u, R%u, #%u", inst.rt, inst.rs, inst.other);
            break;
        default:
            break;
    }

    return tmp;
}

std::string GetCodeType(Inst &inst)
{
    if(inst.type < sizeof(typestr))
        return typestr[inst.type];
    return "";
}

//Inst &DecADDI(std::string inststr, Inst &inst){}
//Inst &DecANDI(std::string inststr, Inst &inst){}
//Inst &DecORI(std::string inststr, Inst &inst){}
//Inst &DecXORI(std::string inststr, Inst &inst){}


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
        
        while(std::getline(instream,line))
        {
            //if(line[0] == CAT1[0] && line[1] == CAT1[1])
            //{
            //    AddC1type(line, address);
            //}
            
            //std::cout << line << std::endl;
            short isvalid = 0;
            
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
            
                if(line[0] == CAT2[0] && line[1] == CAT2[1])
                {
                    AddC2type(line, inst, isvalid);
                }
                else if(GetInstIDB6(line)==0)
                {
                    ADDMISPSpec(line, inst, isvalid);
                }
                else
                {
                    AddMISP(line, inst, isvalid);
                }
            
                memcpy(inst.code, line.c_str(), INSTLENGTH);
                inst.address = address;
                insts.push_back(inst);
            
                if(isvalid == 2)
                {
                    isbreak = true;
                }
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

int InstDecoder::AddMISP(std::string inststr, Inst &inst, short &valid)
{
    int idb = GetInstIDB6(inststr);
    
    switch (idb) {
        case 2://J
            inst.type = insttype(J);
            DecJ(inststr, inst);
            valid = 1;
            break;
        case 4://BEQ
            inst.type = insttype(BEQ);
            DecBEQ(inststr, inst);
            valid = 1;
            break;
        case 1://BLTZ
            inst.type = insttype(BLTZ);
            DecBLTZ(inststr, inst);
            valid = 1;
            break;
        case 7://BGTZ
            inst.type = insttype(BGTZ);
            DecBGTZ(inststr, inst);
            valid = 1;
            break;
        case 43://SW
            inst.type = insttype(SW);
            DecSW(inststr, inst);
            valid = 1;
            break;
        case 35://LW
            inst.type = insttype(LW);
            DecLW(inststr, inst);
            valid = 1;
            break;
        case 28://MUL
            inst.type = insttype(MUL);
            DecMUL(inststr, inst);
            valid = 1;
            break;
        case 8://ADDI
            inst.type = insttype(ADDI);
            //
            valid = 1;
            break;
        case 12://ANDI
            inst.type = insttype(ANDI);
            //
            valid = 1;
            break;
        case 13://ORI
            inst.type = insttype(ORI);
            //
            valid = 1;
            break;
        case 14://XORI
            inst.type = insttype(XORI);
            //
            valid = 1;
            break;
        default:
            break;
    }
    
    return 0;
}

int InstDecoder::ADDMISPSpec(std::string inststr, Inst &inst, short &valid)
{
    //SPECIL:JR|BREAK|SLL|SRL|SRA|NOP|ADD|SUB|AND|OR|XOR|NOR|SLT LAST 6 bit !!!
    int RIDB = GetInstIDB6R(inststr);
    
    switch (RIDB) {
        case 8://JR
            inst.type = insttype(JR);
            DecJR(inststr, inst);
            valid = 1;
            break;
        case 13://BREAK
            inst.type = insttype(BREAK);
            valid = 2;
            break;
        case 0://SLL | NOP(all 0)
            if (GetInstIDB(0, 31, inststr) == 0)
            {
                inst.type = insttype(NOP);
            }
            else
            {
                inst.type = insttype(SLL);
                DecSLL(inststr, inst);
            }
            valid = 1;
            break;
        case 2://SRL
            inst.type = insttype(SRL);
            DecSRL(inststr, inst);
            valid = 1;
            break;
        case 3://SRA
            inst.type = insttype(SRA);
            DecSRA(inststr, inst);
            valid = 1;
            break;
        case 32://ADD
            inst.type = insttype(ADD);
            DecADD(inststr, inst);
            valid = 1;
            break;
        case 34://SUB
            inst.type = insttype(SUB);
            DecSUB(inststr, inst);
            valid = 1;
            break;
        case 36://AND
            inst.type = insttype(AND);
            DecAND(inststr, inst);
            valid = 1;
            break;
        case 37://OR
            inst.type = insttype(OR);
            DecOR(inststr, inst);
            valid = 1;
            break;
        case 38://XOR
            inst.type = insttype(XOR);
            DecXOR(inststr, inst);
            valid = 1;
            break;
        case 39://NOR
            inst.type = insttype(NOR);
            DecNOR(inststr, inst);
            valid = 1;
            break;
        case 42://SLT
            inst.type = insttype(SLT);
            DecSLT(inststr, inst);
            valid = 1;
            break;
        default:
            break;
    }
        
    return 0;
}

/*
int InstDecoder::AddC1type(std::string inststr, Inst &inst, short &valid)
{
    int idb = GetInstIDB4(inststr);
    
    switch (idb) {
        case 0:
            valid = 1;
            inst.type = insttype(J);
            break;
        case 1:
            valid = 1;
            inst.type = insttype(JR);
            break;
        case 2:
            valid = 1;
            inst.type = insttype(BEQ);
            break;
        case 3:
            valid = 1;
            inst.type = insttype(BLTZ);
            break;
        case 4:
            valid = 1;
            inst.type = insttype(BGTZ);
            break;
        case 5:
            valid = 1;
            inst.type = insttype(BREAK);
            break;
        case 6:
            valid = 1;
            inst.type = insttype(SW);
            break;
        case 7:
            valid = 1;
            inst.type = insttype(LW);
            break;
        case 8:
            valid = 1;
            inst.type = insttype(SLL);
            break;
        case 9:
            valid = 1;
            inst.type = insttype(SRL);
            break;
        case 10:
            valid = 1;
            inst.type = insttype(SRA);
            break;
        case 11:
            valid = 1;
            inst.type = insttype(NOP);
            break;
        default:
            break;
    }
    
    return 0;
}
*/

int InstDecoder::AddC2type(std::string inststr, Inst &inst, short &valid)
{
    int idb = GetInstIDB4(inststr);
    
    switch (idb) {
        case 0:
            valid = 1;
            inst.type = insttype(ADDI);
            DecADDI(inststr, inst);
            break;
        case 1:
            valid = 1;
            inst.type = insttype(SUBI);
            DecSUBI(inststr, inst);
            break;
        case 2:
            valid = 1;
            inst.type = insttype(MULI);
            DecMULI(inststr, inst);
            break;
        case 3:
            valid = 1;
            inst.type = insttype(ANDI);
            DecANDI(inststr, inst);
            break;
        case 6:
            valid = 1;
            inst.type = insttype(NORI);
            DecNORI(inststr, inst);
            break;
        case 7:
            valid = 1;
            inst.type = insttype(SLTI);
            DecSLTI(inststr, inst);
            break;
/*
        case 4:
            valid = 1;
            inst.type = insttype(OR);
            DecOR(inststr, inst);
            break;
        case 5:
            valid = 1;
            inst.type = insttype(XOR);
            DecXOR(inststr, inst);
            break;
        case 8:
            valid = 1;
            inst.type = insttype(ADDI);
            //
            break;
        case 9:
            valid = 1;
            inst.type = insttype(ANDI);
            //
            break;
        case 10:
            valid = 1;
            inst.type = insttype(ORI);
            //
            break;
        case 11:
            valid = 1;
            inst.type = insttype(XORI);
            //
            break;
 */
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
                outstream << ' ';
#ifdef TEST
                std::cout << ' ';
                //printf(" ");
#endif
            }
            
            outstream << insts[i].address << '\t' << typestr[insts[i].type].c_str() <<' '
            << (insts[i].type==-1?"":(std::string)GetCodeDisplay(insts[i])) << std::endl;
            
#ifdef TEST
            std::cout << insts[i].address << '\t' << typestr[insts[i].type].c_str() <<' '
                      << (insts[i].type==-1?"":(std::string)GetCodeDisplay(insts[i])) << std::endl;
            //printf("%d\t%s %s\n", insts[i].address, typestr[insts[i].type].c_str(), insts[i].type==-1?"":GetCodeDisplay(insts[i]));
#endif
        }
        
        for(int i=0; i<datas.size(); i++)
        {
            outstream << std::left << std::setw(38) << datas[i].code << datas[i].address << '\t' << datas[i].data << std::endl;
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

