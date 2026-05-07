#pragma once
#include <OpenGL.h>
#include <Object.h>
#include <Color.h>
#include <string>
#include <NUM.h>

class Message:public Object {
public:
	class Message_Box {
	public:
		//[]内为Message内置默认参数，实际上单独构造时全部设为负数，即不改变
		// 文字框宽度wh[-1,-1]。负数表示无限制，依赖\n换行
		int width, height;
		//字体颜色[255,255,255,255]
		Color4 color;
		//文字框边距[0,0,0,0]
		int left, right, top, bottom;
		//间距[0.1] 行距[0.2]百分比计算
		float texsp, rowsp;
		//文字大小[24]
		int font_size;
		//斜体 瘦长[false,false]
		bool italic, slender;
		// 描边[0]，描边颜色[255,255,255,255]
		int bold;
		Color4 outlinecolor;
	};
public:
	//活动标志
	bool active;
	//文字框坐标
	NUM<int> x, y;
	// 文字框位置xy，深度z，默认字体大小24
	Message(int x, int y, int z = 0);
	// 文字str，位置xy，深度z
	Message(const char* str, int x, int y, int z = 0);
	Message(Message&) = delete;
	void operator = (Message&) = delete;
	~Message();

	// 载入文字
	std::string& operator*();
	void operator=(int);
	void operator=(char);
	void operator=(long);
	void operator=(const char*);
	void operator=(std::string&);
	void operator+=(int);
	void operator+=(char);
	void operator+=(long);
	void operator+=(const char*);
	void operator+=(std::string&);
	//获取相关参数，返回Message_Box指针，每帧更新数据都必须调用该函数
	Message_Box* operator ->();

	// 获取字数
	int get_leght();
	// 清空文字
	void clear();
	//读取文字
	void read_file(const char*);

protected:
	Message_Box* ptr;
	int leght;//字数

	// 绘制文字（离线）
	virtual void update();
	void produce();
	VAO vao;//文字框
	VBO vbo;//文字框
	bool revise;//修改标志
	std::string text;
};
