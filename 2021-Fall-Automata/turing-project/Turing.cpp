/*
 * File: Turing.cpp
 * Project: automata
 * File Created: 2021/12/7
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#include "Turing.h"
#include <fstream>
#include <iostream>
#include <vector>
#include<algorithm>

static const string SPLITTER(24,'=');
#define VERBOSE_SPLITTER(str) cout<<SPLITTER<<" "<<#str<<" "<<SPLITTER<<endl;

Turing::Turing(const string& file_name,const string& input_,bool mode_) {
    verbose_mode = mode_;
    parseTM(file_name);
    for(char ch:input_){
        if(input_symbols.count(ch)==0){
            throwTmError(tmUndeclaredError,string(1,ch));
        }
    }
    input = input_;
    tapes.push_back(tape(0,blank_symbol,input));
    for(int i=1;i<tape_num;i++){
        tapes.push_back(tape(i,blank_symbol));
    }
}

void Turing::Run() {
    int step = 0;
    string cur_state = start_symbol;
    if(verbose_mode){
        VERBOSE_SPLITTER(RUN);
    }
    // start running
    while(final_states.count(cur_state)==0){
        vector<char> cur_symbols;
        for(auto t:tapes){
            cur_symbols.push_back(t.GetCurSymbol());
        }
        auto trans = transfunc[cur_state];
        delta d;
        for(auto t:trans){
            vector<char> old_symbols = t.GetOldSymbols();
            int i=0;
            for(;i<tape_num;i++){
                if(cur_symbols[i]!=old_symbols[i]&&old_symbols[i]!='*'){
                    break;
                }
            }
            if(i==tape_num){
                d=t;
                break;
            }else{
                continue;
            }
        }
        if(verbose_mode){
            catchID(step,cur_state);
            step++;
        }
        if(d.empty()){ // no transfunc
            halt();
        }
        auto new_symbols = d.GetNewSymbols(),dir = d.GetDirs();
        for(int i=0;i<tape_num;i++){
            tapes[i].TransCurSymbol(new_symbols[i],dir[i]);
        }
        cur_state = d.GetNewtState();
    }
    if(verbose_mode){
        catchID(step,cur_state);
        printf("Result : %s\n",tapes[0].GetTapeStr().c_str());
        VERBOSE_SPLITTER(END);
    }else{
        cout<<tapes[0].GetTapeStr()<<endl;
    }
}

void Turing::parseTM(const string& file_name) {
    ifstream in(file_name);
    if(in.fail()){
        throwTmError(tmFileError,file_name);
    }
    string line;
    vector<string> transStrs;
    while(getline(in, line)){
        if(line.compare(0,1,"#")==0){
            char type = line.substr(1,1)[0];
            switch (type){
            case 'Q':
            case 'S':
            case 'G':
            case 'F':{
                size_t lcPos = line.find('{'),rcPos = line.find('}');
                if(lcPos == line.npos || rcPos == line.npos){
                    throwTmError(tmSyntaxError);
                } 
                string strs = line.substr(lcPos+1,rcPos-lcPos-1);
                strs+=",";
                size_t pos = strs.find(',');
                while(pos != strs.npos){
                    string temp = strs.substr(0, pos);
                    switch (type){
                        case 'Q':
                            if(!q_states.insert(temp).second)
                                throwTmError(tmDuplicateError);
                                break;
                        case 'S':
                            if(!input_symbols.insert(temp[0]).second)
                                throwTmError(tmDuplicateError);
                                break;
                        case 'G':
                            if(!tape_symbols.insert(temp[0]).second)
                                throwTmError(tmDuplicateError);
                                break;
                        case 'F':
                            if(!final_states.insert(temp).second)
                                throwTmError(tmDuplicateError);
                                break;
                        default:break;
                    }
                    strs = strs.substr(pos+1, strs.size());
                    pos = strs.find(",");
                }
                break;
            }
            case 'q':{
                start_symbol = line.substr(6,line.size()-6);
                break;
            }
            case 'B':{
                blank_symbol = line.substr(5,1)[0];
                break;
            }
            case 'N':{
                tape_num = stoi(line.substr(5,line.size()-5));
                break;
            }
            default:
                break;
            }
        }
        else if(line.compare(0,1,";")!=0&&!line.empty()){
            transStrs.push_back(line);
        }
    }
    for(auto& line:transStrs){
        // parse tm file deltas
        line+=" ";
        size_t pos = line.find(" ");
        vector<string> trans;
        while(pos != line.npos&&trans.size()<5){
            trans.push_back(line.substr(0, pos));
            line = line.substr(pos+1, line.size());
            pos = line.find(" ");
        }
        if(trans.size()!=5){
            throwTmError(tmSyntaxError);
        }
        for(int i=1;i<4;i++){
            if(trans[i].size()!=tape_num){
                throwTmError(tmSyntaxError);
            }
        }
        // prepare for deltas
        string old_state = trans[0], new_state = trans[4];
        if(q_states.count(old_state)==0){
            throwTmError(tmUndefSymError,old_state);
        }else if(q_states.count(new_state)==0){
            throwTmError(tmUndefSymError,new_state);
        }
        vector<char> old_symbols,new_symbols,dirs;
        for(char ch:trans[1]){
            if(tape_symbols.count(ch)==0&&ch!='*'){
                throwTmError(tmUndefSymError,string(1,ch));
            }
            old_symbols.push_back(ch);
        }
        for(char ch:trans[2]){
            if(tape_symbols.count(ch)==0&&ch!='*'){
                throwTmError(tmUndefSymError,string(1,ch));
            }
            new_symbols.push_back(ch);
        }
        for(char ch:trans[3]){
            if(ch!='l'&&ch!='r'&&ch!='*'){
                throwTmError(tmUndefSymError,string(1,ch));
            }
            dirs.push_back(ch);
        }
        // construct deltas
        transfunc[old_state].push_back(delta(old_state,new_state,old_symbols,new_symbols,dirs));
    }
    for(auto& p:transfunc){
        sort(p.second.begin(),p.second.end());
    }
}

void Turing::catchID(int step,const string& cur_state){
    printf("Step   : %d\n",step);
    for(auto& t:tapes){
        t.CatchTapeID();
    }
    printf("State  : %s\n",cur_state.c_str());
    string id_spilter(40,'-');
    cout<<id_spilter<<endl;
}

void Turing::halt(){
    if(verbose_mode){
        printf("Result : %s\n",tapes[0].GetTapeStr().c_str());
        VERBOSE_SPLITTER(END);
    }else{
        cout<<tapes[0].GetTapeStr()<<endl;
    }
    exit(0);
}

void Turing::throwTmError(Turing::tmErrors err,string info) {
    if(verbose_mode){
        cerr<<SPLITTER<<" ERR "<<SPLITTER<<endl;;
        switch (err){
            case tmUndefSymError:
                cerr<<"State OR Symbol '"<<info<<"' undefined\n";
                break;
            case tmUndeclaredError:
                cerr<<"'"<<info<<"' was not declared in the set of input symbols\n";
                break;
            case tmFileError:
                cerr<<"File '"<<info<<"' was unable to open\n";
                break;
            default:
                cerr<<"undefined error\n";
                break;
            }
        VERBOSE_SPLITTER(END);
        exit(-1);
    }else{
        switch (err) {
            case tmSyntaxError:
            case tmUndefSymError:
                cerr<<"syntax error\n";
                exit(tmSyntaxError);
            case tmUndeclaredError:
            case tmFileError:
            case tmInputError:
                cerr<<"illegal input\n";
                exit(tmInputError);
            default:
                cerr<<"undefined error\n";
                exit(tmPseudoError-1);
        }
    }
}
