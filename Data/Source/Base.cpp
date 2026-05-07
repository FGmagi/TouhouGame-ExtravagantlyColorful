//============================================================
//	头文件引用
//============================================================
#include <stdafx.h>
#include <Window.h>
#include <Base.h>
#include <Canvas.h>
#include <Shader.h>
#include <Graphics.h>
#include <Object.h>
#include <OpenGL.h>
#include <Input.h>
#include <Font.h>
#include <Effect.h>
#include <Music.h>
#include <Scene.h>
#include <Bullet.h>
#include <Bitmap.h>
#include <Debug.h>

std::any VK_variables[1000];
Window* window;
Object_port* object_port;
Graphics* graphics;
Input* input;
Font* font;
Shader_port* shader_port;
Scene_port* scene_port;
constants consts;

Doc* read_json(const char* src);
//============================================================
//	手动单例模式
//============================================================
void overall() {//伟大的数组
	VK_variables[2] = true;
	consts.image_bullet.reset(new Bitmap(ZH_Bullet("bullet")));
	consts.image_bullet2.reset(new Bitmap(ZH_Bullet("bullet2")));
	consts.image_item.reset(new Bitmap(ZH_TITLE("item")));
	consts.image_enemy.reset(new Bitmap(ZH_CHARACTER("enemy")));
	consts.image_boom_effect.reset(new Bitmap("Graphics/Back_Ground/光幕.png"));

	std::fstream file("Data/game_data.dat",std::ios::in);
	if (file) {
		file >> consts.BGM_volume >> consts.SE_volume;
	} else {
		consts.BGM_volume = 100;
		consts.SE_volume = 100;
	}
	Music::BGM_Volume(consts.BGM_volume);
	Music::SE_Volume(consts.SE_volume);
	consts.enemy_data.reset(read_json("Data/Scrpit/敌机数据.json"));
	consts.bullet_data.reset(read_json("Data/Scrpit/弹幕数据.json"));
}
void exitall() {

}
//初始化变量
Base::Base() {
	//初始化基础功能
	stbi_set_flip_vertically_on_load(true);
	window = new Window(1280, 960);
	object_port = new Object_port();
	graphics = new Graphics();
	input = new Input();
	shader_port = new Shader_port();
	font = new Font();
	scene_port = new Scene_port();
	overall();
}

Base::~Base() {
	exitall();
	delete scene_port;
	delete font;
	delete shader_port;
	delete input;
	delete graphics;
	delete object_port;
	delete window;
}

Doc* read_json(const char* src) {
	std::ifstream file;
	std::stringstream sstream;
	std::string json;
	file.open(src);
	sstream << file.rdbuf();
	json = sstream.str();
	file.close();

	auto* dom = new rapidjson::Document;
	dom->Parse(json.c_str());
	return dom;
}

vec2::vec2() {
	x = 0.0f;
	y = 0.0f;
}
vec2::vec2(float a, float b) {
	x = a;
	y = b;
}
void vec2::operator+=(vec2 src) {
	x += src.x;
	y += src.y;
}
void vec2::operator-=(vec2 src) {
	x -= src.x;
	y -= src.y;
}
