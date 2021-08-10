#include"UI.h"
//#include<string>
#include<iostream>
using namespace std;

UI::UI(){
	cal = new Calculator();
}

UI::~UI(){
	delete cal;
}

void UI::show()
{
	cout<<"\033[35mwelcome to polynomial calculator\033[0m\n";	char ch;
	do{
		cout<<"\033[37m1.输入 2.混合运算 3.求逆元 4.除法/取模运算 5.求根 6.查看\033[0m\n请选择: ";
		int choice; 
		while(!(cin>>choice)){
			if(cin.fail()){
				cin.clear();
				cin.get();
			}
			cout<<"\033[35m选择失败，请重试(退出请输入-1):\033[0m";
		}
		if(choice==-1)break;
		switch(choice){
			case 1:this->cal->createPoly();break;
			case 2:this->cal->MIXoperator();break;
			case 3:this->cal->GetInverse();break;
			case 4:this->cal->GetMod();break;
			case 5:this->cal->GetRoot();break;
			case 6:this->cal->PrintPoly();break;
			default:cout<<"\033[35m无该选项,\033[0m"; break;
		}
		cout<<"\033[35m是否继续输入(y/n):\033[0m"; cin>>ch;
	}while(ch!='n');
}


