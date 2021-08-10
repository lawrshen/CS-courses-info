//menu.cpp

#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include"command.h"

bool get_command(const string str,const string s)
{
	return s==str.substr(0,s.length());
}

void welcome_menu()
{
	char name[65];
	gethostname(name,sizeof(name));
	string uid=name;
	int spli=uid.find("-");
	uid=uid.substr(0,spli);
	cout<<"\033[33mwelcome to myShell\033[0m\n"
	    <<"\033[36mEight commands:cp cmp wc cat man sh cd exit\033[0m\n";
	while(true){
		cout<<"\033[37m"<<uid<<"@"<<name<<"\033[0m:";
		cout<<"\033[34m"<<getcwd(NULL,0)<<"\033[0m$ ";
		string order;
		cin>>order;
		if(get_command(order,"cp"))
		{
			string ss;
			getline(cin,ss);
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
			if(strs.size()==2)
			{
				char* dest=const_cast<char*>(strs[1].c_str());
				struct stat state_of_entry;
				lstat(dest,&state_of_entry);
				if(S_ISDIR(state_of_entry.st_mode))//cp file dir
					strs[1]=strs[1]+"/"+strs[0];
				copy_file(strs[0],strs[1]);
			}
			else if(strs.size()==3&&strs[0]=="-r")
			{
				char* dest=const_cast<char*>(strs[2].c_str());
				struct stat state_of_dest;
				lstat(dest,&state_of_dest);
				if(S_ISDIR(state_of_dest.st_mode))
					strs[2]+="/"+strs[1];
				copy_all(strs[1],strs[2]);
			}	
		}
		else if(get_command(order,"cmp"))
		{
			string file1,file2;
			cin>>file1>>file2;
			cmp(file1,file2);
		}
		else if(get_command(order,"wc"))
		{	
			string ss;
			getline(cin,ss);
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
			if(strs.size()==1)
				wc("",strs[0]);
			else if(strs.size()==2)
				wc(strs[0],strs[1]);
		}
		else if(get_command(order,"cat"))
		{
			string file;
			cin>>file;
			cat(file);
		}
		else if(get_command(order,"man"))
		{
			string option;cin>>option;
			man(option);
		}
		else if(get_command(order,"sh"))
		{
			string file;
			cin>>file;
			sh(file);
		}
        else if(get_command(order,"cd"))
		{
			string path;
			cin>>path;
			my_cd(path);
		}
		else if(get_command(order,"exit"))
		{
			cout<<"Exit\n";
			break;
		}
		else if(get_command(order,""))
		{
			cout<<"empty command\n";
		}
		else
			cout<<"No such command\n";
	}
}

