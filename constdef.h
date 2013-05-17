//
//  Const.h
//  MISPsim
//
//  Created by Mengdi Wang on 13-4-18.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//
#ifndef MISPsim_Const_h
#define MISPsim_Const_h
#include <string>
#include <deque>
#include <memory.h>
//--------------------------------------------------------------------------
const int REGISTERNUM   = 32;
const int MEMUNIT       = 4;
const int ADDBASE       = 64;
const int INSTLENGTH    = 32;
const int IMMOFFSET     = 2;
//--------------------------------------------------------------------------
const std::string typestr[28] = {"J", "JR", "BEQ", "BLTZ", "BGTZ", "BREAK", "SW", "LW", "SLL", "SRL", "SRA", "NOP",
    "ADD", "SUB", "MUL", "AND", "OR", "XOR", "NOR", "SLT", "ADD", "AND", "ORI", "XORI", "SUB", "MUL", "NOR", "SLT"};
//--------------------------------------------------------------------------
enum insttype
{
    NIL		= 1024,
    J		= 0,
    JR		= 1,
    BEQ		= 2,
    BLTZ	= 3,
    BGTZ	= 4,
    BREAK	= 5,
    SW		= 6,
    LW		= 7,
    SLL		= 8,
    SRL		= 9,
    SRA		= 10,
    NOP		= 11,
    ADD		= 12,
    SUB		= 13,
    MUL		= 14,
    AND		= 15,
    OR		= 16,
    XOR		= 17,
    NOR		= 18,
    SLT		= 19,
    ADDI	= 20,
    ANDI	= 21,
    ORI		= 22,
    XORI	= 23,
    SUBI	= 24,
    MULI	= 25,
    NORI	= 26,
    SLTI	= 27
};
//--------------------------------------------------------------------------
const int IDJ       = 0x02;//000010
const int IDBEQ     = 0x04;//000100
const int IDBLTZ    = 0x01;//000001
const int IDBGTZ    = 0x07;//000111
const int IDSW      = 0x2B;//101011
const int IDLW      = 0x23;//100011
const int IDMUL     = 0x1C;//011000
const int IDADDI    = 0x30;//110000
const int IDSUBI    = 0x31;//110001
const int IDMULI    = 0x21;//100001
const int IDANDI    = 0x32;//110010
const int IDNORI    = 0x33;//110011
const int IDSLTI    = 0x35;//110101
const int IDJR      = 0x08;//001000
const int IDBREAK   = 0x0D;//001101
const int IDSLLNOP  = 0x00;//000000
const int IDSRL     = 0x02;//000010
const int IDSRA     = 0x03;//000011
const int IDADD     = 0x20;//100000
const int IDSUB     = 0x22;//100010
const int IDAND     = 0x24;//100100
const int IDOR      = 0x25;//100101
const int IDXOR     = 0x26;//100110
const int IDNOR     = 0x27;//100111
const int IDSLT     = 0x2A;//101010
//--------------------------------------------------------------------------
const std::string waitlistStr[8] =
{
    "IF Unit",
    "Pre-Issue Buffer",
	"Pre-ALU Queue",
    "Post-ALU Buffer",
    "Pre-ALUB Queue",
	"Post-ALUB Buffer",
    "Pre-MEM Queue",
	"Post-MEM Buffer"
};
//--------------------------------------------------------------------------
enum buffertype
{
    IFUNIT      = 0,
    PREISSUE    = 1,
    PREALU      = 2,
    POSTALU     = 3,
    PREALUB     = 4,
    POSTALUB    = 5,
    PREMEM      = 6,
    POSTMEM     = 7,
    EXEC        = 8
};
enum quetype
{
    QPOSTALU     = 0,
    QPOSTALUB    = 1,
    QPOSTMEM     = 2,
};
//--------------------------------------------------------------------------
class Inst
{
public:
    Inst():address(0),other(0),rs(1024),rt(1024),rd(1024),sa(0),cycle(0)
    {
        memset(code, 0, sizeof(code));
    }
    std::string OutType();
    
public:
    unsigned int address;
    int other;
    insttype type;
    int cycle;
    unsigned short rs;
    unsigned short rt;
    unsigned short rd;
    unsigned short sa;
    char code[32+2];
};
//--------------------------------------------------------------------------
class InstBuffer
{
public:
    InstBuffer():cur(0),max(4),cycle(0)
    {
        //memset(state, 0, sizeof(state));
    }
    
    InstBuffer(int _max)
    {
        cur = 0;
        max = _max;
        cycle = 0;
        //memset(state, 0, sizeof(state));
    }
    
public:
    int cur;
    int max;
    int cycle;
    //int state[4];
    std::deque<Inst> buffer;

};
//--------------------------------------------------------------------------
class Data
{
public:
    Data():data(0),address(0),busy(false)
    {
        memset(code, 0, sizeof(code));
    }
public:
    int data;
    unsigned int address;
    char code[32+2];
    bool busy;//?
};
//--------------------------------------------------------------------------
class ExecData
{
public:
    ExecData():rd(0),data(0)
    {}
    
    ExecData(short _rd, int _data)
    {
        rd      = _rd;
        data    = _data;
    }
    
    int data;
    short rd;

};
//--------------------------------------------------------------------------
#endif
