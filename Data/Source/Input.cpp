//============================================================
//	Input 输入与输出
//============================================================
#include <stdafx.h>
#include <Input.h>
#include <thread>

struct KEY { 
	int value;	//持续帧数
	int status; //当前状态 0未按下，1单击，2按住但未长按，3长按，4弹起，5已拦截的单击
	bool done; //当前帧是否已处理
};
struct Input1 {
	bool active;// 是否监听
	KEY keys[256];//键盘数据
	std::list<char> key_list;//监听列表
	std::jthread thread;//线程
}static *ptr;
void update(Input1* ptr); //线程刷新
//============================================================
//	Input 初始化
//============================================================
Input::Input() {
	ptr = new Input1;
	ptr->active = true;
	for (int i = 0; i < 256; i++) {
		ptr->keys[i].value = 0;
		ptr->keys[i].status = 0;
		ptr->keys[i].done = false;
	}
	// 监听列表
	for (int i = 0; i < 128; i++) {
		ptr->key_list.push_back(char(i));
	}/*
	ptr->key_list = { R_UP, R_DOWN, R_LEFT, R_RIGHT, R_SHIFT,
		R_Z, R_X, R_C, R_ESCAPE,R_ENTER,R_SPACE };*/
	ptr->thread = std::jthread([=](std::stop_token st) {while (!st.stop_requested()) update(ptr); });
};
Input::~Input() {
	delete ptr;
}
//确定按键情况
void Input::clear() {
	auto t_active = ptr->active;
	ptr->active = false;
	std::list<char>& list = ptr->key_list;
	for (auto i = list.begin();i!=list.end();i++){
		KEY& k = ptr->keys[*i];
		ptr->keys[*i].done = false;
		switch (k.status) {
		case 0://未按下
			if (k.value) k.status = 1;
			break;
		case 1://单击
			k.status = (k.value) ? 2 : -1;
			break;
		case 2://按住但未长按
			if (k.value) {
				if (k.value >= 12)
					k.status = 3;
			} else {
				k.status = -1;
			}
			break;
		case 3://长按（12帧以上）
		case 4://长按且为4的倍数帧
			if (k.value) {
				k.status = ((k.value & 0X3) == 0X3) ? 4 : 3;
			} else {
				k.status = -1;
			}
			break;
		case 5://已拦截
			if (!k.value)
				k.status = 0;
			break;
		case -1://弹起
			k.status = (k.value) ? 1 : 0;
			break;
		}
	}
	ptr->active = t_active;
};
//设置监听列表
void Input::set(const char* c) {
	ptr->key_list.clear();
	for (char i = 0; c[i] != 0; i++) {
		ptr->key_list.push_back(i);
	}
};
//是否开启监听
void Input::active(bool n) {
	ptr->active = n;
}
//============================================================
//	Input 输入
//============================================================
//0未按下，1单击，2按住但未长按，3长按（12帧以上），4长按间断输出，5阻断，-1弹起
void Input::block(char c) {
	ptr->keys[c].status = 5;
};
bool Input::input(char c) {
	return (ptr->keys[c].status == 1);
};
bool Input::press(char c) {
	return (ptr->keys[c].status != 5 && ptr->keys[c].status > 0);
};
bool Input::repeat(char c) {
	return (input(c) || hold(c));
};
bool Input::repeat2(char c) {
	return (input(c) || hold(c));
};
bool Input::hold(char c) {
	return (ptr->keys[c].status == 4);
};
bool Input::bonunce(char c) {
	return (ptr->keys[c].status == -1);
};
//刷新 线程控制
void update(Input1* ptr) {
	if (ptr->active) {
		for (auto i = ptr->key_list.begin(); i != ptr->key_list.end(); i++) {
			if (ptr->keys[*i].done == false) {
				if (GetAsyncKeyState(*i) & 0x8000) {
					ptr->keys[*i].value++;
					ptr->keys[*i].done = true;
				} else {
					ptr->keys[*i].value = 0;
				}
			}
		}
	}
};
