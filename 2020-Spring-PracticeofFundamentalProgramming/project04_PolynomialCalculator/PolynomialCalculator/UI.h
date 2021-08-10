#ifndef UI_H
#define UI_H
#include"Calculator.h"

class UI{
	public:
		UI();
		~UI();
		void show();
	private:
		Calculator *cal;
		
		
};

#endif
