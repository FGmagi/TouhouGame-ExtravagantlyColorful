#pragma once
class Color4 {
public:
	unsigned char r, g, b, a;
	Color4();
	Color4& operator =(Color4);
	Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void clear();
	//实际输出ABGR
	int rgba();
	~Color4();
};
class HSL {
public:
	float h,s,l;
	HSL();
	HSL(float h, float s, float l);
	void set(float h, float s, float l);
	void clear();
	~HSL();
};
//根据ID获取颜色
Color4 get_color(int id);

typedef Color4 Color;