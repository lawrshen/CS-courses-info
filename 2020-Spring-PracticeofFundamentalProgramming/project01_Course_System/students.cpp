#include<iostream>
#include<fstream>
#include<iomanip>
#include<algorithm>
#include<Windows.h>
#include"students.h"
#include"admin.h"
#include"login.h"
using namespace std;
const string border(68, '=');
const string asterisk(33, '*');
const string cutting_line(90, '*');
const string cutting_line2(52, '=');
const string null_score = "暂无成绩";
const int MajorInf = 4;
const int MinorInf = 2;
const int TotalSup = 10;
const int title = 20;

void print_stu_title()//打印学生 界面
{
	gotoxy(title, 7);cout << border << endl;
	gotoxy(title, 8);cout << "1.注销登录		2.查看课程信息	3.选课	4.查看个人课表	5.退课";
	gotoxy(title, 9);cout << "6.举手报名助教	7.选择个人助教	8.查看个人成绩	9.修改密码";
	gotoxy(title, 10);cout << border << endl;
	gotoxy(title + 24, 11);cout << "请选择所需要的功能: ";
}

void stu_func(const string stucoursefile)
{
	system("color F3");
	ass_existOrNot(stucoursefile);//提示助教是否退课
	print_stu_title();
	int choice;
paths:
	while (!(cin >> choice)) {	//防止输错字符导致错误
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "\t\t\t操作失败，请正确输入1-9进行操作:";
	}
	while (choice > 0 && choice < 10)
	{
		switch (choice)
		{
		case 1:gotoxy(mid + 10, 12);cout << "Log Out !!(3秒后返回初始页面)" ;	return_menu();return;
		case 2: {vector<course> schedule;storage(schedule, "currentcourse.txt");print_schedule(schedule);break;}
		case 3: {cout << "\n\t\t\t输入课程ID: ";string courseID;cin >> courseID;	
			select_self_course(stucoursefile,courseID);sort_course(stucoursefile);
			cout << "\t\t\t" << asterisk << "\n\t\t\t   已添加到个人课表中!!	\n\t\t\t" << asterisk << endl;break;}
		case 4:self_schedule(stucoursefile);break;
		case 5: {cout << "输入课程ID或课程名称:";string courseID;cin >> courseID;
			if (delete_self_course(stucoursefile, courseID)) 
			{ 
				cout << "\n\t课程删除成功。目前所选的课程有:\n";		
				self_schedule(stucoursefile);
			}
			else
				cout << "\n\t删除失败，查无此课。。。\n";
			break;}
		case 6:apply_ass(stucoursefile);break;
		case 7:select_ass(stucoursefile);break;
		case 8:self_score(stucoursefile);break;
		case 9:return;
		default:break;
		}
		cout << "\t\t\t";
		system("pause");
		system("cls");
		print_stu_title();
		cin >> choice;
	}
	while (choice > 9 || choice < 1)		//超范围错误
	{
		cout << "\t\t\t操作失败，请正确输入1-9进行操作:";
		goto paths;
	}
}

void read_stu_txt(vector<Student>& stu_information)
{
	ifstream in_file("student.txt", ios::in);
	if (!in_file)
	{
		cout << "Open read_stu_txt failed" << endl;
		exit(-1);
	}
	Student st;
	while (!in_file.eof())
	{
		getline(in_file, st.student_id, ',' );
		getline(in_file, st.student_password);
		if (st.student_id != "")
			stu_information.push_back(st);
	}
	in_file.close();
}
//登录和注册
string stu_log(string stu_id, string stu_password)	//学生登录
{
	string stucoursefile;
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	int i;
	for (i = 0;i < stu_information.size();++i)
		if (stu_id == stu_information[i].student_id)
			break;
	if(i== stu_information.size()){
		gotoxy(mid + 8, 14);cout << "	查无此人...	(3秒后返回初始页面)";
		return_menu();
	}
	if (stu_password == stu_information[i].student_password) {
		stucoursefile = stu_id + ".txt";
		return stucoursefile;
	}
	else
	{
		gotoxy(mid + 8, 14);cout << "	密码错误...	(3秒后返回初始页面)";
		return_menu();
	}
}
string stu_reg(string stu_id, string stu_password)	//学生注册
{	
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	for (int i = 0;i < stu_information.size();++i)
		if (stu_id == stu_information[i].student_id)
		{
			gotoxy(mid + 8, 14);cout << "注册失败	该学生用户已存在(3秒后返回初始页面)";
			return_menu();
			exit(-1);
		}
	string stucoursefile;
	ofstream out_file("student.txt",ios::app);
	if (out_file.fail())
	{
		cout << "Open stu_reg failed" << endl;
		exit(-1);
	}
	out_file << stu_id << "," << stu_password << endl;
	out_file.close();
	stucoursefile = stu_id + ".txt";
	return stucoursefile;
}
//课程ID和课程名称匹配
void courseID_map(couMap& course_map)//把课程ID和课程名称匹配起来
{
	vector<course> cou;				
	storage(cou, "currentcourse.txt");
	for (int i = 0;i < cou.size();++i)
		course_map.insert(pair<string, string>(cou[i].id, cou[i].course_name));
}
//查看个人课表
void self_schedule(const string stucoursefile)		
{
	vector<Student> stu_self;		//存放学生个人信息
	store_self(stu_self, stucoursefile);
	vector<course> cou;				//比对课表信息
	storage(cou, "currentcourse.txt");
	cout <<"\t"<< cutting_line << endl<<"\t" << setiosflags(ios::left) <<setw(8)<<"课程ID" << setw(30) <<"课程名称"
		<< setw(20) << "授课老师" << setw(10) << "课程类型" << setw(20) << "个人助教"<<endl;
	for (int j = 0;j < stu_self.size();++j)
		for(int i=0;i<cou.size();++i)
			if (cou[i].id == stu_self[j].self_course_id)
			{
				cout<<"\t"<< setiosflags(ios::left) << setw(8)  << cou[i].id<< setw(30) << cou[i].course_name<< setw(20) << cou[i].teacher_name
					<< setw(10) << cou[i].course_type << setw(20) <<stu_self[j].ass_stu_id<< endl;
				break;
			}
	cout << "\t" << cutting_line << endl;
	vector<string> ass_cou = self_ass(stucoursefile);
	if (!ass_cou.empty()) {
		cout << "\t所报名助教的课程编号为: ";
		for (int k = 0;k < ass_cou.size();++k)//打印出担任助教的课程
			cout << ass_cou[k] << " ";
	}
	cout << endl;
	course_num(stu_self);
}
//选退课
void store_self(vector<Student>&stu_self, const string stucoursefile)
{
	ifstream in_file(stucoursefile, ios::in);
	if (!in_file)
	{
		cout << "Open store_self failed" << endl;
		exit(-1);
	}
	Student st;
	while (!in_file.eof())
	{
		in_file >> st.self_course_id >> st.ass_stu_id>>st.score;
		if (st.self_course_id != "")
			stu_self.push_back(st);
		in_file.get(); // 读取最后的回车符
		if (in_file.peek() == '\n') break;
	}
	in_file.close();
}
//更新学生个人文件
void current_self(const vector<Student> stu_self, const string stucoursefile) //03.31增加成绩一项
{
	ofstream out_file(stucoursefile, ios::out);
	if (out_file.fail())
	{
		cout << "Open current_self failed" << endl;
		exit(-1);
	}
	int len= stu_self.size();
	for (int i = 0;i < len;++i)
	{
		out_file << stu_self[i].self_course_id<<"\t"<<stu_self[i].ass_stu_id <<"\t"<<stu_self[i].score<< endl;
	}
	out_file.close();
}

void course_num(const vector<Student> stu_self)//某一个学生专业课/非专业课选课数目
{
	int major_num = 0, minor_num = 0;
	vector<course> cou;				//比对课表信息
	storage(cou, "currentcourse.txt");
	for (int j = 0;j < stu_self.size();++j)//遍历课程
		for (int i = 0;i < cou.size();++i)//对应课表
			if (cou[i].id == stu_self[j].self_course_id)//找到该名学生的第j门课是专业课还是非专业课
				if (cou[i].course_type == "专业课")
					major_num++;
				else
					minor_num++;
	int delta_major = MajorInf - major_num;
	int delta_minor = MinorInf - minor_num;
	if (delta_major > 0 && delta_minor > 0)
		cout << "\t根据学院要求:目前所选课程数不达标!仍差"<<delta_major<<"门专业课，以及"<<delta_minor<<"门非专业课!请及时选课!\n\n";
	else if (delta_major > 0 && delta_minor <= 0)
		cout << "\t根据学院要求:目前所选课程数不达标!仍差" << delta_major << "门专业课!请及时选课!\n\n";
	else if (delta_major <= 0 && delta_minor > 0)
		cout << "\t根据学院要求:目前所选课程数不达标!仍差"<<delta_minor << "门非专业课!请及时选课!\n\n";
}
//选课
void select_self_course(const string stucoursefile,const string courseID)//参数是选课学生和课程ID 03.31增加成绩一项
{
	ofstream out_file(stucoursefile, ios::app);
	if (out_file.fail())
	{
		cout << "Open select_self_course failed" << endl;
		exit(-1);
	}
	vector<Student>stu_self;
	store_self(stu_self, stucoursefile);
	if (stu_self.size() >= TotalSup)
	{
		cout << "\n\t\t\t课程已选满" << TotalSup << "门，无法再选课了。。\n";
		return;
	}
	for (int j = 0;j < stu_self.size();++j)		//判断课程是否已选过
		if (courseID == stu_self[j].self_course_id)
		{
			cout << "\n\t\t\t该课程已选	\n";
			out_file.close();
			stu_func(stucoursefile);
		}
	vector<course> self_schedule;
	storage(self_schedule,"currentcourse.txt");
	int i;
	for(i=0;i<self_schedule.size();++i)
		if (self_schedule[i].id==courseID)//关联课程信息文件
			if (self_schedule[i].select_number >= self_schedule[i].limit_number)
			{
				cout << "\n\t\t\t	选课失败	选课人数已满	\n";
				stu_func(stucoursefile);
			}
			else
			{
				self_schedule[i].select_number++;
				break;
			}
	if (i == self_schedule.size())
	{
		cout << "	查无此课...	\n";
		stu_func(stucoursefile);
	}//遍历一遍没找到，没有此课
	current(self_schedule);
	out_file << courseID<<"\tNull\t"<<null_score<<endl;		//默认添加助教为Null 分数为null_score
	out_file.close();
}
//退课
bool delete_self_course(const string stucoursefile, const string courseID)
{
	vector<Student> stu_self;
	store_self(stu_self, stucoursefile);
	couMap course_map;
	courseID_map(course_map);
	vector<Student>::iterator it;
	for (it = stu_self.begin();it != stu_self.end();it++)
	{
		if ( course_map[(*it).self_course_id] == courseID|| (*it).self_course_id == courseID )//成功退课对几个文件产生影响
		{
			//总课程文件相应选课人数--
			vector<course> schedule;
			storage(schedule, "currentcourse.txt");
			for (int i = 0;i < schedule.size();++i)
				if (schedule[i].id == courseID)//关联课程信息文件
				{
					schedule[i].select_number--;
					current(schedule);
					break;
				}
			//若该门课报名助教，删
			vector<AssTable> ass;
			read_ass_txt(ass);
			int pos = stucoursefile.find('.');
			string studentID = stucoursefile.substr(0, pos);//该名学生ID
			vector<AssTable>::iterator t1;
			vector<string>::iterator t2;
			for(t1=ass.begin();t1!=ass.end();++t1)
				if ((*t1).ass_course_id == courseID)
					for (t2 = (*t1).student_ids.begin();t2 != (*t1).student_ids.end();++t2)
						if((*t2)==studentID)//分情况改写助教文件
							if ((*t1).student_ids.size() == 1)
							{
								ass.erase(t1);current_ass(ass);goto out;
							}
							else{
								(*t1).student_ids.erase(t2);current_ass(ass);goto out;
							}
out:
			//删掉学生个人文件中这行信息
			stu_self.erase(it);
			current_self(stu_self, stucoursefile);
			return true;
		}
	}
	return false;
}
//助教退课警告
void ass_existOrNot(const string stucoursefile)
{
	couMap course_map;	courseID_map(course_map);//课程ID和课程名称匹配
	vector<Student> stu_self;
	store_self(stu_self, stucoursefile);
	int i, j;
	for (i = 0;i < stu_self.size();++i)//遍历所有选过助教的课
		if (stu_self[i].ass_stu_id != "Null")
		{
			string ass_file = stu_self[i].ass_stu_id + ".txt";
			vector<Student> ass_self;
			if (ass_file != ".txt")
				store_self(ass_self, ass_file);
			else
				return;
			for (j = 0;j < ass_self.size();++j)
				if (ass_self[j].self_course_id == stu_self[i].self_course_id)//助教的课表中还有这门课
					break;
			if (j == ass_self.size()) {
				gotoxy(title, 5);cout << "课程 " << course_map[stu_self[i].self_course_id] << " 的助教" << stu_self[i].ass_stu_id << " 同学 已退课";
				stu_self[i].ass_stu_id = "Null";
				current_self(stu_self, stucoursefile);
			}
		}
}
//个人课表排序
void sort_course(const string stucoursefile)
{
	vector<Student> stu_self;
	store_self(stu_self, stucoursefile);
	sort(stu_self.begin(), stu_self.end(), comp);
	current_self(stu_self,stucoursefile);
}
bool comp(const Student s1,const Student s2)
{
	return stoi(s1.self_course_id) < stoi(s2.self_course_id);
}
//助教
void apply_ass(const string stucoursefile)//举手报名助教
{
	cout << "输入对应课程ID或课程名称:";
	string courseID;	cin >> courseID;//输入课程
	couMap course_map;	courseID_map(course_map);//课程ID和课程名称匹配
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//个人课表
	if (stu_self.empty())
	{
		cout << "请先选课再报名助教\n";return;
	}
	vector<string> ass_cou = self_ass(stucoursefile);
	if (ass_cou.size() >= 2) {
		cout << "已报名两门助教，无法再次报名!\n";return;
	}
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//该名学生ID
	for (int i = 0;i < stu_self.size();++i)//遍历个人课表找到输入的课
		if (courseID == stu_self[i].self_course_id || courseID == course_map[stu_self[i].self_course_id])
		{
			vector<AssTable> ass_information;
			read_ass_txt(ass_information);
			for (int j = 0;j < ass_information.size();++j)
				if(courseID == ass_information[j].ass_course_id || courseID == course_map[ass_information[j].ass_course_id])//已有同学报名了助教
				{
					for(int k=0;k< ass_information[j].student_ids.size();++k)
						if (ass_information[j].student_ids[k] == studentID)
						{
							cout << "已报名过该门课助教！";return;
						}
					ass_information[j].student_ids.push_back(studentID);
					cout << "报名助教成功!\n";
					current_ass(ass_information);
					return;
				}
			AssTable ass;//第一个报名该门课助教
			ass.ass_course_id = stu_self[i].self_course_id;
			ass.student_ids.push_back(studentID);
			ass_information.push_back(ass);
			current_ass(ass_information);
			cout << "报名助教成功!\n";
			return;
		}
	cout << "未选这门课，报名失败!\n";
}

vector<string> self_ass(const string stucoursefile)//个人报名助教的课程
{
	vector<string> ass_cou;
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//该名学生ID
	int apply_ass_times = 1;
	vector<AssTable> ass_information;
	read_ass_txt(ass_information);
	for (int j = 0;j < ass_information.size();++j)
		for (int i = 0;i < ass_information[j].student_ids.size();++i)
			if (ass_information[j].student_ids[i] == studentID)
			{
				apply_ass_times++;
				ass_cou.push_back(ass_information[j].ass_course_id);
				break;
			}
	return ass_cou;
}
void select_ass(const string stucoursefile)		//选择个人助教
{
	cout << "输入对应课程ID或课程名称:";
	string courseID;	cin >> courseID;//输入课程
	couMap course_map;	courseID_map(course_map);//课程ID和课程名称匹配
	vector<AssTable> ass_infor;//所有课的助教名单
	read_ass_txt(ass_infor);
	cout << "目前该门课的助教有:";
	int i, j;
	for (i = 0;i < ass_infor.size();++i)
		if (courseID == ass_infor[i].ass_course_id || courseID == course_map[ass_infor[i].ass_course_id])//匹配到该门课
		{
			for (j = 0;j < ass_infor[i].student_ids.size();++j)//输出助教清单
				cout << ass_infor[i].student_ids[j] << " ";
			break;
		}
	if (i == ass_infor.size())
	{
		cout << "该门课暂无助教\n";
		return;
	}
	cout << "\n请在上述选择中，选择你的个人助教:";
	string self_ass;	cin >> self_ass;//输入选择
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//该名学生ID
	if (self_ass == studentID)
	{
		cout << "不能选择自己作为助教\n";
		return;
	}
	for (j = 0;j < ass_infor[i].student_ids.size();++j)//比对助教清单
		if (self_ass == ass_infor[i].student_ids[j])
			break;
	if (j == ass_infor[i].student_ids.size()) {
		cout << "这门课无该位助教....";
		return;
	}
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//个人课表
	for (int k = 0;k < stu_self.size();++k)//遍历个人课表找到输入的课
		if (courseID == stu_self[k].self_course_id || courseID == course_map[stu_self[k].self_course_id])
		{
			stu_self[k].ass_stu_id = self_ass;
			current_self(stu_self, stucoursefile);
			cout << "已完成个人助教选择!\n";
			return;
		}
	cout << "未选这门课，选择失败!\n";
}
//更新助教文件
void read_ass_txt(vector<AssTable>& ass_information)
{
	ifstream in_file("assistant.txt", ios::in);
	if (!in_file)
	{
		cout << "Open read_ass_txt failed" << endl;
		exit(-1);
	}
	while (!in_file.eof())
	{
		AssTable ass; string ids, id;
		getline(in_file, ass.ass_course_id, '\t');
		getline(in_file, ids, '\n');
		ass.student_ids = Mysplit(ids, ",");
		if (ass.ass_course_id != "")
			ass_information.push_back(ass);
	}
	in_file.close();
}

void current_ass(vector<AssTable> ass_information)
{
	ofstream out_file("assistant.txt", ios::out);
	if (out_file.fail())
	{
		cout << "Open current_ass failed" << endl;
		exit(-1);
	}
	int i, j;
	for (i = 0;i < ass_information.size();++i)
	{
		out_file << ass_information[i].ass_course_id << "\t";//写入课程编号
		for (j = 0;j < ass_information[i].student_ids.size()-1;++j)//写入学生ID
			out_file << ass_information[i].student_ids[j] << ',';
		out_file << ass_information[i].student_ids[j] << endl;
	}
	out_file.close();
}

//字符串分割函数
vector<string> Mysplit(string str, string pattern)
{
	int pos;
	vector<string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//以下为拓展功能
void change_password(string stu_id, string stu_password)
{
	string old_password,new_password;
	gotoxy(centre, 10);cout << "请输入旧密码:";encrypt(old_password);
	if (old_password == stu_password) {
		gotoxy(centre, 11);cout << "请输入新密码:";encrypt(new_password);
		//把新密码写进student.txt
		vector<Student> stu;
		read_stu_txt(stu);
		for (int i = 0;i < stu.size();++i)
			if (stu[i].student_id == stu_id) {
				stu[i].student_password = new_password;
				current_stu(stu);
				gotoxy(centre, 14);cout << "修改密码成功，请重新登录!\n\n\t\t";system("pause");
				gotoxy(centre, 16);system("cls");welcome_ui();
			}
	}
	gotoxy(centre, 11);cout << "原始密码错误";
	return_menu();
}

void current_stu(const vector<Student> stu)
{
	ofstream out_file("student.txt", ios::out);
	if (out_file.fail())
	{
		cout << "Open current_stu failed" << endl;
		exit(-1);
	}
	for (int i = 0;i < stu.size();++i)
		out_file << stu[i].student_id << "," << stu[i].student_password << endl;
	out_file.close();
}

void ass_stulist(const string course_id,const string ass_id)//查看某一门课程特定助教名单下的学生名单
{
	int stu_num = 0;
	vector<Student> stu;//所有学生信息
	read_stu_txt(stu);
	cout << "\n\t目前这门课选择该助教的学生名单: ";
	int len1 = stu.size();
	for (int i = 0;i < len1;++i)
	{
		string stuFile = stu[i].student_id + ".txt";//遍历每个学生
		if (stuFile == ".txt")
			break;
		vector<Student> stu_self;
		store_self(stu_self, stuFile);
		int len2= stu_self.size();
		for (int j = 0;j < len2; ++j)//找寻该学生是否选了这门课并且这门课助教是不是他
			if (course_id == stu_self[j].self_course_id && ass_id == stu_self[j].ass_stu_id)
			{	
				cout << stu[i].student_id<<" ";
				stu_num++;
				break;
			}
	}
	if (stu_num == 0)//没有学生选这名助教
	{
		cout << " 无学生选择\n";
	}
}

void self_score(const string stucoursefile)//查看已有成绩的科目
{
	couMap course_map;	courseID_map(course_map);//课程ID和课程名称匹配
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//个人课表
	if (stu_self.empty())
	{
		cout << "\t\t\t\t请先选课\n";return;
	}
	cout << "\t\t\t" << cutting_line2 << endl << "\t\t\t" << setiosflags(ios::left) << setw(12) << "课程ID" << setw(30) << "课程名称"
		<< setw(12) << "课程分数"<<endl;
	int len = stu_self.size();
	for (int i = 0;i < len;++i)
		if(stu_self[i].score!=null_score)
			cout << "\t\t\t" << setiosflags(ios::left) << setw(12) << stu_self[i].self_course_id
				<< setw(30) << course_map[stu_self[i].self_course_id] << setw(12) << stu_self[i].score << endl;
	cout << "\t\t\t" << cutting_line2 <<endl;
}
