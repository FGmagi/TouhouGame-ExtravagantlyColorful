//============================================================
//	头文件引用
//============================================================
#include <stdafx.h>
#include <Scene_Map.h>
#include <Graphics.h>
#include <Bitmap.h>
#include <Sprite.h>
#include <Effect.h>
#include <Actor.h>
#include <Monster.h>
#include <Bullet.h>
#include <Game_Map.h>
#include <Input.h>
#include <Scene_Begin.h>
#include <functional>
#include <Message.h>
#include <Music.h>
#include <Sprite_Map.h>
#include <Item.h>
#include <time.h>

void input_update();

class Panel {
public:
	S_Ptr<Sprite> show_mark[9];
	S_Ptr<Sprite> show_hp[7];
	S_Ptr<Sprite> show_sp[7];
	S_Ptr<Sprite> show_power[3];
	S_Ptr<Sprite> show_point[6];
	S_Ptr<Sprite> show_rank[2];
	int num[6] = { 0,0,0,0,0,0 };
	S_Ptr<Sprite_Map> sprite_map;

	Panel() {
		sprite_map.reset(new Sprite_Map(105));
		for (int i = 0; i < 9; i++) {
			show_mark[i] = std::make_shared<Sprite>(*consts.image_item, 995 + i * 30, 80, 105);
			sprite_map->push(&*show_mark[i]);
		}

		for (int i = 0; i < 7; i++) {
			show_hp[i] = std::make_shared<Sprite>(*consts.image_item, 1000 + i * 35, 156, 105);
			show_sp[i] = std::make_shared<Sprite>(*consts.image_item, 1000 + i * 35, 231, 105);
			sprite_map->push(&*show_hp[i]);
			sprite_map->push(&*show_sp[i]);
		}

		for (int i = 0; i < 3; i++) {
			show_power[i] = std::make_shared<Sprite>(*consts.image_item, 995 + i * 30, 302, 105);
			show_power[i]->active = false;
			sprite_map->push(&*show_power[i]);
		}

		for (int i = 0; i < 6; i++) {
			show_point[i] = std::make_shared<Sprite>(*consts.image_item, 995 + i * 30, 380, 105);
			show_point[i]->active = false;
			sprite_map->push(&*show_point[i]);
		}

		for (int i = 0; i < 2; i++) {
			show_rank[i] = std::make_shared<Sprite>(*consts.image_item, 1012 + i * 30, 868, 105);
			show_rank[i]->active = false;
			sprite_map->push(&*show_rank[i]);
		}
		update();
	};
	~Panel() {
	}
	void update() {
		draw_num(show_mark, Game_data::mark, 9,9);//分数
		draw_icon(show_hp, 0);//残机
		draw_icon(show_sp, 1);//炸弹
		draw_num(show_power, Game_data::pour, 0,3);//火力
		draw_num(show_point, Game_data::point, 0,6);//点数
		draw_num(show_rank, Game_data::rank, 0,2);//Rank

	};
	void draw_num(S_Ptr<Sprite> ptr[], int n, int len,int max) {
		char text[10] = { 0 };
		_itoa_s(n, text, 10);
		if (len == 0) {
			for (int i = 0; i < max; i++) {
				if (text[i] != '\0') {
					ptr[i]->src({ 30 * (text[i] - 48) + 15, 324, 30, 34 });
					ptr[i]->active = true;
				} else {
					for (int j = i; j < max; j++) {
						ptr[j]->active = false;
					}
					break;
				}
			}
		} else {
			int re_len = int(log10(n)) + 1;
			for (int i = 0, a = 0; i < len; i++) {
				if (len - i <= re_len) {
					ptr[i]->src({ 30 * (text[a] - 48) + 15, 324, 30, 34 });
					a++;
				} else {
					ptr[i]->src({ 15, 324, 30, 34 });
				}
			}
		}
	};
	void draw_icon(S_Ptr<Sprite> ptr[], int kind) {
		if (kind == 0) {
			if (Game_data::hp <= 0) {
				for (int i = 1; i < 7; i++) {
					ptr[i]->active=false;
				}
				ptr[0]->y=138;
				ptr[0]->src({ 576, 320, 8 * 32, 64 });
			} else {
				for (int i = 0; i < 7; i++) {
					if (i < Game_data::hp - 1)
						ptr[i]->src({ 516, 323, 35, 34 });
					else
						ptr[i]->src({ 336, 324, 35, 34 });
				}
			}
		} else {
			for (int i = 0; i < 7; i++) {
				if (i < Game_data::sp)
					ptr[i]->src({ 516, 368, 35, 34 });
				else
					ptr[i]->src({ 336, 367, 35, 34 });
			}
		}
	};
};

void Scene_Map::main() {
	Game_data::mark = 0;//分数
	Game_data::hp = 3;//残机
	Game_data::sp = 3;//炸弹
	Game_data::pour = 0;//火力
	Game_data::point = 0;//点数
	int a[] = { 5,12,15,20 };
	Game_data::rank = a[Game_data::degree - 1];//Rank
	Game_data::random_seed = time(NULL);
	srand(Game_data::random_seed);
	Music::BGM(2);

	Sprite background(ZH_GROUND("B-游戏背景1"), LEFT_EDGE, UP_EDGE, 0);
	Sprite panelground(ZH_GROUND("B-游戏面板"), 0, 0, 100);
	Bullet_port Bullet_port;
	Item_port item_port;
	Panel panel;
	Game_port game_port;
	{
		Effect_Froze(20, { 0,0,0,0 });
		Graphics::update(20);
	}
	do {
		Graphics::update();
		panel.update();
		Game_flow();
		input_update();

	} while (Scene::judge(this));
}
void input_update() {
	if (Input::input(R_ESCAPE)) {
		Music::SE(9);
		Sprite canvas0({ 0,0,1280,960 }, 99);
		canvas0.bitmap.screenshot();
		Sprite canvas1(ZH_TITLE("C-暂停"), 496, 369, 100);
		canvas1.opacity = 0;
		Shader t_shader("A0-基础着色器", "B2-闪烁着色器");
		canvas1.shader(t_shader);
		int tax = 0;
		for (int i = 0; i < 8; i++) {
			canvas0.bitmap.hsl({ 1.0f,0.8f,1.0f });
			canvas1.opacity = 255.0f/8.0f*(i+1);
			Graphics::update();
		}
		float times = 1.0f;
		while (true) {
			Graphics::update();
			times += 0.03f;
			if (times >= 2.0f)
				times = 0.0f;
			float s = 0.2f+sin(times * 3.141592 / 2)*0.8;
			canvas1.call_back = [=]() {Uniform(4f, u_color, 1.0f + 0.1f * s, s, s, 1.0f);
			Uniform(4f, u_range, 0.0f, (1 - tax) * 0.3f, 1.0f, (1 - tax) * 0.3f + 0.3f); };
			if (Input::repeat(R_UP) || Input::repeat(R_DOWN)) {
				tax = !tax;
				times = 0.0f;
			}
			if (INPUT_B) {
				Music::SE(3);
				break;
			}
			if (INPUT_C) {
				if (tax == 0) {
					Music::SE(2);
					break;
				} else {
					Music::SE(2);
					Effect_Froze(20, { 0,0,0 ,255 });
					Graphics::update(20);
					Scene::reset(new Scene_Begin);
					return;
				}
			}
		};
		for (int i = 0; i < 8; i++) {
			canvas0.bitmap.hsl({ 1.0f,1.22f,1.0f });
			if (tax) {

			} else {
				canvas1.opacity = 255- 255.0f / 8.0f * (i + 1);
			}
			Graphics::update();
		}

	}
}