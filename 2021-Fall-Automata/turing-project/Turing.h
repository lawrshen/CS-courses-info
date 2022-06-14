/*
 * File: Turing.h
 * Project: automata
 * File Created: 2021/12/7
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#ifndef AUTOMATA_TURING_H
#define AUTOMATA_TURING_H
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "delta.h"
#include "tape.h"
using namespace std;

class Turing {
public:
    Turing(const string& file_name,const string& input_,bool mode_);
    void Run();
private:
    unordered_set<string> q_states,final_states;
    unordered_set<char> input_symbols,tape_symbols;
    string start_symbol;
    char blank_symbol;
    int tape_num;
    unordered_map<string,vector<delta>> transfunc;
    vector<tape> tapes;

    string input;
    bool verbose_mode;
    enum tmErrors { tmPseudoError,tmSyntaxError,tmInputError,tmUndefSymError,
                    tmDuplicateError,tmUndeclaredError,tmFileError };

    void halt();
    void parseTM(const string& file_name);
    void catchID(int step,const string& cur_state);
    void throwTmError(enum tmErrors err,string info= "char");
};  

#endif //AUTOMATA_TURING_H
