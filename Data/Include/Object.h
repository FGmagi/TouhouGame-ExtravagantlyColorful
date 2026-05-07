//============================================================
//	所有位图对象 Object 基类
//============================================================
#pragma once
#include <map>
#include <list>
#include <Base.h>
class Object_port;
template <typename T> class Complex;

class Object {
public:
	Object(int depth = 0);
	virtual ~Object();
	void set_depth(int depth);
	int get_depth();
	static void rend();
	//设置idle为false后无法自动update，最好需依托其他Object，
	//若手动update，则渲染次序由程序员管理
	void set_idle(bool);
	bool get_idle() { return idle; };
protected:
	bool idle=false;
	int depth;
	bool idioctonia = false;//自杀函数
	virtual void update();
private:
	std::map<int, std::list<Object*>>::iterator map_note;//一级节点位置
	std::list<Object*>::iterator list_note;//二级节点位置
};
class Object_port {
	friend Base;
	Object_port();
	~Object_port();
};
//============================================================
//	功能性函数
//============================================================
//	获取某点世界坐标：screen_xy屏幕坐标；screen_size世界大小
inline float normal_x(int screen_x, int screen_size) {
	return (float(screen_x) / (float(screen_size / 2)) - 1.0f);
}
inline float normal_y(int screen_y, int screen_size) {
	return (float(screen_y) / (float(screen_size / 2)) + 1.0f);
}
//	获取四个顶点对应世界坐标，dst为数组[8]，xy为左上原点，wh为宽高，n为旋转复数的地址,orgin为原点复数地址
void to_Postion(float* dst, int x, int y, int w, int h, Complex<float>* n = NULL, Complex<float>* orgin = NULL);
//	获取在某纹理中矩形对应纹理坐标
//  xy为截取矩形左上角坐标，dst为数组[8]，wh为截取矩形长宽，src_wh为源图片长宽
void to_TexCoord(float* dst, int x, int y, int w, int h, int src_w, int src_h);
void to_TexCoord(float* dst);
//  dst为数组[4]，opacity为透明度
void to_Opacity(float* dst, unsigned char opacity);
// 托管Object，时间到了自动释放
void Object_Deposit(Object*,int);
