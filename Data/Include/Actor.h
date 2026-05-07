//============================================================
//	头文件声明
//============================================================
#pragma once
#include <Sprite.h>
#include <memory>
#include <Base.h>
class Sprite_Map;
class Actor_Bullet;
class Actor_Bullet_port;
#define ACTOR_W 64
#define ACTOR_H 96
#define FREQUENCY 6
#define ATK_SPACE 4
#define Bullet_MAX 500
#define CHU_SHI_TIMES 8
//============================================================
//	角色管理
//============================================================
class Actor {
public:
	Actor();
	~Actor();
	void update();
	void miss();
	S_Ptr<Sprite> sprite = std::make_shared<Sprite>(ZH_CHARACTER("actor"), 451, 820, ACTOR_Z);
	S_Ptr<Sprite> glory = std::make_shared<Sprite>(*consts.image_bullet, 451 - 32, 820 - 16, ACTOR_GLORY_Z);
	S_Ptr<Sprite> boom_effect[3];
	//全部子弹碰撞检测
	void Global_Actor_Bullet_Collision();
public:
	float x = 451.0f, y = 820.0f;//坐标
	vec2 recode_pos = {0.0f,0.0f};//用于二分检测
	vec2 range = { 3.0f ,3.0f };//判定范围（圆形 已废除）

	//速度
	float speed = 9.0f;
	float sp_fast = 8.0f;
	float sp_low = 4.0f;
	//贴图检测
	int fx = 0, fy = 0;
	//状态
	int slow = 0;
	bool atk = false;
	bool boom = false;
	int spell_danger = 30000;
	//其他数值
	bool get_spell = false;
	bool active = true;
	bool key_active = true;
	int immortal = 0;
	int crash_time = 0;//中弹时间
	int atk_times = 0;//发单间隔
private:
	void missile();
	void use_boom();
	Actor_Bullet_port* actor_bullet_port;
};

class Actor_Bullet {
public:
	Actor_Bullet();
	void ini();
	void update();
	void check_crash();
	void del();
public:
	bool active;
	bool follow;
	vec2 r;
	vec2 v;
	vec2 range;
	int danger;
	S_Ptr<Sprite> sprite;
	std::function<void(Actor_Bullet*)> call_back;
};

class Actor_Bullet_port {//单例，由Actor管理
public:
	Actor_Bullet_port();
	S_Ptr<Sprite_Map> sprite_map;
	std::vector<Actor_Bullet> actor_bullet_vec;
	Actor_Bullet* push();
	int index = 0;
};