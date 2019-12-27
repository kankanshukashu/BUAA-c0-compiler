#include "analyser.h"

using namespace std;

void sout(string file)
{
    ofstream out(file);
    if (!out.is_open()) {
        cout << "output error" << endl;
    }
    else {
        int i = 0;
        out << ".constants:" << endl;
        for (i = 0; i < funcList.size(); i++)
            out << i << " S " << "\"" << funcList[i].name << "\"" << endl;
        out << ".start:" << endl;
        for (i = 0; i < startCode.size(); i++)
            out << i << "    " << startCode[i] << endl;
        out << ".functions:" << endl;
        for (i = 0; i < funcList.size(); i++)
            out << i << " " << i << " " << funcList[i].paramSlot << " 1" << endl;
        for (i = 0; i < funcCode.size(); i++) {
            out << ".F" << i << ":" << endl;
            for (int j = 0; j < funcCode[i].size(); j++)
                out << j << "    " << funcCode[i][j] << endl;
        }
    }
    out.close();
}

void oout(string file)
{
    ofstream out(file, ios::binary);
    if (!out.is_open()) {
        cout << "output error" << endl;
    }
    else {
        char s[8] = { 0x43, 0x30, 0x3a, 0x29, 0x00, 0x00, 0x00, 0x01 };
        out.write(s, 8);
        char cc = 0, *c = &cc;
        cc = (char)((funcList.size()) >> 8);
        out.write(c, 1);
        cc = (char)(funcList.size());
        out.write(c, 1);
        for (int i = 0; i < funcList.size(); i++) {
            cc = (unsigned int)0x00;
            out.write(c, 1);
            cc = (char)(((unsigned int)funcList[i].name.size()) >> 8);
            out.write(c, 1);
            cc = (char)((unsigned int)funcList[i].name.size());
            out.write(c, 1);
            for (int j = 0; j < funcList[i].name.size(); j++) {
                cc = funcList[i].name[j];
                out.write(c, 1);
            }
        }
        cc = (char)((startCode.size()) >> 8);
        out.write(c, 1);
        cc = (char)(startCode.size());
        out.write(c, 1);
        for (int i = 0; i < startCode.size(); i++) {
            if (startCode[i].substr(0, 5) == "ipush") {
                cc = (unsigned int)0x02;
                out.write(c, 1);
                int tmp = stoi(startCode[i].substr(6, 10));
                cc = (char)(tmp >> 24);
                out.write(c, 1);
                cc = (char)(tmp >> 16);
                out.write(c, 1);
                cc = (char)(tmp >> 8);
                out.write(c, 1);
                cc = (char)(tmp);
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 5) == "loada") {
                cc = (unsigned int)0x0a;
                out.write(c, 1);
                cc = (unsigned int)0x00;
                out.write(c, 1);
                if (startCode[i].substr(6, 1) == "0")
                    cc = (unsigned int)0x00;
                else
                    cc = (unsigned int)0x01;
                out.write(c, 1);
                int tmp = stoi(startCode[i].substr(9, 10));
                cc = (char)(tmp >> 24);
                out.write(c, 1);
                cc = (char)(tmp >> 16);
                out.write(c, 1);
                cc = (char)(tmp >> 8);
                out.write(c, 1);
                cc = (char)(tmp);
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 5) == "iload") {
                cc = (unsigned int)0x10;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 6) == "istore") {
                cc = (unsigned int)0x20;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 4) == "iadd") {
                cc = (unsigned int)0x30;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 4) == "isub") {
                cc = (unsigned int)0x34;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 4) == "imul") {
                cc = (unsigned int)0x38;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 4) == "idiv") {
                cc = (unsigned int)0x3c;
                out.write(c, 1);
            }
            else if (startCode[i].substr(0, 4) == "ineg") {
                cc = (unsigned int)0x40;
                out.write(c, 1);
            }
        }
        cc = (char)((funcList.size()) >> 8);
        out.write(c, 1);
        cc = (char)(funcList.size());
        out.write(c, 1);
        for (int i = 0; i < funcList.size(); i++) {
            cc = (char)(i >> 8);
            out.write(c, 1);
            cc = (char)(i);
            out.write(c, 1);
            cc = (char)(funcList[i].paramSlot >> 8);
            out.write(c, 1);
            cc = (char)(funcList[i].paramSlot);
            out.write(c, 1);
            cc = (unsigned int)0x00;
            out.write(c, 1);
            cc = (unsigned int)0x01;
            out.write(c, 1);
            cc = (char)(funcCode[i].size() >> 8);
            out.write(c, 1);
            cc = (char)(funcCode[i].size());
            out.write(c, 1);
            for (int j = 0; j < funcCode[i].size(); j++) {
                if (funcCode[i][j].substr(0, 5) == "ipush") {
                    cc = (unsigned int)0x02;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(6, 10));
                    cc = (char)(tmp >> 24);
                    out.write(c, 1);
                    cc = (char)(tmp >> 16);
                    out.write(c, 1);
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 5) == "loada") {
                    cc = (unsigned int)0x0a;
                    out.write(c, 1);
                    cc = (unsigned int)0x00;
                    out.write(c, 1);
                    if (funcCode[i][j].substr(6, 1) == "0")
                        cc = (unsigned int)0x00;
                    else
                        cc = (unsigned int)0x01;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(9, 10));
                    cc = (char)(tmp >> 24);
                    out.write(c, 1);
                    cc = (char)(tmp >> 16);
                    out.write(c, 1);
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 5) == "iload") {
                    cc = (unsigned int)0x10;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 6) == "istore") {
                    cc = (unsigned int)0x20;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "iadd") {
                    cc = (unsigned int)0x30;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "isub") {
                    cc = (unsigned int)0x34;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "imul") {
                    cc = (unsigned int)0x38;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "idiv") {
                    cc = (unsigned int)0x3c;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "ineg") {
                    cc = (unsigned int)0x40;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "icmp") {
                    cc = (unsigned int)0x44;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 3) == "jmp") {
                    cc = (unsigned int)0x70;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(4, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 2) == "je") {
                    cc = (unsigned int)0x71;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(3, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 3) == "jne") {
                    cc = (unsigned int)0x72;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(4, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 3) == "jge") {
                    cc = (unsigned int)0x74;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(4, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 2) == "jg") {
                    cc = (unsigned int)0x75;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(3, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 3) == "jle") {
                    cc = (unsigned int)0x76;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(4, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 2) == "jl") {
                    cc = (unsigned int)0x73;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(3, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "call") {
                    cc = (unsigned int)0x80;
                    out.write(c, 1);
                    int tmp = stoi(funcCode[i][j].substr(5, 10));
                    cc = (char)(tmp >> 8);
                    out.write(c, 1);
                    cc = (char)(tmp);
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 3) == "ret") {
                    cc = (unsigned int)0x88;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 4) == "iret") {
                    cc = (unsigned int)0x89;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 6) == "iprint") {
                    cc = (unsigned int)0xa0;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 6) == "cprint") {
                    cc = (unsigned int)0xa2;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 6) == "printl") {
                    cc = (unsigned int)0xaf;
                    out.write(c, 1);
                }
                else if (funcCode[i][j].substr(0, 5) == "iscan") {
                    cc = (unsigned int)0xb0;
                    out.write(c, 1);
                }
            }
        }
    }
    out.close();
}
