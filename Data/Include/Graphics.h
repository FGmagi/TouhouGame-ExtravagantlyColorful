//============================================================
//	重要宏定义
//============================================================
#pragma once
#include <Base.h>
#include <Color.h>
#include <Rect.h>
//============================================================
//	Graphics 画面显示与管理（接口）
//============================================================
struct Light;
struct Graphics1;
class Graphics{
public:
	//特效显示矩形位置
	static void rect(Rect);
	//设置灯光效果（未实装）
	static void light(Light&,int);
	//设置画面色调，颜色为rgba
	static void color(Color4);
	static void color();
	//设置画面hsl色调
	static void hsl(HSL);
	//刷新画面(times 为次数)
	static void update(int times = 1);
	//清空画面
	static void clear();
	//获取当前时间（单位帧）
	static long code();
	static int& wait_time();
	friend class Base;
private:
	Graphics();
	~Graphics();
	static void draw1();
	static void draw2();
};