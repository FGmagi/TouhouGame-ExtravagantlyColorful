//============================================================
//	Canvas 画布
//============================================================
#pragma once
#include <OpenGL.h>

class Bitmap;
class Canvas {
public:
	//默认构造窗口大小的画布（画布属于危险操作，不用了解）
	Canvas();
	//从src处浅拷贝画布
	Canvas(const Canvas &src);
	//从src处浅拷贝画布
	Canvas &operator=(const Canvas &src);

	// 获取Bitmap 浅拷贝到dst
	void get_Bitmap(Bitmap& src);
	//获取画布宽度
	static int get_width();
	//获取画布高度
	static int get_height();
	//设置为当前画布（仅用于即时绘制）
	void Bind();
private:
	FBO fbo;
	Texture texture;
	//static int g_width, g_height;
};