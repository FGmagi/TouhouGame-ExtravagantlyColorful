//============================================================
//	Bitmap 位图
//============================================================ 
#pragma once
#include <OpenGL.h>
#include <Rect.h>
#include <Color.h>

class Bitmap {
public:
	//载入图片，路径str
	Bitmap(const char* str);
	//设置图片，宽高wh
	Bitmap(int w, int h);
	//浅拷贝图片src
	Bitmap(const Bitmap& src);
	//浅拷贝图片src
	Bitmap& operator=(const Bitmap& src);

	//深拷贝图片src
	void clone(Bitmap& src);
	//从src中裁剪Rect到位置(dst_x,dst_y)处
	void blt(Bitmap& src,Rect rect, int dst_x, int dst_y);
	//将Rect填满Color，透明度为a
	void fill_rect(Color4 color, Rect rect);
	//更改图片hsl（即时）
	void hsl(HSL);

	//清空图片
	void clear();
	//截图
	void screenshot();
	//返回图片宽度
	int get_width();
	//返回图片高度
	int get_height();
	//返回图片Texture（不用了解）
	void Bind();
protected:
	int width;
	int height;
	void load_image(const char*);
	Texture texture;
};