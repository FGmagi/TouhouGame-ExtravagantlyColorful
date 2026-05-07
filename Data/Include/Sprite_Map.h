//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Object.h>
#include <Rect.h>
#include <vector>
#include <Sprite.h>
#include <NUM.h>
#include <utility>
//============================================================
//	精灵集合
//  轻量级的集合，实际上是对同类型对象合并管理
//  隶属于Object，仅刷新Sprite，添加后Sprite的idle将设为flase
//  但单个对象其他属性不改变，避免无用虚函数寻址
//  不同于Sprite_Map2（重量级），该类仅保留指针，不管理对象
//  所以请程序员自行管理原Sprite
//============================================================
class Sprite_Map :public Object {
public:
	Sprite_Map(int depth = 0) :Object(depth) {};
	~Sprite_Map() {};
	void update();
	std::vector<Sprite*> map;
	void push(Sprite*);
};

//============================================================
//	精灵集合2
//  重量级合集，会自行管理精灵，且利用数组存储所有顶点数据
//  合集中 {depth,bitmap} 坐标将覆盖原有精灵属性
//  合集为浅拷贝，利用Sprite底层计数实现全托管
//  注意，直接修改Sprite属性后需手动通过Ask()函数重置GPU数据
//  Ask无参则全部重置，其效率为：迭代器O(1); Sprite&O(n); Ask O(n)高
// （修改该集合的 Bitmap不用）
//============================================================
class Sprite_Map2 :public Object {
public:
	class iterator {
	public:
		void operator ++();
		Sprite& operator *();
		Sprite* operator ->();
	private:
		int index;
		Sprite_Map2* p;
		iterator(int, Sprite_Map2*);
		friend class Sprite_Map2;
	};
public:
	Sprite_Map2(Bitmap bitmap = Bitmap(0,0),int depth = 0);
	~Sprite_Map2();
	void update();
	iterator&& begin();
	iterator&& end();
	void Ask();
	void Ask(Sprite&);
	void Ask(iterator&);
	//返回添加的迭代器，添加后在同一帧内该Sprite更新可以不用Ask(it)
	iterator&& push(Sprite);
	struct box {
		Sprite sp;
		bool rebuild = true;
	};
	std::vector<std::pair<Sprite,bool>> map;
	std::vector<float> vertices;// 17N
	Bitmap bitmap;
private:
	VAO vao;
	VBO vbo;
	int revise = 0;//0不更新，1更新GPU，2更新全部数据
	void Position(Sprite_Map2& p, int i);
	Sprite_Map2(Sprite_Map2&) = delete;
	void operator=(Sprite_Map2&) = delete;
};
