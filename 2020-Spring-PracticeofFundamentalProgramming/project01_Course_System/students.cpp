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
const string null_score = "���޳ɼ�";
const int MajorInf = 4;
const int MinorInf = 2;
const int TotalSup = 10;
const int title = 20;

void print_stu_title()//��ӡѧ�� ����
{
	gotoxy(title, 7);cout << border << endl;
	gotoxy(title, 8);cout << "1.ע����¼		2.�鿴�γ���Ϣ	3.ѡ��	4.�鿴���˿α�	5.�˿�";
	gotoxy(title, 9);cout << "6.���ֱ�������	7.ѡ���������	8.�鿴���˳ɼ�	9.�޸�����";
	gotoxy(title, 10);cout << border << endl;
	gotoxy(title + 24, 11);cout << "��ѡ������Ҫ�Ĺ���: ";
}

void stu_func(const string stucoursefile)
{
	system("color F3");
	ass_existOrNot(stucoursefile);//��ʾ�����Ƿ��˿�
	print_stu_title();
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
	while (choice > 0 && choice < 10)
	{
		switch (choice)
		{
		case 1:gotoxy(mid + 10, 12);cout << "Log Out !!(3��󷵻س�ʼҳ��)" ;	return_menu();return;
		case 2: {vector<course> schedule;storage(schedule, "currentcourse.txt");print_schedule(schedule);break;}
		case 3: {cout << "\n\t\t\t����γ�ID: ";string courseID;cin >> courseID;	
			select_self_course(stucoursefile,courseID);sort_course(stucoursefile);
			cout << "\t\t\t" << asterisk << "\n\t\t\t   ����ӵ����˿α���!!	\n\t\t\t" << asterisk << endl;break;}
		case 4:self_schedule(stucoursefile);break;
		case 5: {cout << "����γ�ID��γ�����:";string courseID;cin >> courseID;
			if (delete_self_course(stucoursefile, courseID)) 
			{ 
				cout << "\n\t�γ�ɾ���ɹ���Ŀǰ��ѡ�Ŀγ���:\n";		
				self_schedule(stucoursefile);
			}
			else
				cout << "\n\tɾ��ʧ�ܣ����޴˿Ρ�����\n";
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
	while (choice > 9 || choice < 1)		//����Χ����
	{
		cout << "\t\t\t����ʧ�ܣ�����ȷ����1-9���в���:";
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
//��¼��ע��
string stu_log(string stu_id, string stu_password)	//ѧ����¼
{
	string stucoursefile;
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	int i;
	for (i = 0;i < stu_information.size();++i)
		if (stu_id == stu_information[i].student_id)
			break;
	if(i== stu_information.size()){
		gotoxy(mid + 8, 14);cout << "	���޴���...	(3��󷵻س�ʼҳ��)";
		return_menu();
	}
	if (stu_password == stu_information[i].student_password) {
		stucoursefile = stu_id + ".txt";
		return stucoursefile;
	}
	else
	{
		gotoxy(mid + 8, 14);cout << "	�������...	(3��󷵻س�ʼҳ��)";
		return_menu();
	}
}
string stu_reg(string stu_id, string stu_password)	//ѧ��ע��
{	
	vector<Student> stu_information;
	read_stu_txt(stu_information);
	for (int i = 0;i < stu_information.size();++i)
		if (stu_id == stu_information[i].student_id)
		{
			gotoxy(mid + 8, 14);cout << "ע��ʧ��	��ѧ���û��Ѵ���(3��󷵻س�ʼҳ��)";
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
//�γ�ID�Ϳγ�����ƥ��
void courseID_map(couMap& course_map)//�ѿγ�ID�Ϳγ�����ƥ������
{
	vector<course> cou;				
	storage(cou, "currentcourse.txt");
	for (int i = 0;i < cou.size();++i)
		course_map.insert(pair<string, string>(cou[i].id, cou[i].course_name));
}
//�鿴���˿α�
void self_schedule(const string stucoursefile)		
{
	vector<Student> stu_self;		//���ѧ��������Ϣ
	store_self(stu_self, stucoursefile);
	vector<course> cou;				//�ȶԿα���Ϣ
	storage(cou, "currentcourse.txt");
	cout <<"\t"<< cutting_line << endl<<"\t" << setiosflags(ios::left) <<setw(8)<<"�γ�ID" << setw(30) <<"�γ�����"
		<< setw(20) << "�ڿ���ʦ" << setw(10) << "�γ�����" << setw(20) << "��������"<<endl;
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
		cout << "\t���������̵Ŀγ̱��Ϊ: ";
		for (int k = 0;k < ass_cou.size();++k)//��ӡ���������̵Ŀγ�
			cout << ass_cou[k] << " ";
	}
	cout << endl;
	course_num(stu_self);
}
//ѡ�˿�
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
		in_file.get(); // ��ȡ���Ļس���
		if (in_file.peek() == '\n') break;
	}
	in_file.close();
}
//����ѧ�������ļ�
void current_self(const vector<Student> stu_self, const string stucoursefile) //03.31���ӳɼ�һ��
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

void course_num(const vector<Student> stu_self)//ĳһ��ѧ��רҵ��/��רҵ��ѡ����Ŀ
{
	int major_num = 0, minor_num = 0;
	vector<course> cou;				//�ȶԿα���Ϣ
	storage(cou, "currentcourse.txt");
	for (int j = 0;j < stu_self.size();++j)//�����γ�
		for (int i = 0;i < cou.size();++i)//��Ӧ�α�
			if (cou[i].id == stu_self[j].self_course_id)//�ҵ�����ѧ���ĵ�j�ſ���רҵ�λ��Ƿ�רҵ��
				if (cou[i].course_type == "רҵ��")
					major_num++;
				else
					minor_num++;
	int delta_major = MajorInf - major_num;
	int delta_minor = MinorInf - minor_num;
	if (delta_major > 0 && delta_minor > 0)
		cout << "\t����ѧԺҪ��:Ŀǰ��ѡ�γ��������!�Բ�"<<delta_major<<"��רҵ�Σ��Լ�"<<delta_minor<<"�ŷ�רҵ��!�뼰ʱѡ��!\n\n";
	else if (delta_major > 0 && delta_minor <= 0)
		cout << "\t����ѧԺҪ��:Ŀǰ��ѡ�γ��������!�Բ�" << delta_major << "��רҵ��!�뼰ʱѡ��!\n\n";
	else if (delta_major <= 0 && delta_minor > 0)
		cout << "\t����ѧԺҪ��:Ŀǰ��ѡ�γ��������!�Բ�"<<delta_minor << "�ŷ�רҵ��!�뼰ʱѡ��!\n\n";
}
//ѡ��
void select_self_course(const string stucoursefile,const string courseID)//������ѡ��ѧ���Ϳγ�ID 03.31���ӳɼ�һ��
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
		cout << "\n\t\t\t�γ���ѡ��" << TotalSup << "�ţ��޷���ѡ���ˡ���\n";
		return;
	}
	for (int j = 0;j < stu_self.size();++j)		//�жϿγ��Ƿ���ѡ��
		if (courseID == stu_self[j].self_course_id)
		{
			cout << "\n\t\t\t�ÿγ���ѡ	\n";
			out_file.close();
			stu_func(stucoursefile);
		}
	vector<course> self_schedule;
	storage(self_schedule,"currentcourse.txt");
	int i;
	for(i=0;i<self_schedule.size();++i)
		if (self_schedule[i].id==courseID)//�����γ���Ϣ�ļ�
			if (self_schedule[i].select_number >= self_schedule[i].limit_number)
			{
				cout << "\n\t\t\t	ѡ��ʧ��	ѡ����������	\n";
				stu_func(stucoursefile);
			}
			else
			{
				self_schedule[i].select_number++;
				break;
			}
	if (i == self_schedule.size())
	{
		cout << "	���޴˿�...	\n";
		stu_func(stucoursefile);
	}//����һ��û�ҵ���û�д˿�
	current(self_schedule);
	out_file << courseID<<"\tNull\t"<<null_score<<endl;		//Ĭ���������ΪNull ����Ϊnull_score
	out_file.close();
}
//�˿�
bool delete_self_course(const string stucoursefile, const string courseID)
{
	vector<Student> stu_self;
	store_self(stu_self, stucoursefile);
	couMap course_map;
	courseID_map(course_map);
	vector<Student>::iterator it;
	for (it = stu_self.begin();it != stu_self.end();it++)
	{
		if ( course_map[(*it).self_course_id] == courseID|| (*it).self_course_id == courseID )//�ɹ��˿ζԼ����ļ�����Ӱ��
		{
			//�ܿγ��ļ���Ӧѡ������--
			vector<course> schedule;
			storage(schedule, "currentcourse.txt");
			for (int i = 0;i < schedule.size();++i)
				if (schedule[i].id == courseID)//�����γ���Ϣ�ļ�
				{
					schedule[i].select_number--;
					current(schedule);
					break;
				}
			//�����ſα������̣�ɾ
			vector<AssTable> ass;
			read_ass_txt(ass);
			int pos = stucoursefile.find('.');
			string studentID = stucoursefile.substr(0, pos);//����ѧ��ID
			vector<AssTable>::iterator t1;
			vector<string>::iterator t2;
			for(t1=ass.begin();t1!=ass.end();++t1)
				if ((*t1).ass_course_id == courseID)
					for (t2 = (*t1).student_ids.begin();t2 != (*t1).student_ids.end();++t2)
						if((*t2)==studentID)//�������д�����ļ�
							if ((*t1).student_ids.size() == 1)
							{
								ass.erase(t1);current_ass(ass);goto out;
							}
							else{
								(*t1).student_ids.erase(t2);current_ass(ass);goto out;
							}
out:
			//ɾ��ѧ�������ļ���������Ϣ
			stu_self.erase(it);
			current_self(stu_self, stucoursefile);
			return true;
		}
	}
	return false;
}
//�����˿ξ���
void ass_existOrNot(const string stucoursefile)
{
	couMap course_map;	courseID_map(course_map);//�γ�ID�Ϳγ�����ƥ��
	vector<Student> stu_self;
	store_self(stu_self, stucoursefile);
	int i, j;
	for (i = 0;i < stu_self.size();++i)//��������ѡ�����̵Ŀ�
		if (stu_self[i].ass_stu_id != "Null")
		{
			string ass_file = stu_self[i].ass_stu_id + ".txt";
			vector<Student> ass_self;
			if (ass_file != ".txt")
				store_self(ass_self, ass_file);
			else
				return;
			for (j = 0;j < ass_self.size();++j)
				if (ass_self[j].self_course_id == stu_self[i].self_course_id)//���̵Ŀα��л������ſ�
					break;
			if (j == ass_self.size()) {
				gotoxy(title, 5);cout << "�γ� " << course_map[stu_self[i].self_course_id] << " ������" << stu_self[i].ass_stu_id << " ͬѧ ���˿�";
				stu_self[i].ass_stu_id = "Null";
				current_self(stu_self, stucoursefile);
			}
		}
}
//���˿α�����
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
//����
void apply_ass(const string stucoursefile)//���ֱ�������
{
	cout << "�����Ӧ�γ�ID��γ�����:";
	string courseID;	cin >> courseID;//����γ�
	couMap course_map;	courseID_map(course_map);//�γ�ID�Ϳγ�����ƥ��
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//���˿α�
	if (stu_self.empty())
	{
		cout << "����ѡ���ٱ�������\n";return;
	}
	vector<string> ass_cou = self_ass(stucoursefile);
	if (ass_cou.size() >= 2) {
		cout << "�ѱ����������̣��޷��ٴα���!\n";return;
	}
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//����ѧ��ID
	for (int i = 0;i < stu_self.size();++i)//�������˿α��ҵ�����Ŀ�
		if (courseID == stu_self[i].self_course_id || courseID == course_map[stu_self[i].self_course_id])
		{
			vector<AssTable> ass_information;
			read_ass_txt(ass_information);
			for (int j = 0;j < ass_information.size();++j)
				if(courseID == ass_information[j].ass_course_id || courseID == course_map[ass_information[j].ass_course_id])//����ͬѧ����������
				{
					for(int k=0;k< ass_information[j].student_ids.size();++k)
						if (ass_information[j].student_ids[k] == studentID)
						{
							cout << "�ѱ��������ſ����̣�";return;
						}
					ass_information[j].student_ids.push_back(studentID);
					cout << "�������̳ɹ�!\n";
					current_ass(ass_information);
					return;
				}
			AssTable ass;//��һ���������ſ�����
			ass.ass_course_id = stu_self[i].self_course_id;
			ass.student_ids.push_back(studentID);
			ass_information.push_back(ass);
			current_ass(ass_information);
			cout << "�������̳ɹ�!\n";
			return;
		}
	cout << "δѡ���ſΣ�����ʧ��!\n";
}

vector<string> self_ass(const string stucoursefile)//���˱������̵Ŀγ�
{
	vector<string> ass_cou;
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//����ѧ��ID
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
void select_ass(const string stucoursefile)		//ѡ���������
{
	cout << "�����Ӧ�γ�ID��γ�����:";
	string courseID;	cin >> courseID;//����γ�
	couMap course_map;	courseID_map(course_map);//�γ�ID�Ϳγ�����ƥ��
	vector<AssTable> ass_infor;//���пε���������
	read_ass_txt(ass_infor);
	cout << "Ŀǰ���ſε�������:";
	int i, j;
	for (i = 0;i < ass_infor.size();++i)
		if (courseID == ass_infor[i].ass_course_id || courseID == course_map[ass_infor[i].ass_course_id])//ƥ�䵽���ſ�
		{
			for (j = 0;j < ass_infor[i].student_ids.size();++j)//��������嵥
				cout << ass_infor[i].student_ids[j] << " ";
			break;
		}
	if (i == ass_infor.size())
	{
		cout << "���ſ���������\n";
		return;
	}
	cout << "\n��������ѡ���У�ѡ����ĸ�������:";
	string self_ass;	cin >> self_ass;//����ѡ��
	int pos = stucoursefile.find('.');
	string studentID = stucoursefile.substr(0, pos);//����ѧ��ID
	if (self_ass == studentID)
	{
		cout << "����ѡ���Լ���Ϊ����\n";
		return;
	}
	for (j = 0;j < ass_infor[i].student_ids.size();++j)//�ȶ������嵥
		if (self_ass == ass_infor[i].student_ids[j])
			break;
	if (j == ass_infor[i].student_ids.size()) {
		cout << "���ſ��޸�λ����....";
		return;
	}
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//���˿α�
	for (int k = 0;k < stu_self.size();++k)//�������˿α��ҵ�����Ŀ�
		if (courseID == stu_self[k].self_course_id || courseID == course_map[stu_self[k].self_course_id])
		{
			stu_self[k].ass_stu_id = self_ass;
			current_self(stu_self, stucoursefile);
			cout << "����ɸ�������ѡ��!\n";
			return;
		}
	cout << "δѡ���ſΣ�ѡ��ʧ��!\n";
}
//���������ļ�
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
		out_file << ass_information[i].ass_course_id << "\t";//д��γ̱��
		for (j = 0;j < ass_information[i].student_ids.size()-1;++j)//д��ѧ��ID
			out_file << ass_information[i].student_ids[j] << ',';
		out_file << ass_information[i].student_ids[j] << endl;
	}
	out_file.close();
}

//�ַ����ָ��
vector<string> Mysplit(string str, string pattern)
{
	int pos;
	vector<string> result;
	str += pattern;//��չ�ַ����Է������
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

//����Ϊ��չ����
void change_password(string stu_id, string stu_password)
{
	string old_password,new_password;
	gotoxy(centre, 10);cout << "�����������:";encrypt(old_password);
	if (old_password == stu_password) {
		gotoxy(centre, 11);cout << "������������:";encrypt(new_password);
		//��������д��student.txt
		vector<Student> stu;
		read_stu_txt(stu);
		for (int i = 0;i < stu.size();++i)
			if (stu[i].student_id == stu_id) {
				stu[i].student_password = new_password;
				current_stu(stu);
				gotoxy(centre, 14);cout << "�޸�����ɹ��������µ�¼!\n\n\t\t";system("pause");
				gotoxy(centre, 16);system("cls");welcome_ui();
			}
	}
	gotoxy(centre, 11);cout << "ԭʼ�������";
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

void ass_stulist(const string course_id,const string ass_id)//�鿴ĳһ�ſγ��ض����������µ�ѧ������
{
	int stu_num = 0;
	vector<Student> stu;//����ѧ����Ϣ
	read_stu_txt(stu);
	cout << "\n\tĿǰ���ſ�ѡ������̵�ѧ������: ";
	int len1 = stu.size();
	for (int i = 0;i < len1;++i)
	{
		string stuFile = stu[i].student_id + ".txt";//����ÿ��ѧ��
		if (stuFile == ".txt")
			break;
		vector<Student> stu_self;
		store_self(stu_self, stuFile);
		int len2= stu_self.size();
		for (int j = 0;j < len2; ++j)//��Ѱ��ѧ���Ƿ�ѡ�����ſβ������ſ������ǲ�����
			if (course_id == stu_self[j].self_course_id && ass_id == stu_self[j].ass_stu_id)
			{	
				cout << stu[i].student_id<<" ";
				stu_num++;
				break;
			}
	}
	if (stu_num == 0)//û��ѧ��ѡ��������
	{
		cout << " ��ѧ��ѡ��\n";
	}
}

void self_score(const string stucoursefile)//�鿴���гɼ��Ŀ�Ŀ
{
	couMap course_map;	courseID_map(course_map);//�γ�ID�Ϳγ�����ƥ��
	vector<Student> stu_self;	store_self(stu_self, stucoursefile);//���˿α�
	if (stu_self.empty())
	{
		cout << "\t\t\t\t����ѡ��\n";return;
	}
	cout << "\t\t\t" << cutting_line2 << endl << "\t\t\t" << setiosflags(ios::left) << setw(12) << "�γ�ID" << setw(30) << "�γ�����"
		<< setw(12) << "�γ̷���"<<endl;
	int len = stu_self.size();
	for (int i = 0;i < len;++i)
		if(stu_self[i].score!=null_score)
			cout << "\t\t\t" << setiosflags(ios::left) << setw(12) << stu_self[i].self_course_id
				<< setw(30) << course_map[stu_self[i].self_course_id] << setw(12) << stu_self[i].score << endl;
	cout << "\t\t\t" << cutting_line2 <<endl;
}
