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
	gotoxy(mid+8, 8);cout << "��ӭʹ��ѧ��ѡ����Ϣ����ϵͳ";
	gotoxy(mid,10);	cout << LINE ;
	gotoxy(mid, 11);cout << " 1.ѧ����¼	 2.ѧ��ע��	3.����Ա��¼" ;
	gotoxy(mid, 12);cout << LINE ;
	gotoxy(mid+8, 14);cout << "������1��2��3���в���:";
	int i,line=15;
	while( (!(cin >> i))|| (i > 3 || i < 1) ){	//��ֹ����ַ����´��� ����Χ����
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		gotoxy(mid, ++line);cout << "����ʧ�ܣ�����ȷ����1��2��3���в���:";
	}
	return i;
}

void log_in(int chosen_path)
{
	ofstream file1;file1.open("student.txt",ios::app);file1.close();
	ofstream file2;file2.open("assistant.txt",ios::app);file2.close();
	switch (chosen_path)
	{
	case 1:		//ѧ����¼
	{
		system("cls");
		string stu_id, stu_password;
		gotoxy(centre, 10);cout << "������ѧ��ID: ";cin >> stu_id;
		gotoxy(centre, 11);cout << "������ѧ����������: ";encrypt(stu_password);
		string stucoursefile = stu_log(stu_id, stu_password);
		gotoxy(centre - 8, 14);cout << Border << "��¼�ɹ��� ��(3�����ת)" << Border << endl << endl;
		Sleep(jump_time);
		system("cls");
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {
			gotoxy(centre, 11);cout << "�����Ա��¼��γ���Ϣ��";
			system("pause");
			return_menu();
		}
		stu_func(stucoursefile);
		system("cls");
		change_password(stu_id, stu_password);
		break;
	}
	case 2:		//ѧ��ע��
	{
		system("cls");
		string stu_id, stu_password,check, stucoursefile;
		gotoxy(centre, 10);cout << "������ѧ��ID����ע��: ";	cin >> stu_id;
		gotoxy(centre, 11);cout << "������ѧ�������������ע��: ";	cin>>stu_password;
		gotoxy(centre, 12);cout << "��������ѧ����������ȷ��: ";	cin >> check;
		if(check==stu_password)
			stucoursefile = stu_reg(stu_id, stu_password);
		else
		{
			gotoxy(centre, 14);cout << "ע��ʧ��...";gotoxy(centre, 16);
			system("pause");system("cls");welcome_ui();
		}
		gotoxy(centre - 8, 14);cout << Border << "ע��ɹ�������¼�ɹ��� (3�����ת)" << Border << endl;
		//����ѧ�������ļ�
		ofstream out_file;
		out_file.open(stucoursefile);
		out_file.close();
		Sleep(jump_time);
		system("cls");
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {
			gotoxy(centre, 11);cout << "�����Ա��¼��γ���Ϣ��";
			system("pause");
			return_menu();
		}
		stu_func(stucoursefile);
		break;
	}
	case 3:		//����Ա��¼
	{
		system("cls");
		string admin_id, admin_password;
		gotoxy(centre, 10);cout << "���������ԱID:	";
		cin >> admin_id;
		gotoxy(centre, 11);cout << "���������Ա����:	";
		encrypt(admin_password);
		while (admin_id != "Admin" || admin_password != "admin") {
			cout <<endl<<"\t\t"<< Border << "��¼ʧ��. ." << Border << endl << Border << "\t\t" << "	please try again	" << Border << endl;
			system("pause");
			system("cls");
			gotoxy(centre, 10);cout << "���������ԱID:	 ";
			cin >> admin_id;
			gotoxy(centre, 11);cout << "���������Ա����:	 ";
			cin >> admin_password;
		}
		gotoxy(centre - 8, 14);cout << Border << "��¼�ɹ�! !(3�����ת)" << Border << endl << endl;
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

void return_menu()//3�����������Ļ
{
	Sleep(jump_time);
	system("cls");
	welcome_ui();
}

//����Ϊ��չ����
void gotoxy(int x, int y)//����ƶ�
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

void encrypt(string& password)//��������
{
	char ch=_getch();
	while (ch != '\n' && ch != '\r')
	{
		if (ch != '\b') {
			cout<<'*';
			password += ch;
		}
		else {
			cout << "\b \b";//�ɻ�ɾ
			password.pop_back();
		}
		ch = _getch();
	}
}
