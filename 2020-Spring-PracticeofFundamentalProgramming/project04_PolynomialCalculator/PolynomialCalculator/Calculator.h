#ifndef CALCULATOR_H
#define CALCULATOR_H
#include<unordered_map>
#include<string>
#include<stack>
#include"Polynomial.h"

class Calculator
{
	public:
		Calculator();
		~Calculator();
		void createPoly();
		void addPoly(string name,Polynomial Poly);
		void MIXoperator();
		void GetInverse();
		void GetMod();
		void GetRoot();
		void PrintPoly();
	private:
		unordered_map<string,Polynomial> mapPoly;
		int postion=0;
		struct Myoperator{
			char op;
			double a,b;
			int pos;
		};
		struct Myname{
			string pname;
			int pos;
		};
		stack<Myoperator> stkop;
		stack<Myname> stkname;
		int Priority(const char op);
		int inPriority(const char op);
		bool read(string exp);
};

#endif
