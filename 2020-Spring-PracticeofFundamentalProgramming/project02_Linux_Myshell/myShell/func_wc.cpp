//func_wc.cpp

#include<iostream>
using namespace std;
#include<fstream>
#include"command.h"

void wc(const string option,const string file)
{
	fstream in_file(file,ios::in);
	if(!in_file){
		cerr<<file<<" open fail\n";
		return;}
	int byte=0,line=0,word=0,wordline=0;
	bool blank_flag=true,add=false;//连续的分隔符问题
	string ss;
	while(!in_file.eof())
	{
		getline(in_file,ss,'\n');
		if(in_file.eof())
			break;
		++line;
		if(ss!="")
			++wordline;
		int len=ss.size();
		for(int i=0;i<len;++i){
			if(ss[i]!=' '&&ss[i]!='\t')
				blank_flag=true;
			byte+=sizeof(ss[i]);
			if((ss[i]==' '||ss[i]=='\t')&&blank_flag)
			{	++word;
				add=true;
				blank_flag=false;
			}
			if((i==0||i==len-1)&&(ss[i]==' '||ss[i]=='\t')&&add)
			{	--word;
				add=false;
			}
		}
		++byte;
	}	
	word+=wordline;
	if(option=="-c")
		cout<<byte<<" "<<file<<endl;
	else if(option=="-w")
		cout<<word<<" "<<file<<endl;
	else if(option=="-l")
		cout<<line<<" "<<file<<endl;
	else if(option=="")
		cout<<line<<" "<<word<<" "<<byte<<" "<<file<<endl;
	in_file.close();
}
