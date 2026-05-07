//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Object.h>
#include <Complex.h>
#include <Bitmap.h>
#include <memory>
#include <NUM.h>
#include <functional>
class Shader;
//============================================================
//	精灵超类
//============================================================
class Sprite : public Object {
public:
	//基本属性
	Bitmap bitmap;
	bool active;//活动标志
	NUM<int> x, y;//坐标
	NUM<unsigned char> opacity;
	typedef void (*Call_Back)(Sprite*);
	std::function<void()> call_back;

	// 根据str创建并载入位图，精灵位置(tx,ty,tz)
	Sprite(const char* str, int tx, int ty, int tz = 0);
	//根据创建 w*h 大小位图并载入，精灵位置(tx,ty,tz)
	Sprite(Rect&& pos, int tz = 0);
	//根据位图创建，精灵(tx,ty,tz)
	Sprite(Bitmap& bitmap, int tx, int ty, int tz = 0);
	Sprite(const Sprite&);
	Sprite& operator =(const Sprite&);
	~Sprite();
	//设置位图截取部分
	void src(Rect&& rect);
	//设置位图截取部分，mask[false]为蒙版（相当于框柱只显示src范围）
	void src(Rect&& rect, int mask);
	// 设置缩放率
	void zoom(float n); 
	void zoom();
	//缩放原点位置
	void orgin(float x, float y);
	// 设置旋转角度
	void angle(float ang);
	void angle();
	void clear();
	void shader(Shader&);
	virtual void update();
	const Rect get_src() { return lens; };
private:
	friend class Sprite_Map2;
	Rect lens;
	bool mask_;//是否为蒙版
	Complex<float> orgin_;//缩放旋转原点位置
	Complex<float> complex;
	void preinstall();//上传基础数据，预设VBO VAO
	void Position();
	bool judge();
	std::shared_ptr<Shader> shader_;
protected:
	VAO vao;
	VBO vbo;
	bool revise;//旋转，放缩，截取标志
};
