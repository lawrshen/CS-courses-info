/*
 * File: delta.h
 * Project: automata
 * File Created: 2021/12/9
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#ifndef AUTOMATA_DELTA_H
#define AUTOMATA_DELTA_H
#include<string>
#include<vector>
using namespace std;

class delta {
public:
    delta();
    delta(string old_state_,string new_state_,
    vector<char> old_symbols_,vector<char> new_symbols_,
    vector<char> dirs_);
    bool empty();
    bool operator< (const delta& other) const;

    const string &GetOldtState() const {return old_state;}
    const string &GetNewtState() const {return new_state;}
    const vector<char> &GetOldSymbols() const {return old_symbols;}
    const vector<char> &GetNewSymbols() const {return new_symbols;}
    const vector<char> &GetDirs() const {return dirs;}

private:
    string old_state,new_state;
    vector<char> old_symbols,new_symbols,dirs;

};


#endif //AUTOMATA_DELTA_H
