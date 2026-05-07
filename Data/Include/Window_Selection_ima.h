//============================================================
//	头文件引用
//============================================================
#pragma once
#include <vector>
#include <NUM.h>
#define ZH_PUSH(a,b) push("Graphics/Title/"##a".png", "Graphics/Title/"##b".png") 
//创造派生类
#define ZH_CREAT(name) \
	class name :public Window_Selection {\
		public:\
		name (int ix, int iy, int iw, int ih, int iz = 0) : Window_Selection(ix, iy, iw, ih, iz) {}; \
		int update();\
	};

class Sprite;
//============================================================
//	Window_Selection 选项窗口
//============================================================
class Window_Selection_ima {
public:
	NUM<int> tax;
	int tax_max;
	int active;

	NUM<int> x, y,z,dw,dh;
	NUM<unsigned char> opacity;
	std::vector<Sprite*> s1, s2;

	Window_Selection_ima(int, int, int, int, int iz=0);
	~Window_Selection_ima();
	virtual int update();
	int input_update();
	void push(const char*, const char*);
	//循环数据
	inline void cycle_add() {
		(tax.value == tax_max - 1) ? tax = 0 : tax++;
	};
	inline void cycle_sub() {
		(tax == 0) ? tax = tax_max - 1 : tax--;
	};
};
