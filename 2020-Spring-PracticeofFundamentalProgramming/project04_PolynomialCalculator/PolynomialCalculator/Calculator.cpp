#include"Calculator.h"
#include<iostream>
using namespace std;
#define EPS 1e-6

Calculator::Calculator(){
	this->mapPoly.clear();
}

Calculator::~Calculator(){
	this->mapPoly.clear();
}

void Calculator::createPoly()
{
	cout<<"多项式长度：";int len; cin>>len;
	cout<<"请输入\n";double* cofficients=new double[len];
	for(int i=0;i<len;++i)
		cin>>cofficients[i];
	cout<<"多项式名为:";	string name;	cin>>name;
	Polynomial Poly(cofficients,len);
	this->addPoly(name,Poly);
	cout<<"\033[35m输入成功，\033[0m";
}

void Calculator::addPoly(string name,Polynomial Poly){
	this->mapPoly[name]=Poly;
}

void Calculator::MIXoperator(){
	while(!stkop.empty())
		this->stkop.pop();
	while(!stkname.empty())
		this->stkname.pop();
	this->postion=0;
	cout<<"请输入表达式:";	string exp;	cin>>exp;
	if(!read(exp))
		cout<<"\033[35m输入错误,\033[0m";
}
void Calculator::GetInverse(){
	cout<<"多项式名为:";	string name;	cin>>name;
	Polynomial poly;
	if(this->mapPoly.find(name)==this->mapPoly.end())
		cout<<"\033[35m没有该多项式，";
	else
		poly=mapPoly[name];
	int len=poly.leng();
	Polynomial result=poly.inv(len);
	cout<<"逆元为:"<<name<<"^-1="<<result<<"\n\033[35m求逆成功，\033[0m";
}

void Calculator::GetMod(){
	cout<<"请输入表达式:";	string exp;	cin>>exp;
	Polynomial div,dived;
	int pos=exp.find('/');
	if(pos==string::npos)
		cout<<"\033[35m输入错误,\033[0m";
	else{
		div=mapPoly[exp.substr(0,pos)];
		dived=mapPoly[exp.substr(pos+1,exp.length())];
		Polynomial quotient=div/dived,remainder=div%dived;
		cout<<"商为:"<<quotient<<"余数为:"<<remainder<<"\n\033[35m计算成功，\033[0m";
	}
}
void Calculator::GetRoot(){
	cout<<"多项式名为:";	string name;	cin>>name;
	double x1=0,x2=0;
	Polynomial poly,der;
	if(this->mapPoly.find(name)==this->mapPoly.end())
		cout<<"\033[35m没有该多项式，";
	else{
		poly=mapPoly[name];
		der=poly.derivate();
	}
	do{
		x1=x2;
		double fx=poly.value(x1),fy=der.value(x1);
		x2=x1-fx/fy;
	}while(fabs(x2-x1)>=EPS);
	cout<<name<<"的存在实数根为:"<<x2<<"\n\033[35m计算成功，\033[0m";
}

void Calculator::PrintPoly(){
	cout<<"多项式名为:";
	string name;	
	cin>>name;
	if(this->mapPoly.find(name)==this->mapPoly.end())
		cout<<"\033[35m没有该多项式，";
	else
		cout<<name<<" = "<<this->mapPoly[name]<<"\n\033[35m输入成功，\033[0m";
}


int Calculator::Priority(const char op)
{
	switch(op){
		case '+':return 1;
		case '*':return 2;
		case '$':return 3;
		case '!':return 4;
		case '(':return 5;
		case ')':return -1;
		default:return 0;
	}
}

int Calculator::inPriority(const char op)
{
	switch(op){
		case '+':return 1;
		case '*':return 2;
		case '$':return 3;
		case '!':return 4;
		case '(':return -1;
		case ')':return 5;
		default:return 0;
	}
}

bool Calculator::read(string exp)
{
	int i=0,len=exp.length();
	string name;
	Myoperator oper;
	Myname polyname;
	while(i<=len){
		if(stkop.empty()&&stkname.size()==1&&i==len)
		{
			string result=stkname.top().pname;
			stkname.pop();
			cout<<exp<<"="<<mapPoly[result]<<"\n\033[35m输入成功，\033[0m";
			return true;
		}
		//以下	表达式入栈
		int j=0;		//读取多项式名称
		char temp[50]={0};
		while((exp[i]>='a'&&exp[i]<='z')||(exp[i]>='A'&&exp[i]<='Z'))
		{
			temp[j]=exp[i];
			++j;	++i;
			if(i==len)
				break;
		}
		if(temp[0]!='\0'){//多项式
			name=temp;
			polyname.pname=name;
			polyname.pos=postion;
			++postion;
			stkname.push(polyname);
		}
		else{//运算符		
			oper.op=exp[i];
			if(oper.op=='$'){
				double x,y;
				++i;
				if(exp[i]=='[')
					++i;
				else
					return false;
				int start=i;
				while(i<len){
					if(exp[i]==',')
						break;
					++i;
				}
				if(i==len)
					return false;
				else{
					x=stof(exp.substr(start,i));
					oper.a=x;
				}++i;
				start=i;
				while(i<len){
					if(exp[i]==']')
						break;
					++i;
				}
				if(i==len)
					return false;
				else{
					y=stof(exp.substr(start,i));
					oper.b=y;
				}
				
			}//$
			if(!stkop.empty()&& Priority(oper.op) < inPriority(stkop.top().op) ){//当前运算符优先级低
				if(stkop.top().op=='!'|| stkop.top().op=='$' && !stkname.empty()){
					Myname poly_name1=stkname.top();
					stkname.pop();
					Polynomial poly1=mapPoly[poly_name1.pname],presult;
					if(stkop.top().op=='!')
						if(stkop.top().pos > poly_name1.pos)
							presult=poly1.derivate();
						else
							return false;
					else if(stkop.top().op=='$')
						if(stkop.top().pos < poly_name1.pos)
							presult=poly1.integrate(oper.a,oper.b);
						else
							return false;
					poly_name1.pname+='1';
					addPoly(poly_name1.pname,presult);
					stkname.push(poly_name1);
					stkop.pop();
				}
				else if((stkop.top().op=='+'|| stkop.top().op=='*') && stkname.size()>1){
					Myname poly_name1=stkname.top();
					stkname.pop();
					Myname poly_name2=stkname.top();
					stkname.pop();
					Polynomial poly1=mapPoly[poly_name1.pname],poly2=mapPoly[poly_name2.pname],presult;
					if(stkop.top().op=='+')
						presult=poly1+poly2;
					else if(stkop.top().op=='*')
						presult=poly1*poly2;
					poly_name1.pname+='1';
					addPoly(poly_name1.pname,presult);
					stkname.push(poly_name1);
					stkop.pop();
				}
				else
					return false;
			}
			++i;
			oper.pos=postion;
			++postion;
			stkop.push(oper);
			if(stkop.top().op==')'){
				stkop.pop();
				if(stkop.top().op=='(')
					stkop.pop();
				else
					return false;
			}
		}//op
		while(i==len&&!stkop.empty()){
			if(stkop.top().op=='!'|| stkop.top().op=='$' && !stkname.empty()){
					Myname poly_name1=stkname.top();
					stkname.pop();
					Polynomial poly1=mapPoly[poly_name1.pname],presult;
					if(stkop.top().op=='!')
						if(stkop.top().pos > poly_name1.pos)
							presult=poly1.derivate();
						else
							return false;
					else if(stkop.top().op=='$')
						if(stkop.top().pos < poly_name1.pos)
							presult=poly1.integrate(oper.a,oper.b);
						else
							return false;
					poly_name1.pname+='1';
					addPoly(poly_name1.pname,presult);
					stkname.push(poly_name1);
					stkop.pop();
				}
				else if((stkop.top().op=='+'|| stkop.top().op=='*') && stkname.size()>1){
					Myname poly_name1=stkname.top();
					stkname.pop();
					Myname poly_name2=stkname.top();
					stkname.pop();
					Polynomial poly1=mapPoly[poly_name1.pname],poly2=mapPoly[poly_name2.pname],presult;
					if(stkop.top().op=='+')
						presult=poly1+poly2;
					else if(stkop.top().op=='*')
						presult=poly1*poly2;
					poly_name1.pname+='1';
					addPoly(poly_name1.pname,presult);
					stkname.push(poly_name1);
					stkop.pop();
				}
				else
					return false;
		}
		
	}
}
