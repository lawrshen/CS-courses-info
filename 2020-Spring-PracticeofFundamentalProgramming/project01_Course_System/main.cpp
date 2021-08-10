#include"login.h"
#include<cstdlib>

int main()
{
	system("title 学生选课信息管理系统");
	system("mode con cols=110 lines=40");//窗口宽度高度
	welcome_ui();
	return 0;
}