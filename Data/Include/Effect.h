//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Object.h>
#include <Color.h>
//============================================================
//	仿函数 在Effect完成后该对象会被释放，请勿额外手动delete
//============================================================
class Effect;
class Functor {
public:
	Functor();
	virtual ~Functor() {};
	virtual void update() = 0;
	//存储归属的Effect地址
	Effect* mul_effect;
private:
	Functor(Functor&) = delete;
	void operator=(Functor&) = delete;
	friend class Effect;
};
//============================================================
//	Effect 演出神器 通过仿函数或函数指针调用
//============================================================
class Effect :public Object {
public:
	//仿函数（带有智能指针）
	bool active;
	//仅可选设定深度
	Effect(int z = 0);
	virtual ~Effect();
	//传入仿函数，深度z
	Effect(Functor* functor, int z = 0);
	void reset(Functor* functor);
	Effect(void (*functor)(void), int z = 0);
	void reset(void (*function)(void));
	bool empty();
	Functor* get_functor();
protected:
	Effect(Effect&) = delete;
	void operator=(Effect&) = delete;
	Functor* functor;
	void (*function)();
	void update();
};

//============================================================
//	常用函数
//============================================================
//时间小于零为永久
void Effect_Froze(int last_times, Color4 color, int* rest = NULL);
void Effect_Set_Fade();//凝固画面
void Effect_Do_Fade(int last_times, int* rest = NULL);//画面渐变
