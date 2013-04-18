//
//  main.cpp
//  MIPSsimTest
//
//  Created by Mengdi Wang on 13-4-17.
//  Copyright (c) 2013年 Mengdi Wang. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

/*
class splitstring : public std::string {
    std::vector<std::string> flds;
public:
    splitstring(const char *s):std::string(s) { };
    std::vector<std::string>& split(char delim, int rep=0);
};

// split: receives a char delimiter; returns a vector of strings
// By default ignores repeated delimiters, unless argument rep == 1.
std::vector<std::string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    std::string work = data();
    std::string buf = "";
    int i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}
*/

const std::string FIVE0 = "00000";
std::string GenInst(std::string line, bool &isbreak);
int ReadFile(const char *input, const char *output);
//---------------------------------------------------------
std::string GetArgStr(std::string line)
{
    int pos = line.find(' ');
    return line.substr(pos+1);
}

void operationSpecial(std::string line, int &rd, int &rs, int &rt)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, R%d, R%d", &rd, &rs, &rt);
}

void operationBranch(std::string line, int &rs, int &offset)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, #%d", &rs, &offset);
}

void operationJ(std::string line, int &imm)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "#%d", &imm);
}

void operationJR(std::string line, int &rs)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(),"R%d", &rs);
}

void operationShift(std::string line, int &rd, int &rt, int &sa)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, R%d, R%d", &rd, &rt, &sa);
}

void operationCat2(std::string line, int &rs, int &rt, int &im)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, R%d, #%d", &rt, &rs, &im);
}

void operationBXTZ(std::string line, int &rs, int &offset)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, #%d", &rs, &offset);
}

void operationXW(std::string line, int &rt, int &rs, int &offset)
{
    std::string linearg = GetArgStr(line);
    sscanf(linearg.c_str(), "R%d, %d(R%d)", &rt, &offset, &rs);
}

std::string fiveb2str(int input)
{
    int LEN = 5;
    std::string ret = "00000";
    
    for(int i=LEN-1; i>=0; i--)
        ret[LEN-i-1] = ((input >> i) & 1)?'1':'0';
    return ret;
}

std::string sixteenb2str(unsigned short input)
{
    int LEN = 16;
    //int ori = input >> 2;
    std::string ret = "0000000000000000";
    for(int i=LEN-1; i>=0; i--)
        ret[LEN-i-1] = ((input >> i) & 1)?'1':'0';
    return ret;
}

std::string twentyb2str(int input)
{
    unsigned int tmp = (unsigned) input;
    int LEN = 26;
    std::string ret = "00000000000000000000000000";
    for(int i=LEN-1; i>=0; i--)
        ret[LEN-i-1] = ((tmp >> i) & 1)?'1':'0';
    return ret;
    
}

int main(int argc, const char * argv[])
{

    // insert code here...
    if(argc < 3)
    {
        std::cout << "Usage:MIPSsimTest <input> <output>\n";
        return 1;
    }
    
    ReadFile(argv[1], argv[2]);
    
    return 0;
}

std::string GetNum(std::string line)
{
    int s = 0;
    std::string ret = "00000000000000000000000000000000";
    sscanf(line.c_str(), "%d", &s);
    unsigned tmp = (unsigned)s;
    int LEN = 32;
    for(int i=LEN; i>=0; i--)
    {
        ret[LEN-i-1] = ((tmp>>i)&1) ? '1':'0';
    }
    return ret;
}

int ReadFile(const char *input, const char *output)
{
    std::ifstream instream(input);
    std::string line;
    std::stringstream ret;
    bool isbreak = false;
    
    if(instream.is_open())
    {
        while(std::getline(instream,line))
        {
            if(!isbreak)
            {
                std::string sret = GenInst(line, isbreak);
                std::cout << sret << std::endl;
                ret << sret << '\n';
            }
            else
            {
                std::string sret = GetNum(line);
                std::cout << sret << std::endl;
                ret << sret << '\n';
            }
        }
        instream.close();
    }
    else
    {
        return -1;
    }
    
    //std::cout << ret.str() << std::endl;
    
    std::ofstream outstream(output);
    if(outstream.is_open())
    {
        outstream << ret.str();
    }
    
    return 0;
}

std::string GenInst(std::string line, bool &isbreak)
{
    bool unfind = true;
    std::string ret = "";
    std::string id = line.substr(0,3);
    int rd = 0;
    int rs = 0;
    int rt = 0;
    int sa = 0;
    int other = 0;
    
    if(unfind && id == "BEQ")
    {
        unfind = false;
        ret += "000100";
        operationBranch(line, rs, other);
        ret += fiveb2str(rs);
        ret += fiveb2str(rt);
        ret += sixteenb2str((signed short)other>>2);
    }
    else if(unfind && id == "BLT")
    {
        unfind = false;
        ret += "000001";
        operationBXTZ(line, rs, other);
        ret += fiveb2str(rs);
        ret += FIVE0;
        ret += sixteenb2str((signed short)other>>2);
    }
    else if(unfind && id == "BGT")
    {
        unfind = false;
        ret += "000111";
        operationBXTZ(line, rs, other);
        ret += fiveb2str(rs);
        ret += FIVE0;
        ret += sixteenb2str((signed short)other>>2);
    }
    else if(unfind && id == "SLL")
    {
        unfind = false;
        ret += "000000";
        operationShift(line, rd, rt, sa);
        ret += FIVE0;
        ret += fiveb2str(rt);
        ret += fiveb2str(rd);
        ret += fiveb2str(sa);
        ret += "000000";
    }
    else if(unfind && id == "SRL")
    {
        unfind = false;
        ret += "000000";
        operationShift(line, rd, rt, sa);
        ret += FIVE0;
        ret += fiveb2str(rt);
        ret += fiveb2str(rd);
        ret += fiveb2str(sa);
        ret += "000010";

    }
    else if(unfind && id == "SRA")
    {
        unfind = false;
        ret += "000000";
        operationShift(line, rd, rt, sa);
        ret += FIVE0;
        ret += fiveb2str(rt);
        ret += fiveb2str(rd);
        ret += fiveb2str(sa);
        ret += "000011";

    }
    else if(unfind && id == "ADD")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "110000";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "000000";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "100000";
        }
    }
    else if(unfind && id == "SUB")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "110001";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "000000";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "100010";
        }
    }
    else if(unfind && id == "MUL")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "100001";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "011100";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "000010";
        }
    }
    else if(unfind && id == "AND")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "110010";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "000000";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "100100";
        }
    }
    else if(unfind && id == "XOR")
    {
        unfind = false;
        ret += "000000";
        operationSpecial(line, rd, rs, rt);
        ret += fiveb2str(rs);
        ret += fiveb2str(rt);
        ret += fiveb2str(rd);
        ret += FIVE0;
        ret += "100110";
    }
    else if(unfind && id == "NOR")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "110011";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "000000";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "100111";
        }
    }
    else if(unfind && id == "SLT")
    {
        unfind = false;
        if(line.find('#')!=std::string::npos)
        {
            ret += "110101";
            operationCat2(line, rs, rt, other);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += sixteenb2str((signed short)other);
        }
        else
        {
            ret += "000000";
            operationSpecial(line, rd, rs, rt);
            ret += fiveb2str(rs);
            ret += fiveb2str(rt);
            ret += fiveb2str(rd);
            ret += FIVE0;
            ret += "101010";
        }
    }
    else if(unfind && id == "BRE")
    {
        unfind = false;
        ret = "00000000000000000000000000001101";
        isbreak = true;
    }
    else if(unfind && id == "NOP")
    {
        unfind = false;
        ret = "00000000000000000000000000000000";
    }
    
    id = line.substr(0,2);
    if(unfind && id == "JR")
    {
        unfind = false;
        ret += "000000";
        operationJR(line, rs);
        ret += fiveb2str(rs);
        ret += FIVE0;
        ret += FIVE0;
        ret += FIVE0;
        ret += "001000";
    }
    else if(unfind && id == "SW")
    {
        unfind = false;
        ret += "101011";
        operationXW(line, rt, rs, other);
        ret += fiveb2str(rs);
        ret += fiveb2str(rt);
        ret += sixteenb2str(other);
    }
    else if(unfind && id == "LW")
    {
        unfind = false;
        ret += "100011";
        operationXW(line, rt, rs, other);
        ret += fiveb2str(rs);
        ret += fiveb2str(rt);
        ret += sixteenb2str(other);
    }
    else if(unfind && id == "OR")
    {
        unfind = false;
        ret += "000000";
        operationSpecial(line, rd, rs, rt);
        ret += fiveb2str(rs);
        ret += fiveb2str(rt);
        ret += fiveb2str(rd);
        ret += FIVE0;
        ret += "100101";
    }
    
    id = line.substr(0,1);
    if(unfind && id == "J")
    {
        unfind = false;
        ret += "000010";
        operationJ(line, other);
        ret += twentyb2str(other >> 2);
    }
    
    return ret;
}

