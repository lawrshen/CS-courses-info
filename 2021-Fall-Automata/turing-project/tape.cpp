/*
 * File: tape.cpp
 * Project: automata
 * File Created: 2021/12/10
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#include "tape.h"
#include <cstdio>

tape::tape(int id_,const char& blank_,const string &input_) {
    for(int i=0;i<input_.size();i++){
        tape_symbols.insert({i,input_[i]});
    }
    head = 0;
    id=id_;
    blank=blank_;
}

tape::tape(int id_,const char &blank_) {
    tape_symbols.insert({0,blank_});
    head = 0;
    id=id_;
    blank=blank_;
}

void tape::TransCurSymbol(char &sym, char &dir) {
    if(sym!='*'){
        tape_symbols[head]=sym;
    }
    if(dir=='l'){
        head--;
    }else if(dir=='r'){
        head++;
    }
    if(tape_symbols.count(head)==0){
        tape_symbols[head] = blank;
    }
}

const char &tape::GetCurSymbol() {
    return tape_symbols[head];
}

void tape::CatchTapeID(){
    printf("Index%d : ",id);
    bool flag = true;
    for(auto& idx:tape_symbols){
        if(flag&&idx.second==blank&&idx.first<head){
            continue;
        }else{
            flag = true;
        }
        if(idx.second!=blank||idx.first<=head){
            printf("%d ",abs(idx.first));
        }
    }
    printf("\n");
    printf("Tape%d  : ",id);
    flag = true;
    for(auto& idx:tape_symbols){
        if(flag&&idx.second==blank&&idx.first<head){
            continue;
        }else{
            flag = true;
        }
        if(idx.second!=blank||idx.first<=head){
            int num=abs(idx.first);
            int cnt = num==0? 2:1;
            while(num>0){
                num/=10;
                cnt++;
            }
            string str = "%-"+to_string(cnt)+"c";
            printf(str.c_str(),idx.second); 
        }
    }
    printf("\n");
    printf("Head%d  : ",id);
    for(int i=getFirstElePos();i<head;i++){
        int num=abs(i);
        int cnt = num==0? 2:1;
        while(num>0){
            num/=10;
            cnt++;
        }
        string str = "%-"+to_string(cnt)+"c";
        printf(str.c_str(),' '); 
    }
    printf("^\n");
}

const string tape::GetTapeStr() const {
    string str;
    for(auto& t:tape_symbols){
        if(t.second!=blank){
            str+=t.second;
        }
    }
    return str;
}

int tape::getFirstElePos(){
    int pos=0;
    for(auto& t:tape_symbols){
        pos=t.first;
        if(t.second!=blank){
            break;
        }
    }
    return pos;
}
