//func_cat.cpp

#include<iostream>
using namespace std;
#include<fstream>
#include"command.h"

void cat(const string file)
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
		cout<<ss<<endl;
	}
}
