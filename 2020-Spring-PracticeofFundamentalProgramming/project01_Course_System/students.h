#pragma once
#ifndef STUDENTS_H_
#define STUDENTS_H_
#include<string>
#include<vector>
#include<map>
#include<set>
#include"admin.h"
typedef std::map<std::string, std::string> couMap;

struct Student
{
	std::string student_id;		//ѧ��ID
	std::string student_password;//ѧ������
	std::string self_course_id;	//���˿γ̱��
	std::string ass_stu_id;		//��������
	std::string score;			//���˳ɼ�
};

struct AssTable
{
	std::string ass_course_id;		//�������̵Ŀγ̱��
	std::vector<string> student_ids;//�������̵�����ѧ��ID
};

void print_stu_title();
void stu_func(const string stucoursefile);	//ѧ����������
void read_stu_txt(vector<Student>& stu_information);//��ȡѧ�����ļ�
string stu_log(const string stu_id, const string stu_password);	//ѧ����¼
string stu_reg(const string stu_id, const string stu_password);	//ѧ��ע��
void courseID_map(couMap& course_map);//ƥ��γ̱�źͿγ�����
void self_schedule(const string stucoursefile);		//�鿴���˿α�
void current_self(const vector<Student> stu_self, const string stucoursefile);//����ѧ�������ļ�
void store_self(vector<Student>& stu_self, const string stucoursefile);//��ȡѧ�������ļ�
void course_num(const vector<Student> stu_self);//רҵ�η�רҵ��ѡ�����
//ѡ�˿�
void select_self_course(const string stucoursefile, const string courseID);//ѡ��
bool delete_self_course(const string stucoursefile, const string courseID);//�˿γɹ�����true
void ass_existOrNot(const string stucoursefile);//�˿����̸�֪
void sort_course(const string stucoursefile);//��������ĵ�
bool comp(const Student s1,const Student s2);//����Ƚ�
//����
void read_ass_txt(vector<AssTable>& ass_information);//��ȡ�����ļ�
void current_ass(vector<AssTable> ass_information);//���������ļ�
vector<string> Mysplit(string str, string pattern);//�ַ����ָ��
void apply_ass(const string stucoursefile);		//���ֱ�������
vector<string> self_ass(const string stucoursefile);//���˱������̵Ŀγ�
void select_ass(const string stucoursefile);		//ѡ���������

void change_password(string stu_id, string stu_password);//�޸�����
void current_stu(const vector<Student> stu);//����ѧ���ļ�
void ass_stulist(const string course_id, const string ass_id);//�鿴ĳһ�ſγ��ض����������µ�ѧ������
void self_score(const string stucoursefile);//�鿴���гɼ��Ŀ�Ŀ

#endif // !STUDENTS_H_
