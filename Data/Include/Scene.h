//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Base.h>
#include <memory>
//new某对象并返回指针
class Scene_port;
//===========================================================================================
// Scene 界面超类
//===========================================================================================
class Scene {
public:
	//主循环
	virtual void main() = 0;
	virtual ~Scene();
	//重设指针
	static void reset(Scene* p);
	static void reset();
	//将p2指向p1并释放内存，仅在Main循环中固定使用
	static void assign();
	//调用Scene的Main()
	static void login();
	//判断是否为空
	static bool empty();
	//判断当前界面指针是否与p类型相等
	static bool judge(Scene* p);
	//类型判断（类似judge，但为动态函数）
	bool operator==(Scene* p);
	friend class Scene_port;
	static S_Ptr<Scene> p1, p2;
};
class Scene_port {
	friend Base;
	Scene_port();
	~Scene_port();
};
