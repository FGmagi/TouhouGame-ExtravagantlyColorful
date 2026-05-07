//============================================================
//	Selection_Window 选项窗口
//============================================================
#include <stdafx.h>
#include "Window_Selection.h"
#include <Message.h>
#include <Music.h>
#include <Input.h>
//============================================================
//	构造函数
//============================================================
Window_Selection::Window_Selection(int max) {
	active = true;
	tax_max = max;
	tax = 0;
	message = -1;
}
//============================================================
//	析构函数
//============================================================
Window_Selection::~Window_Selection() {

}
int Window_Selection::get_tax_max() {
	return tax_max;
}
int Window_Selection::get_tax() {
	return tax;
}
Window_Selection::box Window_Selection::set_tax(int i) {
	auto recode = tax;
	tax = i;
	return { tax,recode, -1 };
}
//============================================================
//	选项刷新
//============================================================
Window_Selection::box Window_Selection::update() {
 	int message = -1;
	int recode = tax;
	if (!active) {
		return { tax,recode, -1 };
	}
	if (Input::repeat(R_UP)) {
		(tax == 0) ? tax = tax_max - 1 : tax--;
	}
	if (Input::repeat(R_DOWN)) {
		(tax == tax_max - 1) ? tax = 0 : tax++;
	}
	if (INPUT_C) {
		message = tax;
	}
	return { tax,recode, message };
}