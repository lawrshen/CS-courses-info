//func_man.cpp

#include<iostream>
using namespace std;
#include<fstream>
#include<unistd.h>
#include<ncurses.h>
#include"command.h"
const string pre="/myShell/man/";
const string suff="_man.txt";

void man(const string option)
{
	//cd为内建命令没有man手册
	if(option=="cd"){
		cout<<"没有cd的手册页条目\n";
		return;
	}
	else if(option!="cp"&&option!="cmp"&&option!="cat"&&option!="wc"&&option!="man"&&option!="sh")
		return;
	//清屏
	WINDOW* scr=initscr();
	clear();
	//执行
	string prev=getcwd(NULL,0);
	int pos=prev.find("/myShell");
	prev=prev.substr(0,pos);
	string file=prev+pre+option+suff;//man存放的绝对路径
	print_man(file);
	//退出
	addstr("\npress 'q' to quit\n");
	refresh();
	char ch;
	while(ch=getch())
	{
		if(ch=='q')
			break;
		continue;
	}
	endwin();
	//clearok(scr,1);
	//refresh();
	return;
}

void print_man(const string file)
{
	ifstream in_file(file,ios::in);
	if(!in_file)
		return;	
	string ss;
	while(!in_file.eof())
	{
		getline(in_file,ss,'\n');
		if(in_file.eof())
			break;
		addstr(ss.c_str());
		addch('\n');
	}
}
