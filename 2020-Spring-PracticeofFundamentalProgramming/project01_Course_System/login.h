#pragma once
#ifndef LOGIN_H_
#define LOGIIN_H_
#include<string>
using namespace std;
const int mid = 26;
const int centre = 35;
const int jump_time = 3000;

void gotoxy(int x, int y);
void encrypt(string& password);//密码加密
int start_ui();
void log_in(int chosen_path);
void welcome_ui();
void return_menu();//返回主页面

#endif // !LOGIN_H_
