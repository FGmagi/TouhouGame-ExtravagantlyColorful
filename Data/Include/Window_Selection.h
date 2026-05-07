//============================================================
//	头文件引用
//============================================================
#pragma once
#include <NUM.h>
//============================================================
//	Window_Selection 选项辅助窗口
//============================================================
class Window_Selection {
public:
	struct box {
		int tax;
		int recode;
		int message;
	};
	int active;

	//输入选项上限
	Window_Selection(int max);
	~Window_Selection();
	Window_Selection::box update();
	int get_tax();
	Window_Selection::box set_tax(int);
	int get_tax_max();
protected:
	int tax_max;
	int tax;
	int message;
};
