//============================================================
//	Window 程序窗口
//============================================================
#pragma once
#include <Base.h>
#include <windows.h>
struct Window1;
void set_wh(int w,int h);
class Window {
public:
	//重设窗口大小（[-1]全屏 [0]1280*960 [1]960*720 [2]640*480）
	static void resize(int mode = 1);
	//交换缓冲
	static void swap();
	//获取句柄
	static HWND hwnd();
	//窗口是否要关闭
	static bool should_close();
	//获取窗口大小
	static int get_width();
	static int get_height();
	//是否全屏模式
	static bool is_fullscreen();
	friend class Base;
	friend void set_wh(int w,int h);
private:
	Window(unsigned width, unsigned height);
	~Window();
	static bool setGL();
};