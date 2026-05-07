#pragma once
#include <Base.h>
#define ITEM_MAX 3000
class Sprite;
//============================================================
//	item  掉落
//============================================================
class Item {
public:
	static Item* Push(int id, float x, float y);
	Item();
	void del();
	void update();
	bool check_crash();
public:
	int id;//0:hp; 1:boom; 2:power; 3:point 4:big_power; 5:Full
	vec2 r;
	float fx, fy;
	int follow;//0不跟随，1吸引，2跟随
	//到达最高点，静止一段时间，后匀速运动开始
	int uniform;
	bool active;
	S_Ptr<Sprite> sprite;
};
class Item_port {
public:
	Item_port();
	~Item_port();
};