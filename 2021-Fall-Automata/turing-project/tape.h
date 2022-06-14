/*
 * File: tape.h
 * Project: automata
 * File Created: 2021/12/10
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#ifndef AUTOMATA_TAPE_H
#define AUTOMATA_TAPE_H
#include <string>
#include <map>
using namespace std;

class tape {
public:
    tape(int id_,const char& blank_,const string& input_);
    tape(int id_,const char& blank_);
    void TransCurSymbol(char& sym,char& dir);
    const char& GetCurSymbol();
    void CatchTapeID();
    const string GetTapeStr()const;
private:
    map<int,char> tape_symbols;
    char blank;
    int head,id;
    int getFirstElePos();
};


#endif //AUTOMATA_TAPE_H
