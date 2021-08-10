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
	std::string student_id;		//学生ID
	std::string student_password;//学生密码
	std::string self_course_id;	//个人课程编号
	std::string ass_stu_id;		//个人助教
	std::string score;			//个人成绩
};

struct AssTable
{
	std::string ass_course_id;		//报名助教的课程编号
	std::vector<string> student_ids;//报名助教的所有学生ID
};

void print_stu_title();
void stu_func(const string stucoursefile);	//学生操作界面
void read_stu_txt(vector<Student>& stu_information);//读取学生总文件
string stu_log(const string stu_id, const string stu_password);	//学生登录
string stu_reg(const string stu_id, const string stu_password);	//学生注册
void courseID_map(couMap& course_map);//匹配课程编号和课程名称
void self_schedule(const string stucoursefile);		//查看个人课表
void current_self(const vector<Student> stu_self, const string stucoursefile);//更新学生个人文件
void store_self(vector<Student>& stu_self, const string stucoursefile);//读取学生个人文件
void course_num(const vector<Student> stu_self);//专业课非专业课选课情况
//选退课
void select_self_course(const string stucoursefile, const string courseID);//选课
bool delete_self_course(const string stucoursefile, const string courseID);//退课成功返回true
void ass_existOrNot(const string stucoursefile);//退课助教告知
void sort_course(const string stucoursefile);//排序个人文档
bool comp(const Student s1,const Student s2);//排序比较
//助教
void read_ass_txt(vector<AssTable>& ass_information);//读取助教文件
void current_ass(vector<AssTable> ass_information);//更新助教文件
vector<string> Mysplit(string str, string pattern);//字符串分割函数
void apply_ass(const string stucoursefile);		//举手报名助教
vector<string> self_ass(const string stucoursefile);//个人报名助教的课程
void select_ass(const string stucoursefile);		//选择个人助教

void change_password(string stu_id, string stu_password);//修改密码
void current_stu(const vector<Student> stu);//更新学生文件
void ass_stulist(const string course_id, const string ass_id);//查看某一门课程特定助教名单下的学生名单
void self_score(const string stucoursefile);//查看已有成绩的科目

#endif // !STUDENTS_H_
