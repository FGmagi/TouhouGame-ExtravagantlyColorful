//===========================================================================================
// Window_Begin 开幕画面
//===========================================================================================
#include <stdafx.h>
#include <Scene_Begin.h>
#include <Graphics.h>
#include <Window.h>
#include <Window_Selection.h>
#include <Input.h>
#include <Music.h>
#include <Font.h>
#include <Effect.h>
#include <Canvas.h>
#include <Sprite.h>
#include <Message.h>
#include <Shader.h>
#include <Scene_Ready.h>
#include <Sprite_Map.h>
#include <Music.h>
void write();
//===========================================================================================
// 主循环
//===========================================================================================
void Scene_Begin::main() {
	{
		Effect_Froze(0, { 0, 0, 0, 0});
		Effect effect(reinterpret_cast<Functor*>(MAKE<Model>()),5);
		Graphics::update(60);
	}
	do {
		Graphics::update();
		auto mes = selection->update();
		if (mes.recode != mes.tax) {
			Music::SE(1);
			(*message[mes.recode])->color = get_color(11);
			(*message[mes.tax])->color = get_color(0);
		}

		if (INPUT2_C) {
			switch (selection->get_tax()) {
				case 0: {//开始游戏
					Music::SE(2);
					Effect_Froze(20, { 0,0,0,255 });
					Graphics::update(20);
					Scene::reset(new Scene_Ready);
					break;
				}
				case 1: 
					Music::SE(2);
					son_loop();
					break;
				case 2://退出游戏
					Music::SE(3);
					Scene::reset();
					break;
			}
		}
		if (INPUT_B) {
			Music::SE(1);
			auto mes = selection->set_tax(2);
			(*message[mes.recode])->color = get_color(11);
			(*message[mes.tax])->color = get_color(0);
		}

	} while (Scene::judge(this));
};

void Scene_Begin::son_loop() {
	for (int i = 0; i < 15; i++) {
		(*message[0])->color.a = 255 - i * 17;
		(*message[1])->color.a = 255 - i * 17;
		(*message[2])->color.a = 255 - i * 17;
		Graphics::update();
	}
	std::string temp = "BGM音量:" + std::to_string(int(consts.BGM_volume));
	(*message[0]) = temp;
	(*message[0])->color = get_color(0);
	temp = "SE音量:" + std::to_string(int(consts.SE_volume));
	(*message[1]) = temp;
	(*message[1])->color = get_color(11);
	(*message[2]) = "返回";
	(*message[2])->color = get_color(11);
	for (int i = 0; i < 3; i++) {
		(*message[i])->font_size = 44;
		(*message[i]).x = 120+i*5;
		(*message[i]).y += 15;
	}
	for (int i = 0; i < 15; i++) {
		(*message[0])->color.a = i * 17;
		(*message[1])->color.a = i * 17;
		(*message[2])->color.a = i * 17;
		Graphics::update();
	}
	int tax = 0;
	while (true) {
		Graphics::update();
		if (Input::repeat(R_UP)) {
			Music::SE(1);
			(*message[tax])->color = get_color(11);
			tax = (tax == 0) ? 2 : tax-1;
			(*message[tax])->color = get_color(0);
		}
		if (Input::repeat(R_DOWN)) {
			Music::SE(1);
			(*message[tax])->color = get_color(11);
			tax = (tax == 2) ? 0 : tax+1;
			(*message[tax])->color = get_color(0);
		}
		if (Input::repeat(R_RIGHT)) {
			Music::SE(1);
			if (tax == 0) {
				Music::BGM_Volume(min(consts.BGM_volume + 5, 100));
				std::string temp = "BGM音量:" + std::to_string(consts.BGM_volume);
				(*message[0]) = temp; write();


			}
			if (tax == 1) {
				Music::SE_Volume(min(consts.SE_volume +5, 100));
				std::string temp = "SE音量:" + std::to_string(consts.SE_volume);
				(*message[1]) = temp; write();
			}
		}
		if (Input::repeat(R_LEFT)) {
			Music::SE(1);
			if (tax == 0) {
				Music::BGM_Volume(max(0, consts.BGM_volume - 5));
				std::string temp = "BGM音量:" + std::to_string(consts.BGM_volume);
				(*message[0]) = temp; write();
			}
			if (tax == 1) {
				Music::SE_Volume(max(0, consts.SE_volume - 5));
				std::string temp = "SE音量:" + std::to_string(consts.SE_volume);
				(*message[1]) = temp; write();
			}
		}
		if (INPUT_C && tax == 2) {
			Music::SE(2);
			goto flag;
		}
		if (INPUT_B) {
			Music::SE(3);
			flag:
			for (int i = 0; i < 15; i++) {
				(*message[0])->color.a = 255 - i * 17;
				(*message[1])->color.a = 255 - i * 17;
				(*message[2])->color.a = 255 - i * 17;
				Graphics::update();
			}
			(*message[0]) = "开始游戏";
			(*message[1]) = "调整设置";
			(*message[2]) = "休息一下";
			(*message[0])->color = get_color(11);
			(*message[1])->color = get_color(0);
			(*message[2])->color = get_color(11);
			(*message[0])->font_size = 54;
			(*message[1])->font_size = 54;
			(*message[2])->font_size = 54;
			(*message[0]).x = 112;
			(*message[1]).x = 125;
			(*message[2]).x = 138;
			(*message[0]).y = 619;
			(*message[1]).y = 699;
			(*message[2]).y = 779;
			for (int i = 0; i < 15; i++) {
				(*message[0])->color.a = i * 17;
				(*message[1])->color.a = i * 17;
				(*message[2])->color.a = i * 17;
				Graphics::update();
			}
			selection->set_tax(1);
			break;
		}
	}
}
void write() {
	std::ofstream file("Data/game_data.dat", std::ios::out);
	file << consts.BGM_volume<<" "<< consts.SE_volume;
}
//===========================================================================================
// 构造函数
//===========================================================================================
Scene_Begin::Scene_Begin(){
	message[0].reset(new Message("开始游戏", 112, 619, 1));
	message[1].reset(new Message("调整设置", 125, 699, 1));
	message[2].reset(new Message("休息一下", 138, 779, 1));
	for (int i = 0; i < 3; i++) {
		Message& n = *message[i];
		n->font_size = 54;
		n->italic = true;
		n->slender = true;
		n->color = get_color(11);
	}
	(*message[0])->color = get_color(0);
	Music::BGM(1);
};
Scene_Begin::~Scene_Begin() {
	delete background;
	delete selection;
}
//===========================================================================================
//  数据结构
//===========================================================================================
inline bool pass_effect() {
	return (Input::press(R_UP) || Input::press(R_DOWN) || (INPUT_C));
};
class Model :public Functor {
public:
	Model() :shader("B1-开幕专用着色器", "B1-开幕专用着色器") {
		times = 0;
	};
	void update() {
		if (times < 60) {
			vao.Bind();
			shader.Bind();
			Uniform(1i, Time, times);
			Uniform(1f, Middle, WIDTH / 2);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			vao.relieve();
		}
		times++;
		if (times >= 60 || pass_effect()) {
			mul_effect->active = false;
			Graphics::wait_time() = 0;
		}
	};
	Shader shader;
	VAO vao;
	int times;
};