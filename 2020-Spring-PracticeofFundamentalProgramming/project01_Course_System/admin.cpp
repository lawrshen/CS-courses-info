#include<iostream>
#include<fstream>
#include<iomanip>
#include<Windows.h>
#include"admin.h"
#include"students.h"
#include"login.h"
using namespace std;
const string border(70, '=');
const string asterisk(33, '*');
const string cutting_line(86, '*');
const int title = 20;

void print_title()//打印管理员 界面
{
	gotoxy(title, 7);cout << border<<endl;
	gotoxy(title, 8);cout << "1.注销登录	2.录入课程信息	3.查看全部课程信息	   4.增加课程  ";
	gotoxy(title, 9);cout << "5.删除课程	6.修改课程	7.查看具体课程	8.指选课程  9.批量增加学生";
	gotoxy(title, 10);cout << border<<endl;
	gotoxy(title+20, 11);cout << "请选择所需要的功能: ";
}
void admin_func()
{
	system("color FA");
	print_title();
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
	if (choice == 2)
	{
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {	//只录入一次
			vector <course> course_information;
			storage(course_information, "defaultcourse.txt");
			current(course_information);
		}
	}
	while (choice > 0 && choice < 10){
		fstream file1;
		file1.open("currentcourse.txt", ios::in);
		if (!file1) {
			cout << "\t\t\t请管理员先导入课程!\n\t\t\t";system("pause");system("cls");admin_func();
		}
		switch (choice)
		{
		case 1:gotoxy(mid + 10, 12);cout << "Admin Log Out !!(3秒后返回初始页面)";return_menu();return;
		case 2:gotoxy(title + 20, 12);cout<<"信息已录入~"<<endl;break;
		case 3: {vector<course> schedule;storage(schedule, "currentcourse.txt");print_schedule(schedule);break;}
		case 4:add_course();break;
		case 5: {cout << "\t\t输入课程编号(选择超过一门课时请以“,”分割):";	
			vector<string> cous = input_cous();
			for (int i = 0;i < cous.size();++i)
				delete_course(cous[i]);break;}
		case 6:change_course();break;
		case 7:detail_course();break;
		case 8:add_Allstu();break;
		case 9: {cout << "\t请输入新建学生文件数:";
			int num; cin >> num; add_stus(num);
		}
		default:break;
		}
		cout << "\t\t\t";
		system("pause");
		system("cls");
		print_title();
		cin >> choice;
	}
	while (choice > 9 || choice < 1)		//超范围错误
	{
		cout << "\t\t\t操作失败，请正确输入1-9进行操作:";
		goto paths;
	}
}

void storage(vector <course>& course_information,string open_file)
{
	ifstream in_file(open_file, ios::in);
	if (!in_file)
	{
		cout << "Open storage failed" << endl;
		exit(-1);
	}
	course cou;
	while(!in_file.eof())
	{
		in_file >> cou.id >> cou.course_name >> cou.teacher_name >> cou.limit_number >> cou.select_number>>cou.course_type;
		if(cou.id!="")
			course_information.push_back(cou);
		in_file.get(); // 读取最后的回车符
		if (in_file.peek() == '\n') break;
	}
	in_file.close();
}

void current(vector <course>& course_information)
{
	ofstream out_file("currentcourse.txt", ios::out);
	if (out_file.fail())
	{
		cout << "Open current failed" << endl;
		exit(-1);
	}
	for (int i = 0;i < course_information.size();++i)
	{
		out_file << course_information[i].id<< "\t" << course_information[i].course_name
			<< "\t" << course_information[i].teacher_name<< "\t" << course_information[i].limit_number
			<< "\t" << course_information[i].select_number<< "\t" << course_information[i].course_type << endl;
	}
	out_file.close();
}

void print_schedule(vector <course> cou)
{
	if (cou.empty())
	{
		cout << "\n\t\t\t\t暂无课程\n";
		return;
	}
	cout <<endl<<"\t"<< cutting_line << endl;
	cout<< "\t" << setiosflags(ios::left) << setw(8) << "课程ID"<< setw(30) << "课程名称"
		<< setw(20) << "授课老师"<< setw(10) << "上限人数"
		<< setw(10) << "目前已选"<< setw(10) << "课程类型" << endl;
	for (int i = 0;i < cou.size();++i)
		cout << "\t"<< setiosflags(ios::left) << setw(8) << cou[i].id<< setw(30) << cou[i].course_name
			<< setw(20) << cou[i].teacher_name<< setw(10) << cou[i].limit_number
			<< setw(10) << cou[i].select_number<< setw(10) << cou[i].course_type << endl;
	cout << "\t" << cutting_line << endl ;
}

//课程	增删改	456
void add_course()
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	cout << "输入课程具体信息(课程名称，授课老师，上限人数，目前已选和课程类型):";
	string cou_infor[5];
	cin.ignore();
	getline(cin, cou_infor[0], ',');
	for (int i = 1;i < 4;++i)
		getline(cin, cou_infor[i], ',');
	getline(cin, cou_infor[4]);
	int pos = cou_infor[4].find(',');//容错
	if (pos != -1)
	{
		cout << "\n\t\t课程信息输入过多！错误\n\n";
		return;
	}
	for (int j = 0;j < course_information.size();++j)//遍历已存在课表，判断增添课程编号是否已存在
		if (cou_infor[0] == course_information[j].course_name)
		{
			cout << "\n\t\t该课程已存在\n\n";
			return;
		}
	try {//容错
		stoi(cou_infor[2]);
		stoi(cou_infor[3]);
	}
	catch (std::invalid_argument&) {
		cout << "\t\t\t课程上限人数和目前已选未正确输入数字\n"; return;
	}
	int limited_num = stoi(cou_infor[2]);
	int selected_num = stoi(cou_infor[3]);
	int last_num;
	if (!course_information.empty())
		last_num = stoi(course_information.back().id);
	else
		last_num = 0;
	last_num++;
	string new_id;		//添加新课的ID
	if(last_num<10)
		new_id= "00" + to_string(last_num);
	else if(last_num>9&&last_num<100)
		new_id= "0" + to_string(last_num);
	else
		new_id=to_string(last_num);
	course cou;
	cou.id = new_id; cou.course_name = cou_infor[0];cou.teacher_name = cou_infor[1];
	cou.limit_number = limited_num; cou.select_number = selected_num; cou.course_type = cou_infor[4];
	course_information.push_back(cou);
	current(course_information);
	cout << "\t\t添加成功！\n\n";
	print_schedule(course_information);
}

void delete_course(string courseId)
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	/*cout << "输入课程编号:";
	string courseId;
	cin >> courseId;*/
	vector<course>::iterator it;
	for(it=course_information.begin();it!=course_information.end();it++)
		if ((*it).id == courseId)
		{
			if ((*it).select_number > 0)
				cout << "\n\t\t删除课程 "<<courseId<<" 失败，目前已有学生选课。	\n";
			else {
				course_information.erase(it);
				cout << "\n\t\t课程 " << courseId << " 已删除\n";
			}
			current(course_information);
			return;
		}
	cout << "\n\t\t删除失败，不存在课程" << courseId << endl;
}

void change_course()
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	cout << "需要修改的课程ID:";
	string courseID;
	cin >> courseID;
	cout << "需要修改的具体字段(1.授课老师 2.容纳人数):";
	int num;
	while (!(cin >> num)) {	//防止输错字符导致错误
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "操作失败，请正确输入1或2进行操作:";
	}
	while (num > 2 || num < 1)		//超范围错误
	{
		cout << "操作失败，请正确输入1或2进行操作:";
		cin >> num;
	}
	for(int i=0;i<course_information.size();++i)
		if (course_information[i].id == courseID) {
			if (num == 1)
			{
				cout << "具体修改为:";
				string new_teacher;
				cin >> new_teacher;
				course_information[i].teacher_name = new_teacher;
				current(course_information);
				cout << "修改成功！\n\n";
			}
			else if (num == 2)
			{
				cout << "具体修改为:";
				int new_limit;
				cin >> new_limit;
				if (new_limit < course_information[i].select_number){
					cout << "修改失败!修改后容纳人数小于目前已选人数!\n\n";
					return;
				}
				else {
					course_information[i].limit_number = new_limit;
					current(course_information);
					cout << "修改成功！\n\n";
				}
			}
			return;
		}
	cout << "修改失败，该课程不存在...\n\n";
}
//查看具体课程	7
vector<string> stu_list(const string courseID)
{
	vector<string> cou_stuList;
	couMap course_map;
	courseID_map(course_map);
	vector<Student> stu;//所有学生信息
	read_stu_txt(stu);
	cout << "\n\t目前选择该门课的有:";
	int len1= stu.size();
	for (int i = 0;i < len1;++i)
	{
		string stuFile = stu[i].student_id + ".txt";//遍历每个学生
		if (stuFile == ".txt")
			break;
		vector<Student> stu_self;
		store_self(stu_self, stuFile);
		int len2 = stu_self.size();
		for (int j = 0;j < len2;++j)//找寻该学生是否选了这门课
			if (courseID == stu_self[j].self_course_id || courseID == course_map[stu_self[j].self_course_id])
			{
				cou_stuList.push_back(stu[i].student_id);//储存选课学生文件
				cout << stu[i].student_id << " ";//输出选课学生
				break;
			}
	}
	return cou_stuList;
}
vector<string> ass_list(const string courseID)
{
	vector<string> cou_assList;
	couMap course_map;
	courseID_map(course_map);
	vector<AssTable> ass_infor;//所有课的助教名单
	read_ass_txt(ass_infor);
	int len1= ass_infor.size();
	for (int i = 0;i < len1;++i)
		if (courseID == ass_infor[i].ass_course_id || courseID == course_map[ass_infor[i].ass_course_id])//匹配到该门课
		{
			cout << "\n\t目前该门课的助教有:";
			int len2 = ass_infor[i].student_ids.size();
			for (int j = 0;j <len2 ;++j)//储存并输出助教清单
			{
				cou_assList.push_back(ass_infor[i].student_ids[j]);
				cout << ass_infor[i].student_ids[j] << " ";
			}
			return cou_assList;
		}
	cout << "\n\t目前此门课暂无助教...\n";
	return cou_assList;
}

void detail_course()
{
	cout << "\n\t输入需要查看的ID或课程名称:";
	string courseID;
	cin >> courseID;
	vector<course> cou;
	storage(cou, "currentcourse.txt");
	int i;
	cout << "\t" << cutting_line << endl;
	for (i = 0;i < cou.size();++i)//打印该门课
		if (courseID == cou[i].id|| courseID == cou[i].course_name)
		{
			cout<< "\t" << setiosflags(ios::left) << setw(8) << cou[i].id<< setw(30) << cou[i].course_name<< setw(20) << cou[i].teacher_name
				<< setw(10) << cou[i].limit_number<< setw(10) << cou[i].select_number<< setw(10) << cou[i].course_type << endl;
			break;
		}
	cout << "\t" << cutting_line << endl<<"\n\t还希望进行以下哪些操作:1.查看选课学生名单 2.查看课程报名助教名单\n\t输入操作编号:";
	int num;
	while( (!(cin >> num))|| (num > 2 || num < 1)) {	//防止输错字符导致错误 超范围错误
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "操作失败，请正确输入1或2进行操作:";
	}
	if (num == 1) {
		if (cou[i].select_number == 0)
			cout << "\n\t暂无学生选此门课\n";
		else {
			vector<string> cou_stuList=stu_list(courseID);;
			cout << "\n\t共计" << cou[i].select_number << "名学生,距离选满课还差"<<cou[i].limit_number-cou[i].select_number<<"人!\n"
				<< "\n\n\t还希望进行以下哪些操作:1.录入此门课成绩\n\t输入操作编号:";
			int num;	cin >> num;
			if (num == 1) {
				int len = cou_stuList.size();
				for (int k = 0;k < len;++k)
				{
					cout <<"\t"<< cou_stuList[k] << " 的成绩:";
					string open_file = cou_stuList[k] + ".txt";
					add_score(open_file, courseID);//录入成绩
				}
			}
			cout << "\n\t无此操作";
		}
	}
	else if (num == 2) {
		vector<string> cou_assList= ass_list(courseID);//储存某门课助教名单并打印 
		if (!cou_assList.empty())//若这门课存在助教则可以相应查看学生名单
		{
			cout << "\n\n\t还希望进行以下哪些操作:1.查看选择助教的学生名单\n\t输入操作编号:";
			int j;	cin >> j;
			if (j == 1) {
				cout << "\n\t请输入以上助教id之一:";
				string ass_id;cin >> ass_id; 
				int len= cou_assList.size();
				for (int i = 0;i < len;i++)
					if (cou_assList[i] == ass_id)
					{
						ass_stulist(courseID, ass_id);
						return;
					}
				cout << "\n\t输入错误，该门课无这位助教";
			}
			cout << "\n\t无此操作";
		}
	}
}

//以下为拓展功能
void add_Allstu()
{
	cout << "\n\t\t\t\t\t请输入指选课程ID:";
	string course_id; cin >> course_id;
	vector<course> cou;
	storage(cou, "currentcourse.txt");//存课表
	for (int i = 0;i < cou.size();++i)
		if (cou[i].id == course_id)
		{
			if (cou[i].select_number == 0)//某门课选课为0时
			{
				vector<Student> stu;
				read_stu_txt(stu);//存所有学生
				if (cou[i].limit_number >= stu.size())//可以容纳人数时 
				{
					current(cou);//更新课表选课人数
					for (int j = 0;j < stu.size();++j)
					{
						string stu_file = stu[j].student_id + ".txt";
						select_self_course(stu_file, course_id);
						sort_course(stu_file);
					}
					cout << "\n\t\t\t\t\t\t指选成功";
				}
				else {
					cout << "\t\t\t\t学生人数超过课程容纳上限";return;
				}
			}
			else
				cout << "\t\t\t\t已有学生选此门课。。。";
			return;
		}
}

vector<string> input_cous()//输入一串课程
{
	vector<string> cous;
	string cou;
	cin >> cou;
	cous = Mysplit(cou, ",");
	return cous;
}

void add_score(const string stucoursefile,const string cou_id)//录入某门课分数
{
	string cou_score;
	cin >> cou_score;
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//个人课表
	couMap course_map;	courseID_map(course_map);//课程ID和课程名称匹配
	int len = stu_self.size();
	for (int i = 0;i < len;++i)
		if (cou_id == stu_self[i].self_course_id || cou_id == course_map[stu_self[i].self_course_id])
		{
			stu_self[i].score = cou_score;
			current_self(stu_self, stucoursefile);//更新学生个人文件
			return;
		}
}

void add_stus(int num)//批量建立一批num个学生账号，默认从学生文件最后一个人学号开始，若没有则191220001开始,默认密码123
{
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	int stuID;
	if (stu_information.empty())//因为学生文件为空，从191220001开始建立num个学生
		stuID = 191220000;
	else//从学生文件最后一个人学号开始建立num个学生
		stuID = stoi(stu_information.back().student_id);
	for (int j = 0;j < num;++j) {
		stuID++;
		ofstream out_file("student.txt", ios::app);
		if (out_file.fail())
		{
			cout << "Open stu_reg failed" << endl;
			exit(-1);
		}
		out_file << to_string(stuID) << ",123\n";//默认密码123
		out_file.close();
		string stucoursefile = to_string(stuID) + ".txt";
		ofstream outFile;
		outFile.open(stucoursefile);
		outFile.close();
	}
}
