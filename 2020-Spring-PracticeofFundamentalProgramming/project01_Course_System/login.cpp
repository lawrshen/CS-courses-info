#include<iostream>
#include<fstream>
#include<string>
#include<conio.h>
#include<Windows.h>
#include"login.h"
#include"admin.h"
#include"students.h"
using namespace std;
const string LINE(49,'=');
const string Border(16, '*');

int start_ui()
{
	system("color FD");
	gotoxy(mid+8, 8);cout << "欢迎使用学生选课信息管理系统";
	gotoxy(mid,10);	cout << LINE ;
	gotoxy(mid, 11);cout << " 1.学生登录	 2.学生注册	3.管理员登录" ;
	gotoxy(mid, 12);cout << LINE ;
	gotoxy(mid+8, 14);cout << "请输入1或2或3进行操作:";
	int i,line=15;
	while( (!(cin >> i))|| (i > 3 || i < 1) ){	//防止输错字符导致错误 超范围错误
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		gotoxy(mid, ++line);cout << "操作失败，请正确输入1或2或3进行操作:";
	}
	return i;
}

void log_in(int chosen_path)
{
	ofstream file1;file1.open("student.txt",ios::app);file1.close();
	ofstream file2;file2.open("assistant.txt",ios::app);file2.close();
	switch (chosen_path)
	{
	case 1:		//学生登录
	{
		system("cls");
		string stu_id, stu_password;
		gotoxy(centre, 10);cout << "请输入学生ID: ";cin >> stu_id;
		gotoxy(centre, 11);cout << "请输入学生个人密码: ";encrypt(stu_password);
		string stucoursefile = stu_log(stu_id, stu_password);
		gotoxy(centre - 8, 14);cout << Border << "登录成功！ ！(3秒后跳转)" << Border << endl << endl;
		Sleep(jump_time);
		system("cls");
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {
			gotoxy(centre, 11);cout << "请管理员先录入课程信息！";
			system("pause");
			return_menu();
		}
		stu_func(stucoursefile);
		system("cls");
		change_password(stu_id, stu_password);
		break;
	}
	case 2:		//学生注册
	{
		system("cls");
		string stu_id, stu_password,check, stucoursefile;
		gotoxy(centre, 10);cout << "请输入学生ID进行注册: ";	cin >> stu_id;
		gotoxy(centre, 11);cout << "请输入学生个人密码进行注册: ";	cin>>stu_password;
		gotoxy(centre, 12);cout << "请再输入学生个人密码确认: ";	cin >> check;
		if(check==stu_password)
			stucoursefile = stu_reg(stu_id, stu_password);
		else
		{
			gotoxy(centre, 14);cout << "注册失败...";gotoxy(centre, 16);
			system("pause");system("cls");welcome_ui();
		}
		gotoxy(centre - 8, 14);cout << Border << "注册成功！！登录成功！ (3秒后跳转)" << Border << endl;
		//建立学生个人文件
		ofstream out_file;
		out_file.open(stucoursefile);
		out_file.close();
		Sleep(jump_time);
		system("cls");
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {
			gotoxy(centre, 11);cout << "请管理员先录入课程信息！";
			system("pause");
			return_menu();
		}
		stu_func(stucoursefile);
		break;
	}
	case 3:		//管理员登录
	{
		system("cls");
		string admin_id, admin_password;
		gotoxy(centre, 10);cout << "请输入管理员ID:	";
		cin >> admin_id;
		gotoxy(centre, 11);cout << "请输入管理员密码:	";
		encrypt(admin_password);
		while (admin_id != "Admin" || admin_password != "admin") {
			cout <<endl<<"\t\t"<< Border << "登录失败. ." << Border << endl << Border << "\t\t" << "	please try again	" << Border << endl;
			system("pause");
			system("cls");
			gotoxy(centre, 10);cout << "请输入管理员ID:	 ";
			cin >> admin_id;
			gotoxy(centre, 11);cout << "请输入管理员密码:	 ";
			cin >> admin_password;
		}
		gotoxy(centre - 8, 14);cout << Border << "登录成功! !(3秒后跳转)" << Border << endl << endl;
		Sleep(jump_time);
		system("cls");
		admin_func();
		break;
	}
	default:break;
	}
}

void welcome_ui() {
	int chosen_path = start_ui();
	log_in(chosen_path);
}

void return_menu()//3秒后跳回主屏幕
{
	Sleep(jump_time);
	system("cls");
	welcome_ui();
}

//以下为拓展功能
void gotoxy(int x, int y)//光标移动
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

void encrypt(string& password)//加密密码
{
	char ch=_getch();
	while (ch != '\n' && ch != '\r')
	{
		if (ch != '\b') {
			cout<<'*';
			password += ch;
		}
		else {
			cout << "\b \b";//可回删
			password.pop_back();
		}
		ch = _getch();
	}
}
