#pragma once
#ifndef ADMIN_H_
#define ADMIN_H_
#include<vector>
using namespace std;

struct course
{
	std::string id;				//课程编号
	std::string course_name;	//课程名称
	std::string teacher_name;	//授课老师	
	int limit_number;			//容纳人数
	int select_number;			//目前已选
	std::string course_type;	//课程类型
};

void print_title();
void admin_func();		//管理员操作界面选择
void storage(vector <course>& course_information, string open_file);		//将文件中数据存入vector中
void current(vector <course>& course_information);	//	创建currentcourse.txt
void print_schedule(vector <course> cou);
//课程	增删改
void add_course();
void delete_course(string courseId);
void change_course();
//查看具体课程
vector<string> stu_list(const string courseID);	//	某门课学生名单
vector<string> ass_list(const string courseID);	//	返回某门课助教名单vector
void detail_course();

void add_Allstu();
vector<string> input_cous();
void add_score(const string stucoursefile, const string cou_id);//录入某门课分数
void add_stus(int num);//
#endif // !ADMIN_H_
