/*
 * File: main.cpp
 * Project: automata
 * File Created: 2021/12/7
 * Author: Tianjie Shen (lawrshen@smail.nju.edu.cn)
 * -----
 * Copyright 2021 NJU, Tianjie Shen
 */

#include <cstdio>
#include <cstring>
#include <getopt.h>
#include "Turing.h"

int main(int argc, char** argv){
    static struct option tm_options[] = {
        {"verbose", no_argument, NULL, 'v'},
        {"help"   , no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };
    int opt = 0;
    bool verbose_mode = false; 
    while(EOF != (opt = getopt_long(argc,argv,"hv",tm_options,NULL))){
        switch(opt){
            case 'h':
                printf("usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
                break;
            case 'v':
                verbose_mode = true;
                break;
            default:
                break;
        }
    }

    if (optind + 2 == argc){
        Turing* tm=new Turing(argv[optind],argv[optind+1],verbose_mode);
        tm->Run();
    }else{
        fprintf(stderr,"non-match argu numbers\nTry 'turing --help' for more information.\n");
    }

    return 0;
}