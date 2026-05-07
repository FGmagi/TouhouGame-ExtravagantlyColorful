//============================================================
//	Selection_Window_ima 选项窗口（载入图片）
//============================================================
#include <stdafx.h>
#include <Window_Selection_ima.h>
#include <Sprite.h>
#include <Input.h>
//============================================================
//	构造函数
//============================================================
Window_Selection_ima::Window_Selection_ima(int ix, int iy,int iw,int ih, int iz) {
	active = true;
	x.assign(ix);
	y.assign(iy);
	z.assign(iz);
	dw.assign(iw);
	dh.assign(ih);
	tax.assign(0);
	tax_max = 0;
	opacity.assign(255);
}
//============================================================
//	析构函数
//============================================================
Window_Selection_ima::~Window_Selection_ima() {
	for (int i = 0; i < tax_max;i++) {
		delete s1[i];
		delete s2[i];
	}
}
//============================================================
//	选项刷新
//============================================================
int Window_Selection_ima::update() {
	int n = input_update();
	if (x.judge() || y.judge() || z.judge() || dw.judge() || dh.judge()){//位置
		for (int i = 0; i < tax_max; i++) {
			s1[i]->x = x + dw * i;
			s2[i]->x = s1[i]->x;
			s1[i]->y = y + dh * i;
			s2[i]->y = s1[i]->y;
			s1[i]->set_depth(z);
			s2[i]->set_depth(z);
		}
		x.assign();
		y.assign();
		z.assign();
		dw.assign();
		dh.assign();
	}
	if (opacity.judge()) {
		for (int i = 0; i < tax_max; i++) {
			s1[i]->opacity = opacity;
			s2[i]->opacity = opacity;
		}
		opacity.assign();
	}
	if (active && tax.judge()) {//是否更改选项
		s1[tax.recode]->active = true;
		s2[tax.recode]->active = false;
		s1[tax]->active = false;
		s2[tax]->active = true;
		tax.assign();
	}
	return n;
}
//============================================================
//	输入刷新
//============================================================
int Window_Selection_ima::input_update() {
	if (!active) return -1;//活动状态检测
	if (Input::repeat(R_UP)) {
		cycle_sub();
	}
	if (Input::repeat(R_DOWN)) {
		cycle_add();
	}
	if (Input::input(R_ENTER) || Input::input(R_SPACE)) {
		return tax;
	}
	return -1;
}
//============================================================
//	添加选项卡
//============================================================
void Window_Selection_ima::push(const char* p1, const char* p2) {
	int tx = x + dw * tax_max;
	int ty = y + dh * tax_max;
	Sprite* b1 = new Sprite(p1, tx, ty, z);
	Sprite* b2 = new Sprite(p2, tx, ty, z);
	b1->opacity = opacity;
	b2->opacity = opacity;
	if (tax.value == s1.size()) {
		b1->active = false;
		b2->active = true;
	} else {
		b1->active = true;
		b2->active = false;
	}
	s1.emplace_back(b1);
	s2.emplace_back(b2);
	tax_max++;
}