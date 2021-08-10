//func_cnp.cpp

#include<iostream>
using namespace std;
#include<fstream>
#include<iterator>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include"command.h"

int copy_file(const string file1,string file2)
{
	if(access(file1.c_str(),R_OK))
	{
		cerr<<file1<<" Permission denied\n";
		return -1;
	}
	ifstream in_file(file1,ios::in);
	if(!in_file){
		in_file.close();
		cerr<<file1<<" open error\n";
		return -1;
	}
	ofstream out_file(file2,ios::out);
	if(out_file.fail()){
		in_file.close();
		out_file.close();
		cerr<<file2<<" open wrong\n";
		return -1;
	}
	if(access(file2.c_str(),W_OK))
	{
		cerr<<file2<<" Permission denied\n";
		return -1;
	}
	in_file>>noskipws;//读取输入流的空白字符
	istream_iterator<char> is(in_file);
	istream_iterator<char> eof;
	ostream_iterator<char> os(out_file);
	copy(is,eof,os);
	in_file.close();
	out_file.close();
	return 0;
}

void copy_all(const string str1,string str2)
{
	char* src=const_cast<char*>(str1.c_str());
	struct stat state_of_scr;
	lstat(src,&state_of_scr);
	if(S_ISDIR(state_of_scr.st_mode))
	{
		copy_dir(str2);
		search(str1,str2);
	}
	else if(S_ISREG(state_of_scr.st_mode))
		copy_file(str1,str2);	
}

void search(const string str1,const string str2)
{
	char* src_path=const_cast<char*>(str1.c_str());
	char* dest_path=const_cast<char*>(str2.c_str());
	DIR *src_dir=opendir(src_path);
	DIR *dest_dir=opendir(dest_path);
	struct dirent* entry=NULL;
	struct stat state_of_entry;
	char* curr_dir=getcwd(NULL,0);
	while((entry=readdir(src_dir))!=NULL)
	{
		if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)
			continue;
		string abs_src=curr_dir;
		abs_src+="/"+str1+"/"+entry->d_name;
		char* ch_src=const_cast<char*>(abs_src.c_str());
		stat(ch_src,&state_of_entry);
		if(S_ISDIR(state_of_entry.st_mode))
		{
			string src=str1+"/"+entry->d_name;
			string dest=str2+"/"+entry->d_name;
            		copy_dir(dest);
			search(src,dest);
		}
		else if(S_ISREG(state_of_entry.st_mode))
		{
			string src_file=str1+"/"+entry->d_name;
			string dest_file=str2+"/"+entry->d_name;
            		copy_file(src_file, dest_file);
		}
	}
}

void copy_dir(const string dest_path)
{
	mkdir(dest_path.c_str(),0777);
}

