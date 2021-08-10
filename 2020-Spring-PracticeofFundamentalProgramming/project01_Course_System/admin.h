#pragma once
#ifndef ADMIN_H_
#define ADMIN_H_
#include<vector>
using namespace std;

struct course
{
	std::string id;				//�γ̱��
	std::string course_name;	//�γ�����
	std::string teacher_name;	//�ڿ���ʦ	
	int limit_number;			//��������
	int select_number;			//Ŀǰ��ѡ
	std::string course_type;	//�γ�����
};

void print_title();
void admin_func();		//����Ա��������ѡ��
void storage(vector <course>& course_information, string open_file);		//���ļ������ݴ���vector��
void current(vector <course>& course_information);	//	����currentcourse.txt
void print_schedule(vector <course> cou);
//�γ�	��ɾ��
void add_course();
void delete_course(string courseId);
void change_course();
//�鿴����γ�
vector<string> stu_list(const string courseID);	//	ĳ�ſ�ѧ������
vector<string> ass_list(const string courseID);	//	����ĳ�ſ���������vector
void detail_course();

void add_Allstu();
vector<string> input_cous();
void add_score(const string stucoursefile, const string cou_id);//¼��ĳ�ſη���
void add_stus(int num);//
#endif // !ADMIN_H_
