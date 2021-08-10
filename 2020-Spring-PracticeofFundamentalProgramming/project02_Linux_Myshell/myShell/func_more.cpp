#include<iostream>
using namespace std;
#include<fstream>
#include<string>
#include<vector>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include"command.h"

void my_cd(string path)
{
	if(path==".."||path==".")
	{
		chdir(path.c_str());
		return;
	}
	string prev=getcwd(NULL,0);
	string curr=prev+"/"+path;
	chdir(curr.c_str());
	if(prev==getcwd(NULL,0))
	{
		cout<<"No such path>..\n";return;
	}
	else
		return;
}

int sh(const string file)
{	
	if(access(file.c_str(),R_OK))
	{
		cerr<<file<<" Permission denied\n";
		return -1;
	}
	ifstream in_file(file,ios::in);
	if(!in_file)
		return -1;	
	string ss;
	while(!in_file.eof())
	{
		getline(in_file,ss,'\n');
		if(in_file.eof())
			break;
		if(ss=="")
			continue;
		//按空格分割一个字符串
		ss+=' ';
		vector<string> strs;
		int len=ss.size();
		for(int i=0;i<len;++i)
		{
			int pos=ss.find(' ',i);
			if(pos<len){
				string str=ss.substr(i,pos-i);
				if(str!="")
				strs.push_back(str);
				i=pos;
			}
		}
		//自动执行命令
		string order=strs[0];
		if(get_command(order,"cp"))
		{
			if(strs.size()==3)
			{
				char* dest=const_cast<char*>(strs[2].c_str());
				struct stat state_of_entry;
				lstat(dest,&state_of_entry);
				if(S_ISDIR(state_of_entry.st_mode))//cp file dir
					strs[2]=strs[2]+"/"+strs[1];
				copy_file(strs[1],strs[2]);
			}
			else if(strs.size()==4&&strs[1]=="-r")
			{
				char* dest=const_cast<char*>(strs[3].c_str());
				struct stat state_of_dest;
				lstat(dest,&state_of_dest);
				if(S_ISDIR(state_of_dest.st_mode))
					strs[3]+="/"+strs[2];
				copy_all(strs[2],strs[3]);
			}
			
		}
		else if(get_command(order,"cmp"))
		{
			cmp(strs[1],strs[2]);
		}
		else if(get_command(order,"wc"))
		{	
			if(strs.size()==2)
				wc("",strs[1]);
			else if(strs.size()==3)
				wc(strs[1],strs[2]);
		}
		else if(get_command(order,"cat"))
		{
			cat(strs[1]);
		}
		else if(get_command(order,"man"))
		{
			man(strs[1]);
		}
		else 
		{
			cout<<" no command\n";
		}	
	}
	in_file.close();
	return 0;
}
