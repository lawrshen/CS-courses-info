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

void print_title()//��ӡ����Ա ����
{
	gotoxy(title, 7);cout << border<<endl;
	gotoxy(title, 8);cout << "1.ע����¼	2.¼��γ���Ϣ	3.�鿴ȫ���γ���Ϣ	   4.���ӿγ�  ";
	gotoxy(title, 9);cout << "5.ɾ���γ�	6.�޸Ŀγ�	7.�鿴����γ�	8.ָѡ�γ�  9.��������ѧ��";
	gotoxy(title, 10);cout << border<<endl;
	gotoxy(title+20, 11);cout << "��ѡ������Ҫ�Ĺ���: ";
}
void admin_func()
{
	system("color FA");
	print_title();
	int choice;
paths:
	while (!(cin >> choice)) {	//��ֹ����ַ����´���
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "\t\t\t����ʧ�ܣ�����ȷ����1-9���в���:";
	}
	if (choice == 2)
	{
		fstream file;
		file.open("currentcourse.txt", ios::in);
		if (!file) {	//ֻ¼��һ��
			vector <course> course_information;
			storage(course_information, "defaultcourse.txt");
			current(course_information);
		}
	}
	while (choice > 0 && choice < 10){
		fstream file1;
		file1.open("currentcourse.txt", ios::in);
		if (!file1) {
			cout << "\t\t\t�����Ա�ȵ���γ�!\n\t\t\t";system("pause");system("cls");admin_func();
		}
		switch (choice)
		{
		case 1:gotoxy(mid + 10, 12);cout << "Admin Log Out !!(3��󷵻س�ʼҳ��)";return_menu();return;
		case 2:gotoxy(title + 20, 12);cout<<"��Ϣ��¼��~"<<endl;break;
		case 3: {vector<course> schedule;storage(schedule, "currentcourse.txt");print_schedule(schedule);break;}
		case 4:add_course();break;
		case 5: {cout << "\t\t����γ̱��(ѡ�񳬹�һ�ſ�ʱ���ԡ�,���ָ�):";	
			vector<string> cous = input_cous();
			for (int i = 0;i < cous.size();++i)
				delete_course(cous[i]);break;}
		case 6:change_course();break;
		case 7:detail_course();break;
		case 8:add_Allstu();break;
		case 9: {cout << "\t�������½�ѧ���ļ���:";
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
	while (choice > 9 || choice < 1)		//����Χ����
	{
		cout << "\t\t\t����ʧ�ܣ�����ȷ����1-9���в���:";
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
		in_file.get(); // ��ȡ���Ļس���
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
		cout << "\n\t\t\t\t���޿γ�\n";
		return;
	}
	cout <<endl<<"\t"<< cutting_line << endl;
	cout<< "\t" << setiosflags(ios::left) << setw(8) << "�γ�ID"<< setw(30) << "�γ�����"
		<< setw(20) << "�ڿ���ʦ"<< setw(10) << "��������"
		<< setw(10) << "Ŀǰ��ѡ"<< setw(10) << "�γ�����" << endl;
	for (int i = 0;i < cou.size();++i)
		cout << "\t"<< setiosflags(ios::left) << setw(8) << cou[i].id<< setw(30) << cou[i].course_name
			<< setw(20) << cou[i].teacher_name<< setw(10) << cou[i].limit_number
			<< setw(10) << cou[i].select_number<< setw(10) << cou[i].course_type << endl;
	cout << "\t" << cutting_line << endl ;
}

//�γ�	��ɾ��	456
void add_course()
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	cout << "����γ̾�����Ϣ(�γ����ƣ��ڿ���ʦ������������Ŀǰ��ѡ�Ϳγ�����):";
	string cou_infor[5];
	cin.ignore();
	getline(cin, cou_infor[0], ',');
	for (int i = 1;i < 4;++i)
		getline(cin, cou_infor[i], ',');
	getline(cin, cou_infor[4]);
	int pos = cou_infor[4].find(',');//�ݴ�
	if (pos != -1)
	{
		cout << "\n\t\t�γ���Ϣ������࣡����\n\n";
		return;
	}
	for (int j = 0;j < course_information.size();++j)//�����Ѵ��ڿα��ж�����γ̱���Ƿ��Ѵ���
		if (cou_infor[0] == course_information[j].course_name)
		{
			cout << "\n\t\t�ÿγ��Ѵ���\n\n";
			return;
		}
	try {//�ݴ�
		stoi(cou_infor[2]);
		stoi(cou_infor[3]);
	}
	catch (std::invalid_argument&) {
		cout << "\t\t\t�γ�����������Ŀǰ��ѡδ��ȷ��������\n"; return;
	}
	int limited_num = stoi(cou_infor[2]);
	int selected_num = stoi(cou_infor[3]);
	int last_num;
	if (!course_information.empty())
		last_num = stoi(course_information.back().id);
	else
		last_num = 0;
	last_num++;
	string new_id;		//����¿ε�ID
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
	cout << "\t\t��ӳɹ���\n\n";
	print_schedule(course_information);
}

void delete_course(string courseId)
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	/*cout << "����γ̱��:";
	string courseId;
	cin >> courseId;*/
	vector<course>::iterator it;
	for(it=course_information.begin();it!=course_information.end();it++)
		if ((*it).id == courseId)
		{
			if ((*it).select_number > 0)
				cout << "\n\t\tɾ���γ� "<<courseId<<" ʧ�ܣ�Ŀǰ����ѧ��ѡ�Ρ�	\n";
			else {
				course_information.erase(it);
				cout << "\n\t\t�γ� " << courseId << " ��ɾ��\n";
			}
			current(course_information);
			return;
		}
	cout << "\n\t\tɾ��ʧ�ܣ������ڿγ�" << courseId << endl;
}

void change_course()
{
	vector <course> course_information;
	storage(course_information, "currentcourse.txt");
	cout << "��Ҫ�޸ĵĿγ�ID:";
	string courseID;
	cin >> courseID;
	cout << "��Ҫ�޸ĵľ����ֶ�(1.�ڿ���ʦ 2.��������):";
	int num;
	while (!(cin >> num)) {	//��ֹ����ַ����´���
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "����ʧ�ܣ�����ȷ����1��2���в���:";
	}
	while (num > 2 || num < 1)		//����Χ����
	{
		cout << "����ʧ�ܣ�����ȷ����1��2���в���:";
		cin >> num;
	}
	for(int i=0;i<course_information.size();++i)
		if (course_information[i].id == courseID) {
			if (num == 1)
			{
				cout << "�����޸�Ϊ:";
				string new_teacher;
				cin >> new_teacher;
				course_information[i].teacher_name = new_teacher;
				current(course_information);
				cout << "�޸ĳɹ���\n\n";
			}
			else if (num == 2)
			{
				cout << "�����޸�Ϊ:";
				int new_limit;
				cin >> new_limit;
				if (new_limit < course_information[i].select_number){
					cout << "�޸�ʧ��!�޸ĺ���������С��Ŀǰ��ѡ����!\n\n";
					return;
				}
				else {
					course_information[i].limit_number = new_limit;
					current(course_information);
					cout << "�޸ĳɹ���\n\n";
				}
			}
			return;
		}
	cout << "�޸�ʧ�ܣ��ÿγ̲�����...\n\n";
}
//�鿴����γ�	7
vector<string> stu_list(const string courseID)
{
	vector<string> cou_stuList;
	couMap course_map;
	courseID_map(course_map);
	vector<Student> stu;//����ѧ����Ϣ
	read_stu_txt(stu);
	cout << "\n\tĿǰѡ����ſε���:";
	int len1= stu.size();
	for (int i = 0;i < len1;++i)
	{
		string stuFile = stu[i].student_id + ".txt";//����ÿ��ѧ��
		if (stuFile == ".txt")
			break;
		vector<Student> stu_self;
		store_self(stu_self, stuFile);
		int len2 = stu_self.size();
		for (int j = 0;j < len2;++j)//��Ѱ��ѧ���Ƿ�ѡ�����ſ�
			if (courseID == stu_self[j].self_course_id || courseID == course_map[stu_self[j].self_course_id])
			{
				cou_stuList.push_back(stu[i].student_id);//����ѡ��ѧ���ļ�
				cout << stu[i].student_id << " ";//���ѡ��ѧ��
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
	vector<AssTable> ass_infor;//���пε���������
	read_ass_txt(ass_infor);
	int len1= ass_infor.size();
	for (int i = 0;i < len1;++i)
		if (courseID == ass_infor[i].ass_course_id || courseID == course_map[ass_infor[i].ass_course_id])//ƥ�䵽���ſ�
		{
			cout << "\n\tĿǰ���ſε�������:";
			int len2 = ass_infor[i].student_ids.size();
			for (int j = 0;j <len2 ;++j)//���沢��������嵥
			{
				cou_assList.push_back(ass_infor[i].student_ids[j]);
				cout << ass_infor[i].student_ids[j] << " ";
			}
			return cou_assList;
		}
	cout << "\n\tĿǰ���ſ���������...\n";
	return cou_assList;
}

void detail_course()
{
	cout << "\n\t������Ҫ�鿴��ID��γ�����:";
	string courseID;
	cin >> courseID;
	vector<course> cou;
	storage(cou, "currentcourse.txt");
	int i;
	cout << "\t" << cutting_line << endl;
	for (i = 0;i < cou.size();++i)//��ӡ���ſ�
		if (courseID == cou[i].id|| courseID == cou[i].course_name)
		{
			cout<< "\t" << setiosflags(ios::left) << setw(8) << cou[i].id<< setw(30) << cou[i].course_name<< setw(20) << cou[i].teacher_name
				<< setw(10) << cou[i].limit_number<< setw(10) << cou[i].select_number<< setw(10) << cou[i].course_type << endl;
			break;
		}
	cout << "\t" << cutting_line << endl<<"\n\t��ϣ������������Щ����:1.�鿴ѡ��ѧ������ 2.�鿴�γ̱�����������\n\t����������:";
	int num;
	while( (!(cin >> num))|| (num > 2 || num < 1)) {	//��ֹ����ַ����´��� ����Χ����
		if (cin.fail())
		{
			cin.clear();
			cin.get();
		}
		cout << "����ʧ�ܣ�����ȷ����1��2���в���:";
	}
	if (num == 1) {
		if (cou[i].select_number == 0)
			cout << "\n\t����ѧ��ѡ���ſ�\n";
		else {
			vector<string> cou_stuList=stu_list(courseID);;
			cout << "\n\t����" << cou[i].select_number << "��ѧ��,����ѡ���λ���"<<cou[i].limit_number-cou[i].select_number<<"��!\n"
				<< "\n\n\t��ϣ������������Щ����:1.¼����ſγɼ�\n\t����������:";
			int num;	cin >> num;
			if (num == 1) {
				int len = cou_stuList.size();
				for (int k = 0;k < len;++k)
				{
					cout <<"\t"<< cou_stuList[k] << " �ĳɼ�:";
					string open_file = cou_stuList[k] + ".txt";
					add_score(open_file, courseID);//¼��ɼ�
				}
			}
			cout << "\n\t�޴˲���";
		}
	}
	else if (num == 2) {
		vector<string> cou_assList= ass_list(courseID);//����ĳ�ſ�������������ӡ 
		if (!cou_assList.empty())//�����ſδ��������������Ӧ�鿴ѧ������
		{
			cout << "\n\n\t��ϣ������������Щ����:1.�鿴ѡ�����̵�ѧ������\n\t����������:";
			int j;	cin >> j;
			if (j == 1) {
				cout << "\n\t��������������id֮һ:";
				string ass_id;cin >> ass_id; 
				int len= cou_assList.size();
				for (int i = 0;i < len;i++)
					if (cou_assList[i] == ass_id)
					{
						ass_stulist(courseID, ass_id);
						return;
					}
				cout << "\n\t������󣬸��ſ�����λ����";
			}
			cout << "\n\t�޴˲���";
		}
	}
}

//����Ϊ��չ����
void add_Allstu()
{
	cout << "\n\t\t\t\t\t������ָѡ�γ�ID:";
	string course_id; cin >> course_id;
	vector<course> cou;
	storage(cou, "currentcourse.txt");//��α�
	for (int i = 0;i < cou.size();++i)
		if (cou[i].id == course_id)
		{
			if (cou[i].select_number == 0)//ĳ�ſ�ѡ��Ϊ0ʱ
			{
				vector<Student> stu;
				read_stu_txt(stu);//������ѧ��
				if (cou[i].limit_number >= stu.size())//������������ʱ 
				{
					current(cou);//���¿α�ѡ������
					for (int j = 0;j < stu.size();++j)
					{
						string stu_file = stu[j].student_id + ".txt";
						select_self_course(stu_file, course_id);
						sort_course(stu_file);
					}
					cout << "\n\t\t\t\t\t\tָѡ�ɹ�";
				}
				else {
					cout << "\t\t\t\tѧ�����������γ���������";return;
				}
			}
			else
				cout << "\t\t\t\t����ѧ��ѡ���ſΡ�����";
			return;
		}
}

vector<string> input_cous()//����һ���γ�
{
	vector<string> cous;
	string cou;
	cin >> cou;
	cous = Mysplit(cou, ",");
	return cous;
}

void add_score(const string stucoursefile,const string cou_id)//¼��ĳ�ſη���
{
	string cou_score;
	cin >> cou_score;
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//���˿α�
	couMap course_map;	courseID_map(course_map);//�γ�ID�Ϳγ�����ƥ��
	int len = stu_self.size();
	for (int i = 0;i < len;++i)
		if (cou_id == stu_self[i].self_course_id || cou_id == course_map[stu_self[i].self_course_id])
		{
			stu_self[i].score = cou_score;
			current_self(stu_self, stucoursefile);//����ѧ�������ļ�
			return;
		}
}

void add_stus(int num)//��������һ��num��ѧ���˺ţ�Ĭ�ϴ�ѧ���ļ����һ����ѧ�ſ�ʼ����û����191220001��ʼ,Ĭ������123
{
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	int stuID;
	if (stu_information.empty())//��Ϊѧ���ļ�Ϊ�գ���191220001��ʼ����num��ѧ��
		stuID = 191220000;
	else//��ѧ���ļ����һ����ѧ�ſ�ʼ����num��ѧ��
		stuID = stoi(stu_information.back().student_id);
	for (int j = 0;j < num;++j) {
		stuID++;
		ofstream out_file("student.txt", ios::app);
		if (out_file.fail())
		{
			cout << "Open stu_reg failed" << endl;
			exit(-1);
		}
		out_file << to_string(stuID) << ",123\n";//Ĭ������123
		out_file.close();
		string stucoursefile = to_string(stuID) + ".txt";
		ofstream outFile;
		outFile.open(stucoursefile);
		outFile.close();
	}
}
