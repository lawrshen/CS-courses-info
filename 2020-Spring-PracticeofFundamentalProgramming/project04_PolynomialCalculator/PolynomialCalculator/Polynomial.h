#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include<string>
#include<vector>
#define _USE_MATH_DEFINES
#include<cmath>
#include<algorithm>
#include<iostream>
using namespace std;

class Polynomial{
public:
	Polynomial();
	~Polynomial();
	Polynomial(double* cofficients,int len);
	friend ostream &operator << (ostream &out, const Polynomial &Poly);
	Polynomial operator + (const Polynomial& other)const;
	Polynomial operator - (const Polynomial& other)const;
	Polynomial operator * (const Polynomial& other)const;
	Polynomial operator / (Polynomial& other);
	Polynomial operator % (Polynomial& other);
	Polynomial derivate();				//求导
	Polynomial integrate(double a, double b);	//积分
	Polynomial inv(int len);			//求逆
	double value(double x);				//求值
	int leng();					//最高次

private:
	vector<double> factors;				//系数
	Polynomial preverse();				//翻转
	Polynomial mod(int len);			//模x^n
	struct complex{
		double real,imag;
		complex() { real = 0;imag = 0; }
		complex(double x,double y){real=x; imag=y;}
		inline complex operator + (const complex b)const{return complex(real+b.real,imag+b.imag);}
		inline complex operator - (const complex b)const{return complex(real-b.real,imag-b.imag);}
		inline complex operator * (const complex b)const{return complex(real*b.real-imag*b.imag,real*b.imag+imag*b.real);}
	};
	void change(complex*y,int len)const;		//蝴蝶变换
	void FFT(complex*y,int len,const int fla)const; //快速傅里叶变换
};

#endif
