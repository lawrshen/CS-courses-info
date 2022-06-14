/*
 * File: delta.cpp
 * Project: automata
 * File Created: 2021/12/9
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#include "delta.h"


delta::delta(): old_state(""),new_state("") {}

delta::delta(string old_state_, string new_state_,
             vector<char> old_symbols_, vector<char> new_symbols_,
             vector<char> dirs_) : old_state(old_state_), new_state(new_state_),
                                   old_symbols(old_symbols_), new_symbols(new_symbols_), dirs(dirs_) {}

bool delta::empty() {
    return old_state.empty()||new_state.empty();
}

bool delta::operator<(const delta &other) const {
    int n = old_symbols.size(),i=0;
    while(old_symbols[i]==other.old_symbols[i]&&i<n){
        i++;
    }
    if(i==n){
        return true;
    }else if(old_symbols[i]=='*'){
        return false;
    }else if(other.old_symbols[i]=='*'){
        return true;
    }else{
        return old_symbols[i] < other.old_symbols[i];
    }
}

