// Game_Map只负责运动管理
// 子弹及敌人类型由各自class管理
#include <stdafx.h>
#include <Game_Map.h>
#include <Bullet.h>
#include <Monster.h>
#include <Sprite.h>
#include <Actor.h>
#include <Graphics.h>
#include <vector>
#include <Item.h>
#include <Input.h>
#include <Music.h>
#include <Message.h>
#include <Effect.h>
#include <Scene.h>
#include <Scene_Begin.h>
#include <Game_Function.h>

extern std::vector<Item> Item_vec;
static S_Ptr<Sprite> bold_line;
static S_Ptr<Sprite> bold_line_0;
static int times;
static int stage;
int mon_size;
int recode_size;
S_Ptr<Actor> actor;
float game_variables[20];
#define WAN 10000
#define Rank (Game_data::rank)
#define HARD (Game_data::degree)

std::vector<S_Ptr<Monster>> monsters;
#define CASE_STAGE(a) break;;case a:
#define MOTION(i,aa,bb,cc) monsters[i]->r=aa;monsters[i]->v=bb;monsters[i]->a=cc;
#define MAKE(id) new Monster(id); mon_size++;
#define RX r.x
#define RY r.y
#define VX v.x
#define VY v.y
#define AX a.x
#define AY a.y

bool check_size();
void ini_size();
Monster& Push(int id);
void Fuck();
void bash();

static S_Ptr<Message> message;
bool set_bold(int ,int,int);

void Game_flow() {
	#define During(a,b) (times <= (b) && times >= (a))
	/*static bool initial = true;
	if (initial) {
		stage = 10;
		times = -60;
		actor->immortal = 0;
		Game_data::pour = 128;
		initial = false;
	}*/
	switch (stage) {
		CASE_STAGE(-1) {
			if (times == -50)
				Bullet::push(1, 100, 100, 0, 0);
		}
		CASE_STAGE(0) {
			if (times == 0) {
				game_variables[0] = 0.0f;
			}
			if (times == 0 || times == 15 || times == 30) {
				locus<1>(64 * (times / 15),-times*3, 5,1);
				locus<1>(64 * (11-times / 15), -times * 3, -5,1);
			}
			if (times == 30 || times == 40 || times == 50 || times == 60 || times == 70) {
				locus<1>(64 * 5 - 64 * (times / 10 - 2), 90 - times * 2, 5, 2);
				locus<1>(64 * 6 + 64 * (times / 10 - 2), 90 - times * 2, -5, 2);
			}
			if (times == 45) {
				locus<1>(int(64 * 5.50f), 0, -5, 1);
			}
			if (times == 60) {
				locus<1>(int(64 * 4), 0, 5, 1);
				locus<1>(int(64 * 5.50f), 0, -5, 1);
				locus<1>(int(64 * 7), 0, -5, 1);
			}
			if (times == 75) {
				locus<1>(int(64 * 3), 0, 5, 1);
				locus<1>(int(64 * 4.6f), 0, 5, 1);
				locus<1>(int(64 * 6.2f), 0, -5, 1);
				locus<1>(int(64 * 8), 0, -5, 1);
			}
			if (check_size() && times >= 380) {
				ini_size();
				times = -80;
				stage = 1;
			}
		}
		CASE_STAGE(1) {
			if (times == 0) {
				locus<2>(64 + 128 * 0, 0, 3);
				locus<2>(64 + 128 * 1, 0, 3);
				locus<2>(64 + 128 * 2, 0, 3);
				locus<2>(64 + 128 * 3, 0, 3);
				locus<2>(64 + 128 * 4, 0, 3);
				locus<2>(64 + 128 * 5, 0, 3);
			}
			if (check_size() && times >= 300) {
				ini_size();
				times = -50;
				stage = 2;
			}
		}
		CASE_STAGE(2) {
			if (times == 0) {
				locus<3>(29 - 32, 110 + 29, 1, 4);
				locus<3>(873 + 32, 110 + 29, 2, 4);
				locus<4>(451, -32 + 29, 5);
			}
			if (check_size() && times >= 300) {
				ini_size();
				stage = 3;
				times = -30;
			}
		}
		CASE_STAGE(3) {
			if (times % 30 == 0 && times <= 120) {
				int i = times / 30;
				locus<5>(29 - 32, 210 + 29 - 50 * i, vec2{ 3.0f,0 });
			}
			if (times % 30 == 0 && times <= 180 && times >= 60) {
				int i = times / 30 - 2;
				locus<5>(873 + 32, 210 + 29 - 50 * i, vec2{ -3.0f,0 });
			}
			if (times % 30 == 0 && times <= 240 && times >= 120) {
				int i = times / 30 - 3;
				locus<5>((873 - 29) / 6 * i + 29, 29 - 32, vec2{ 0.0f,3.0f });
			}
			if (times % 30 == 0 && times <= 255 && times >= 135) {
				int i = (times - 15) / 30 - 3;
				locus<5>(int((873 - 29) / 6 * (i + 0.5f)) + 29, 29 - 32, vec2{ 0.0f,4.50f });
			}
			if (times == 150) {
				locus<6>(451, -32 + 29);
			}
			if (check_size() && times >= 300) {
				ini_size();
				stage = 4;
				times = -30;
			}
		}
		CASE_STAGE(4) {
			if (times == 0) {
				if (HARD <= 2) {
					locus<7>((873 - 29) / 4 + 29, -32 + 29, 0);
					locus<7>((873 - 29) / 4 * 3 + 29, -32 + 29, 4);
				} else {
					locus<7>((873 - 29) / 6 * 1 + 29, -32 + 29, 8);
					locus<7>((873 - 29) / 6 * 2 + 29, -32 + 29, 0);
					locus<7>((873 - 29) / 6 * 4 + 29, -32 + 29, 4);
					locus<7>((873 - 29) / 6 * 5 + 29, -32 + 29, 12);
				}
			}
			if (check_size() && times >= 50) {
				ini_size();
				stage = 5;
				times = -60;
			}
		}
		CASE_STAGE(5) {
			if (times == 0) {
				locus<8>(451, -32 + 29);
			}
			if (check_size() && times >= 50) {
				ini_size();
				stage = 6;
				times = -80;
			}
		}
		CASE_STAGE(6) {
			if (times == 0) {
				locus<8>((873 - 29) / 4 + 29, -32 + 29);
			}
			if (check_size() && times >= 50) {
				ini_size();
				stage = 7;
				times = -80;
			}
		}
		CASE_STAGE(7) {
			if (times == 0) {
				locus<8>((873 - 29) / 4 * 3 + 29, -32 + 29);
			}
			if (check_size() && times >= 50) {
				ini_size();
				stage = 8;
				times = -80;
			}
		}
		CASE_STAGE(8) {
			if (times == 0) {
				locus<9>(451, -32 + 29);
			}
			if (check_size() && times >= 50) {
				ini_size();
				stage = 9;
				times = -100;
			}
		}
		CASE_STAGE(9) {
			switch (times) {
				case 0: {
					locus<10>((29 + 873) / 2 + 29, 29 - 32, vec2{ 0,8.0f });
					break;
				}
				case 500: {
					locus<11>(29 - 32, 279, vec2{ 6.50f,0 });
					locus<11>(873 + 32, 279, vec2{ -6.50f, 0 });
					break;
				}
				case 1400: {
					locus<12>(29 - 32, 29 - 32, vec2{ 6.50f,6.9f });
					locus<12>(873 + 32, 29 - 32, vec2{ -6.50f,6.9f });
					locus<12>(29 - 32, 930 + 32, vec2{ 6.50f,-6.9f });
					locus<12>(873 + 32, 930 + 32, vec2{ -6.50f,-6.9f });
					break;
				}
				case 2500: {
					ini_size();
					Bullet::clear(true);
					stage = 10;
					times = -60;
				}
				default:
					break;
			}
			break;
		}
		case 10:
		case 11:
		case 12:
		case 13: {
			if (times == 0) {
				Music::BGM(3);
				bold_line->active = true;
				bold_line_0->active = true;
				bold_line->opacity = 0;
				bold_line_0->active = 0;
				if (mon_size != 0) {
					ini_size();
				}
				Push(12);
				monsters[0]->r = { 451,300 };
				monsters[0]->sprite->opacity = 0;
				monsters[0]->circle->opacity = 0;
				monsters[0]->immortal = true;
				actor->get_spell = true;
			}
			if (times > 0 && times <= 10) {
				bold_line->opacity = min(bold_line->opacity + 26, 255);
				bold_line_0->active = min(bold_line_0->opacity + 26, 255);
				monsters[0]->sprite->opacity = min(monsters[0]->sprite->opacity + 26, 255);
				monsters[0]->circle->opacity = min(monsters[0]->circle->opacity + 26, 255);
			}
			extern int move_by;
			if (stage == 10 && times == 15) {
				move_by = 1;
				monsters[0]->times = 0;
				monsters[0]->call_back = [](Monster* ptr) { locus<13>(ptr); };
			}
			if (stage == 10 && times > 0 && monsters[0]->hp <= 250000 && monsters[0]->v.x == 0) {
				stage = 11;
				monsters[0]->hp = 250000;
				monsters[0]->call_back = [](Monster* ptr) { locus<13>(ptr); locus<14>(ptr); };
			}
			if (stage == 11 && times > 0 && monsters[0]->hp <= 180000 && monsters[0]->v.x == 0) {
				stage = 12;
				monsters[0]->hp = 180000;
				monsters[0]->call_back = [](Monster* ptr) { locus<13>(ptr); locus<14>(ptr); locus<15>(ptr); };
			}
			if (stage == 12 && times > 0 && monsters[0]->hp <= 100000 && monsters[0]->v.x == 0) {
				stage = 13;
				monsters[0]->hp = 100000;
				monsters[0]->call_back = [](Monster* ptr) { locus<13>(ptr); locus<14>(ptr); locus<15>(ptr);  locus<16>(ptr); };
			}
			if (times > 0 && set_bold(0, 778, 300000)) {
				stage = 14;
				monsters[0]->v = { 0,0 };
				monsters[0]->call_back = 0;
				times = -30;
				Music::SE(6);
				Game_data::mark += 100000;
				Bullet::clear(true);
			}
			break;
		}
		case 14: {
			if (times >= 0 && times < 10) {
				bold_line->opacity = max(bold_line->opacity - 26, 0);
				bold_line_0->active = max(bold_line_0->opacity - 26, 0);
				monsters[0]->sprite->opacity = max(monsters[0]->sprite->opacity - 26, 0);
				monsters[0]->circle->opacity = max(monsters[0]->circle->opacity - 26, 0);
			}
			if (times == 10) {
				bold_line->active = false;
				bold_line_0->active = false;
				bold_line->opacity = 0;
				bold_line_0->active = 0;
				ini_size();
				monsters[0] = NULL;
			}
			if (times == 30) {
				int recode = 0, now = 0;
				if (Game_data::hp > 0) {
					int temp[] = { 100000, 250000, 400000, 600000 };
					now = (Game_data::mark + actor->get_spell * temp[HARD-1]) * Game_data::rank * 0.1f;
					std::fstream file("Data/game_recode.dat", std::ios::in);
					if (file) {
						file >> recode;
					} else {
						recode = 0;
					}
					if (now > recode) {
						file = std::fstream("Data/game_recode.dat", std::ios::out);
						file << now;
					}
				}
				message->x = 343;
				message->y = 200;
				std::string text = "";
				if (Game_data::hp > 0) {
					text += "恭喜通关！\n";
					text += "当前得分：" + std::to_string(Game_data::mark) + "\n";
					text += "关底收取奖励：" + std::to_string(actor->get_spell * 100000) + "\n";
					text += "难度加成：" + std::to_string(Game_data::rank * 10) + "%\n";
					text += "实际分数：" + std::to_string(now) + "\n";
					text += "历史最高：" + std::to_string(recode) + "\n\n";
					text += "感谢您的参与！\n";
					text += "Z键 返回标题菜单\n";
				} else {
					message->x = 363;
					message->y = 350;
					text += "由于当前已续关，\n无法记录分数\n";
					text += "请再接再厉吧！\n\n";
					text += "Z键 返回标题菜单\n";
				}
				*message = text;
				(*message)->color = { 180,180,180,0 };
				(*message)->rowsp = 0.15f;
				(*message)->italic = true;
				auto& i = (*message)->font_size = 50;
			}
			if (times == 59) {
				Effect_Froze(40, { 0,0,0 ,255 });
				Graphics::update(40);
			}
			if (times >= 60 && times < 80) {
				(*message)->color.a = min(255, (times - 60) * 13);
			}
			if (times >= 80) {
				if (INPUT_C) {
					Music::SE(2);
					for (int i = 0; i < 20; i++) {
						(*message)->color.a = max(0, 255-i*12);
						Graphics::update();
					}
					Scene::reset(new Scene_Begin);
					return;
				}
			}
			break;
		}
	}
	if (times < 1000000) {//防报错而已
		times++;
	}
	Fuck();
}
void Fuck() {
	//处死提前跳转
	if (actor->crash_time >= 1) {
		goto Cu_shi;
	}
	//碰撞检测
	if (!actor->immortal) {
		for (int i = 0; i < Bullet::pool->max; i++) {
			Bullet::at(i).check_crash();
		}
		bash();
	}
	for (int i = 0; i < ITEM_MAX; i++) {
		if (Item_vec[i].check_crash()) {
			Item_vec[i].del();
		};
	}
	actor->Global_Actor_Bullet_Collision();
	
	//处死决议
	if (actor->crash_time >= 1) {
		Cu_shi:
		if (actor->crash_time == 1) {
			if (Game_data::sp == 0) {
				actor->crash_time = CHU_SHI_TIMES;
			} else {
				Mini_Animation(3, 0, 0, 0);
			}
		}
		if (Input::input(R_X) && Game_data::sp) {
			actor->crash_time = 0;
		} else {
			//处死失败
			if (actor->crash_time >= CHU_SHI_TIMES) {
				actor->miss();
			} else {
				actor->crash_time++;
				return;
			}
		}
	}

	//位置设置
	for (int i = 0; i < Bullet::pool->max; i++) {
		Bullet::at(i).update();
	}
	for (auto i = 0; i < mon_size; i++) {
		if (monsters[i] && monsters[i]->active) {
			//离开画面
			if (monsters[i]->r.x < -100 || monsters[i]->r.x > HEIGHT + 100 || monsters[i]->r.y < -200 || monsters[i]->r.y > HEIGHT + 100) {
				monsters[i].reset();
				recode_size++;
				continue;
			}
			//生命归零
			if (monsters[i]->hp <= 0 && !monsters[i]->boss) {
				extern int boom_times;
				if (!boom_times) {
					Music::SE(4);
				}
				int kk = 96 + 16 * monsters[i]->fall_items.size();
				for (auto j : (monsters[i]->fall_items)) {
					int tx = (rand() % kk) - kk/2; tx = min(max(tx, 0), RIGHT_EDGE);
					int ty = (rand() % kk) - kk/2; ty = min(max(ty, 0), DOWN_EDGE);
					Item::Push(j,monsters[i]->r.x+tx, monsters[i]->r.y+ty);
					Game_data::mark += monsters[i]->mark;
				}
				Mini_Animation(1, monsters[i]->color, monsters[i]->r.x, monsters[i]->r.y);
				monsters[i]->sprite.reset();
				monsters[i]->circle.reset();
				monsters[i].reset();
				recode_size++;
				continue;
			}
			//位置重置
			monsters[i]->update();
		}
	}
	actor->update();
	for (int i = 0; i < ITEM_MAX; i++) {
		Item_vec[i].update();
	}
}
bool check_size() {
	return (recode_size == mon_size);
}
void ini_size() {
	if (!check_size()) {
		for (auto i = 0; i < mon_size; i++) {
			if (monsters[i]) {
				monsters[i].reset();
			}
		}
	}
	mon_size = 0;
	recode_size = 0;
}

//接口管理
Game_port::Game_port() {
	times = -100;
	stage = 0;
	mon_size = 0;
	recode_size = 0;
	actor.reset(new Actor);
	monsters.resize(300);
	actor->immortal = 100;
	for (int i = 0; i < 10; i++) {
		game_variables[i] = 0.0f;
	}
	bold_line.reset(new Sprite(ZH_TITLE("血条"), 62+3, 74+2, 99));
	bold_line_0.reset(new Sprite(ZH_TITLE("空血条"), 62, 74, 99));
	message.reset(new Message(0, 0, 999999));
	bold_line->active = false;
	bold_line_0->active = false;
}
Game_port::~Game_port() {
	actor.reset();
	monsters.clear();
	bold_line.reset();
	bold_line_0.reset();
	message.reset();
}

//添加敌机
Monster& Push(int id) {
	monsters[mon_size].reset(new Monster(id));
	return *monsters[mon_size++];
}
//敌机体术判定
void bash() {
	for (auto i = 0; i < mon_size; i++) {
		if (monsters[i] && monsters[i]->range.x > 0) {
			int t = monsters[i]->range.x * (25.0f / 32.0f);
			if (abs(monsters[i]->r.x - actor->x) <= t && abs(monsters[i]->r.y - actor->y) <= t) {
				if (!actor->crash_time) {
					actor->crash_time = 1;
					if (!(monsters[i]->circle || monsters[i]->boss)) {
						monsters[i]->call_back = [](Monster* ptr) { ptr->hp = 0; };
					}
				}
			}
		}
	}
}
//我方子弹判定
void Actor_Bullet::check_crash() {
	if (active){
		for (auto i = 0; i < mon_size; i++) {
			if (monsters[i] && monsters[i]->range.x > 0) {
				if (r.x < RIGHT_EDGE && r.x > LEFT_EDGE && r.y > UP_EDGE && r.y < DOWN_EDGE) {
					vec2 d = { abs(monsters[i]->r.x - r.x),abs(monsters[i]->r.y - r.y) };
					if ((d.x < range.x + monsters[i]->range.x) && (d.y < range.y + monsters[i]->range.y)) {
						if (!monsters[i]->immortal)
							monsters[i]->hp -= danger;
						//Music::SE(11);
						del();
						if (times <= 10000) {
							Game_data::mark += 10;
						}
						return;
					}
				}
			}
		}
	}
};
//敌方子弹判定
#define pow(a) ((a)*(a))
void grazed_off();
void Bullet::check_crash() {
	if (active) {
		int round = 0;
		re_run:
		if (round == 0) {
			vec2 temp = {actor->x, actor->y};
			actor->x = actor->recode_pos.x;
			actor->y = actor->recode_pos.y;
			actor->recode_pos = temp;
		}
		if (round == 1) {
			actor->x = actor->recode_pos.x;
			actor->y = actor->recode_pos.y;
		}
		vec2 d = { abs(actor->x - r.x), abs(actor->y - r.y) };
		vec2 d2 = { pow(d.x),pow(d.y) };

		//擦弹
		if (d2.x + d2.y <= 48*48 && !has_grazed) {
			has_grazed = true;
			grazed_off();
			Music::SE(5);
		}

		switch (status) {
			case 1: {//圆形（正交）
				float XR = pow(range.x) + pow(actor->range.x);
				if (d2.x + d2.y <= XR) {
					if (!actor->crash_time) {
						actor->crash_time = 1;
						call_back = [](Bullet* ptr) {ptr->del(); };
					}
				}
				break;
			}
			case 2: {//椭圆 存在30%容错
				float distance = sqrt(d2.x + d2.y);
				float r = actor->range.x;
				vec2 d3 = { d.x/distance*r,d.y/distance*r };
				vec2 d4 = { (d.x - d3.x) / range.x,(d.y - d3.y) / range.y };//仿变后坐标
				float R = pow(d4.x) + pow(d4.y);
				if (R <= 1.0f*0.7f) {
					if (!actor->crash_time) { 
						actor->crash_time = 1; 
						call_back = [](Bullet* ptr) {ptr->del(); };
					}
				}
				break;
			}
			case 3: {//矩形
				if (d.x <= range.x && d.y <= range.y) {
					if (!actor->crash_time) {
						actor->crash_time = 1;
						call_back = [](Bullet* ptr) {ptr->del(); };
					}
				}
				break;
			}
		}
		if (round == 0) {
			round = 1;
			goto re_run;
		}
	}
};
//擦弹判定
void grazed_off() {
	if (times <= 10000) {
		Game_data::mark += 100 + Game_data::point * 10;
	}
}
//a 血条左位置，b 血条长度，m 总血量
bool set_bold(int a, int b, int m) {
	float t = (monsters[0]->hp) / float(m);
	int src = min(b, max(b * t, 0));
	bold_line->src({ 0,0,a + src,27 });
	if (monsters[0] && monsters[0]->hp <= 0) {
		return true;
	}
	return false;
}

int Game_data::mark;//分数
int Game_data::hp;//残机
int Game_data::sp;//炸弹
int Game_data::pour;//火力
int Game_data::point;//点数
int Game_data::rank;//Rank
int Game_data::degree;//难度
int Game_data::random_seed;//随机数种子
