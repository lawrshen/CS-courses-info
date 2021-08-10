//func_cmp.cpp

#include<iostream>
using namespace std;
#include<fstream>
#include"command.h"

int cmp(const string file1,const string file2)
{
	ifstream in_file1(file1,ios::in);	
	ifstream in_file2(file2,ios::in);
	//情况1 作为参数的file1或者file2实际上不存在
	if(!in_file1)
	{
		cout<<file1<<"：没有那个文件或目录t\n";
		return -1;
	}	
	if(!in_file2)
	{
		cout<<file2<<"：没有那个文件或目录t\n";
		return -1;
	}
	string content1,content2;
	int line=1,byte=1;
	while(true)
	{
		//情况2 相互比较的两个文件完全一样
		if(in_file1.eof()&&in_file2.eof())
			break;
		//情况3 有差别
		getline(in_file1,content1,'\n');
		getline(in_file2,content2,'\n');
		if(content1==content2){
			++line;
			int len=content1.size();
			for(int i=0;i<len;++i)
				byte+=sizeof(content1[i]);
			byte++;//回车符
		}
		else{
			if(content1.compare(content2)<0)//file1比较少
			{
				int j,len1=content1.size();
				for(j=0;j<len1;++j){
					if(content1[j]!=content2[j])
						break;
					else
						byte+=sizeof(content1[j]);
				}
				if(j==len1){
					cout<<"cmp: "<<file1<<" 已结束\n";
					break;
				}	
			}
			else if(content1.compare(content2)>0)//file2比较少
			{
				int i,len2=content2.size();
				for(i=0;i<len2;++i){
					if(content1[i]!=content2[i])
						break;
					else
						byte+=sizeof(content2[i]);
				}
				if(i==len2){
					cout<<"cmp: "<<file2<<" 已结束\n";
					break;
				}
			}
			cout<<file1<<" "<<file2<<" 不同: "<<"第 "
				<<byte<<" 字节,第 "<<line<<" 行\n";
			break;
		}
	}
	in_file1.close();	
	in_file2.close();	
    	return 0;
}

