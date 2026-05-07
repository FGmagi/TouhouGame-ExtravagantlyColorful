#pragma once
#include <Base.h>
#include <cmath>
#include <vector>
#include <Sprite.h>
#include <functional>
class Sprite;
class Effect;
class Sprite_Map;
inline float distance(float a, float b) {
	return sqrt(a * a + b * b);
}

struct Angle {
	void operator()(vec2& src) {
		R = distance(src.x, src.y);
		sin_theta = src.y / R;
		cos_theta = src.x / R;
	};
	float sin_theta;
	float cos_theta;
	float R;
};

//敌机子弹
class Bullet {
public:
	struct Pool {
		std::vector<Bullet> Bullet_list;
		int index;
		int max;
	};
public:
	void set_kind(int);
	void ini();
	Bullet();
	void update();
	static Bullet* push(int id, float x, float y,float vx,float vy);
	static Bullet* push(int id, float x, float y);
	static Bullet* push(int id);
	void del();
	static Bullet& at(int i);
	static void clear(bool forcus = false);
	void check_crash();
public:
	//线量
	vec2 r = vec2{ 0,0 };
	vec2 v = vec2{ 0.0f,0.0f };
	vec2 a = vec2{ 0.0f,0.0f };
	vec2 offset = vec2{ 16.0f,16.0f };
	//其他
	int id;
	int times;
	bool active;
	bool has_grazed;
	int status = 0;
	int color = 0;//无蓝绿红白
	bool immortal = false;
	vec2 range;
	S_Ptr<Sprite> sprite;
	std::function<void(Bullet*)> call_back;
	int note;
	static S_Ptr<Pool> pool;
	static S_Ptr<Sprite_Map> sprite_map;
private:
	static Bullet* push();
};
class Bullet_port {
public:
	Bullet_port();
	~Bullet_port();
};
//专用临时小动画
struct Mini_Animation {
	Mini_Animation(int id, int color, int x, int y);
};