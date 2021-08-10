#ifndef COMMAND_H
#define COMMAND_H
#include<string>

//欢迎界面
void welcome_menu();
bool get_command(const string str,const string s);
//具体功能
int copy_file(const string file1,string file2);
void copy_all(const string str1,string str2);
void search(const string str1,const string str2);
void copy_dir(const string dest_path);
int cmp(const string file1,const string file2);
void wc(const string option,const string file);
void cat(const string file);
void man(const string option);
void print_man(const string file);
int sh(const string file);
void my_cd(string path);
#endif
