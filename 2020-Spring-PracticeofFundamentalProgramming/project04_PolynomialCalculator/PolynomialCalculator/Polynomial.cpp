#include"Polynomial.h"
#define EPS 1e-6
#define M_PI 3.1415926

Polynomial::Polynomial(){
	this->factors.clear();
}

Polynomial::~Polynomial(){
	this->factors.clear();
}

Polynomial::Polynomial(double* cofficients,int len){
	for(int index=len-1;index>=0;--index)
		this->factors.push_back(cofficients[index]);
}

Polynomial Polynomial::operator +(const Polynomial& other)const{
	Polynomial result;
	int len1=this->factors.size(),len2=other.factors.size();
	int miner=min(len1,len2);
	for(int i=0;i<miner;++i)
		result.factors.push_back(this->factors[i]+other.factors[i]);
	if(len1>miner)
		for(int i=miner;i<len1;++i)
			result.factors.push_back(this->factors[i]);
	else
		for(int i=miner;i<len2;++i)
			result.factors.push_back(other.factors[i]);
	return result;
}

Polynomial Polynomial::operator -(const Polynomial& other)const{
	Polynomial result;
	int len1=this->factors.size(),len2=other.factors.size();
	int miner=min(len1,len2);
	for(int i=0;i<miner;++i)
		result.factors.push_back(this->factors[i]-other.factors[i]);
	if(len1>miner)
		for(int i=miner;i<len1;++i)
			result.factors.push_back(this->factors[i]);
	else
		for(int i=miner;i<len2;++i)
			result.factors.push_back(-other.factors[i]);
	return result;
}


Polynomial Polynomial::operator *(const Polynomial& other)const{
	int len1=this->factors.size(),len2=other.factors.size(),len=1;
	while(len<len1*2 || len<len2*2) len<<=1;//分治DFT处理多项式长度只能为2^n
	complex	*x1 = new complex[len],*x2 = new complex[len];
	for(int i=0;i<len1;++i) x1[i]=complex(this->factors[i],0);
	for(int i=len1;i<len;++i) x1[i]=complex(0,0);
	for(int i=0;i<len2;++i) x2[i]=complex(other.factors[i],0);
	for(int i=len2;i<len;++i) x2[i]=complex(0,0);
	FFT(x1,len,1);	FFT(x2,len,1);
	for(int i=0;i<len;++i)	x1[i]=x1[i]*x2[i];
	FFT(x1,len,-1);
	double* xx=new double[len];
	for(int i=0;i<len;++i)	xx[i]=x1[len-i-1].real;
	Polynomial result= Polynomial(xx,len);
	while(result.factors.size()>0&&fabs(result.factors.back())<EPS)
		result.factors.pop_back();
	return result;

}

Polynomial Polynomial::operator /(Polynomial& other){
	Polynomial F=this->preverse(),G=other.preverse(),result;
	int n=F.leng(),m=G.leng();
	Polynomial Ginv=G.inv(n-m+1);
	result=(F*Ginv).mod(n-m+1);
	int len=result.leng();
	while(len<n-m+1){
		result.factors.push_back(0);
		++len;
	}
	return result.preverse();
}

Polynomial Polynomial::operator %(Polynomial& other){
	Polynomial result = (*this)-(*this/other)*other;
	while(result.factors.size()>0&&fabs(result.factors.back())<EPS)
		result.factors.pop_back();
	return result;
}

Polynomial Polynomial::derivate(){
	Polynomial der;
	int len=this->factors.size();
	for(int i=1;i<len;++i)
		der.factors.push_back(i*this->factors[i]);
	return der;
}

Polynomial Polynomial::integrate(double a, double b){
	Polynomial antider;
	antider.factors.push_back(0);
	int len=this->factors.size();
	for(int i=0;i<len;++i){
		double factor=1.0*this->factors[i]/(i+1);
		antider.factors.push_back(factor);
	}
	double sum=0;
	for(int index=len;index>0;--index)
	{
		double an=1,bn=1;
		for(int j=0;j<index;++j){
			an*=a;bn*=b;
		}
		an*=antider.factors[index];
		bn*=antider.factors[index];
		sum+=(bn-an);
	}
	Polynomial result;
	result.factors.push_back(sum);
	return result;
}

ostream& operator << (ostream &out,const Polynomial &Poly)
{
	int len=Poly.factors.size();
	bool flag=0;
	for(int index=len-1;index>=0;--index)
	{
		if(Poly.factors[index]!=0)
		{
			if(flag&&Poly.factors[index]>0)
				out<<"+";
			if(Poly.factors[index]<0)
				out<<"-";
			if(!index||(fabs(Poly.factors[index]-1)>EPS&&fabs(Poly.factors[index]+1)>EPS))
				out<<fabs(Poly.factors[index]);
			if(index > 1)
				out<<"x^"<<index;
			else if(index==1)
				out<<"x";
			flag=1;
		}
	}
	if(!flag)out<<"0";
	return out;
}

void Polynomial::change(complex*y,int len)const{
	int* rev=new int[len]();
	for (int i = 0; i < len; ++i) {
		rev[i] = rev[i >> 1] >> 1;
		if (i & 1)   //如果最后一位是 1，则翻转成 len/2
			rev[i] |= len >> 1;
	}
	for (int i = 0; i < len; ++i) 
		if (i < rev[i])   //保证每对数只翻转一次
			swap(y[i], y[rev[i]]);
	return;
}

void Polynomial::FFT(complex*y,int len,const int fla)const{
	change(y,len);
	for (int h = 2; h <= len; h <<= 1) {                  // 模拟合并过程
		complex wn(cos(2 * M_PI / h), sin(fla * 2 * M_PI / h));  // 计算当前单位复根
		for (int j = 0; j < len; j += h) {
			complex w(1, 0);  // 计算当前单位复根
			for (int k = j; k < j + h / 2; ++k) {
				complex u = y[k],t = w * y[k + h / 2];
				y[k] = u + t;  // 这就是吧两部分分治的结果加起来
				y[k + h / 2] = u - t;
				// 后半个 “step” 中的ω一定和 “前半个” 中的成相反数
				//“红圈”上的点转一整圈“转回来”，转半圈正好转成相反数
				// 一个数相反数的平方与这个数自身的平方相等
				w = w * wn;
			}
		}
	}
	if (fla == -1) 
		for (int i = 0; i < len; ++i) 
			y[i].real /= len;
}

int Polynomial::leng(){
	return this->factors.size();
}

Polynomial Polynomial::inv(int len){//递归求逆
	if(len==1){
		double t[1]={1.0/this->factors[0]};
		Polynomial res=Polynomial(t,1);
		return res;
	}
	Polynomial temp=inv(ceil(1.0*len/2));//cout<<temp<<endl;
	double co[1]={2};
	Polynomial two=Polynomial(co,1),result=temp*(two-(*this)*temp);
	return result.mod(len);
}

Polynomial Polynomial::mod(int len)
{
	double* fina = new double[len];
	for(int i=0;i<len;++i)
		fina[len-i-1]=this->factors[i];
	return Polynomial(fina,len);
}

Polynomial Polynomial::preverse(){
	Polynomial rev=*this;
	reverse(rev.factors.begin(),rev.factors.end());
	return rev;
}

double Polynomial::value(double x){
	double y=0;
	int len=this->factors.size();
	for(int i=len-1;i>=0;--i)
		y=y*x+this->factors[i];
	return y;
}
