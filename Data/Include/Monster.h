//============================================================
//	头文件引用
//============================================================
#pragma once
#include<Base.h>
#include<memory>
#include<list>
#include<functional>
class Sprite;
class Monster {
public:
	Monster(int id);
	//自行update
	void update();
	void set_kind(int id);
public:
	int id;
	bool active = true;
	int times = 0;
	bool boss = false;
	bool in_spell = false;

	vec2 r = { 0.0f,0.0f };
	vec2 v = { 0.0f,0.0f };
	vec2 a = { 0.0f,0.0f };
	vec2 range = { 32.0f ,32.0f};//判定范围（方形）

	std::function<void(Monster*)> call_back;
	//贴图检测
	int hp = 1000;//生命
	int mark = 0;//分数
	int color = 0;//纹理位置
	bool immortal = false;
	int fx = 0;
	S_Ptr<Sprite> circle;//光环位置
	S_Ptr<Sprite> sprite;
	std::list<int> fall_items;
};