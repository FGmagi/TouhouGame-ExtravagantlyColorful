//============================================================
//	前置数据定义
//============================================================
#pragma once
#include <any>
#include <string>
#include <memory>
class Actor;
class Bitmap;
class Base {
public:
	//初始化
	Base();
	//释放
	~Base();
};
extern std::any VK_variables[1000];
typedef unsigned char uc;
typedef long long LL;
#define WIDTH 1280
#define HEIGHT 960
#define S_Ptr std::shared_ptr
#define U_Ptr std::unique_ptr
#define ZH_GROUND(name)	"Graphics/Back_Ground/"##name".png"//设置背景
#define ZH_TITLE(name)	"Graphics/Title/"##name".png"//设置背景
#define ZH_CHARACTER(name)	"Graphics/Character/"##name".png"//设置背景
#define ZH_Bullet(name)	"Graphics/Bullet/"##name".png"//设置背景
#define LEFT_EDGE 29//弹幕窗口边界
#define RIGHT_EDGE 873
#define UP_EDGE 29
#define DOWN_EDGE 930
#define ACTOR_Z 40//各种深度坐标
#define ACTOR_GLORY_Z 80
#define MONSTERS_Z 50
#define MONSTERS_CIRCLE_Z 20
#define EN_Bullet_Z 51
#define MY_Bullet_Z 30
#define Mini_Animation_Z 85//动画的sprite位置
#define Item_Z 45
#define PI 3.1415926535f
template <typename T, typename ...U>
T* MAKE(U&&... arg) {
	return new T(std::forward<U>(arg)...);
}

struct vec2 {
	float x, y;
	vec2();
	vec2(float, float);
	void operator+=(vec2);
	void operator-=(vec2);
};
struct ivec2 {
	int x, y;
};

class constants {
public:
	bool F2_open = true;;
	int BGM_volume;
	int SE_volume;
	S_Ptr<Bitmap> image_bullet;
	S_Ptr<Bitmap> image_bullet2;
	S_Ptr<Bitmap> image_item;
	S_Ptr<Bitmap> image_enemy;
	S_Ptr<Bitmap> image_boom_effect;

	S_Ptr<Doc> enemy_data;
	S_Ptr<Doc> bullet_data;
};
extern constants consts;