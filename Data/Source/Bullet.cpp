#include "stdafx.h"
#include "Bullet.h"
#include "Sprite_Map.h"
#include "Actor.h"
#include "Effect.h"
#include "Monster.h"
#include "Item.h"
#define EFFECT_Z 999
#define Bullet_MAX 3000
S_Ptr<Bullet::Pool> Bullet::pool;
S_Ptr<Sprite_Map> Bullet::sprite_map;

void Bullet::ini() {
	id = 0;
	r = vec2{ 0,0 };
	v = vec2{ 0.0f,0.0f };
	a = vec2{ 0.0f,0.0f };
	offset = vec2{ 16.0f,16.0f };
	color = -1;
	times = 0;
	active = true;
	has_grazed = false;
	range = vec2{ 0.0f,0.0f };
	immortal = false;
	if (sprite) {
		sprite->active = true;
		sprite->opacity = 75;
		sprite->orgin(0.5f, 0.5f);
		sprite->angle();
		sprite->zoom();
	}
}
Bullet::Bullet() {
	ini();
}
void Bullet::clear(bool forcus) {
	for (int i = 0; i < Bullet::pool->max; i++) {
		auto &j = Bullet::at(i);
		if (j.active) {
			if (forcus || (j.immortal == false)) {
				Item* a = Item::Push(6, j.r.x, j.r.y);
				a->follow = true;
				j.del();
			}
		}
	}
}
void Bullet::update() {
	if (active) {
		if (sprite->opacity.value != 255) {
			sprite->opacity = min((sprite->opacity + 15), 255);
		}
		//平动
		r += v;
		v += a;
		//回调
		if (call_back) {
			call_back(this);
		}
		times++;
		//更新顶点
		sprite->x = static_cast<int>(round(r.x - offset.x));
		sprite->y = static_cast<int>(round(r.y - offset.y));
		if (r.x < -100 || r.x > HEIGHT + 100 || r.y < -100 || r.y > HEIGHT + 100) {
			del();
		}
	}
};
Bullet* Bullet::push(int id, float x, float y,float vx,float vy) {
	auto i = push();
	i->set_kind(id);
	i->r = { x,y };
	i->v = { vx,vy };
	return i;
}
Bullet* Bullet::push(int id, float x, float y) {
	auto i = push();
	i->set_kind(id);
	i->r = { x,y };
	return i;
}
Bullet* Bullet::push(int id) {
	auto i = push();
	i->set_kind(id);
	return i;
}
Bullet* Bullet::push() {
	auto q = pool->index;
	for (int i = pool->index; i < pool->max; i++) {
		if (auto& f = at(i);f.active == false) {
			f.ini();
			pool->index = i;
			return &at(i);
		}
	}
	for (int i = 0; i < q; i++) {
		if (auto& f = at(i); f.active == false) {
			f.ini();
			pool->index = i;
			return &at(i);
		}
	}
	return NULL;
};
void Bullet::del() {
	at(note).r = { 0,0 };
	at(note).active = false;
	at(note).sprite->active = false;
	at(note).sprite->x = 0;
	at(note).sprite->y = 0;
	at(note).call_back = NULL;
	if (note < pool->index) {
		pool->index = note;
	}
}
Bullet& Bullet::at(int i) {
	return pool->Bullet_list[i];
};


Bullet_port::Bullet_port() {
	Bullet::pool.reset(new Bullet::Pool);
	Bullet::sprite_map.reset(new Sprite_Map(EN_Bullet_Z));
	Bullet::pool->Bullet_list.resize(Bullet_MAX);
	for (int i = 0; i < Bullet_MAX; i++) {
		Bullet temp;
		Bullet::at(i) = temp;
		Bullet::at(i).note = i;
		Bullet::at(i).sprite.reset(new Sprite(*consts.image_bullet, 0, 0, EN_Bullet_Z));
		Bullet::at(i).ini();
		Bullet::at(i).sprite->active = false;
		Bullet::at(i).active = false;
		Bullet::sprite_map->push(&*Bullet::at(i).sprite);
	}
	Bullet::pool->index = 0;
	Bullet::pool->max = Bullet_MAX;
}
Bullet_port::~Bullet_port() {
	Bullet::pool.reset();
	Bullet::sprite_map.reset();
}

#define SET(a,b,c,d) sprite->src({a,b,c,d})
void Bullet::set_kind(int id) {
	this->id = id;
	Doc* dom = &*consts.bullet_data;
	if (auto f = dom->FindMember(std::to_string(id).c_str()); f != dom->MemberEnd() && f->value.IsObject()) {
		auto& da = f->value;
		if (auto g = da.FindMember("src"); g !=  da.MemberEnd() && g->value.IsArray()) {
			SET(g->value[0].GetInt(), g->value[1].GetInt(), g->value[2].GetInt(), g->value[3].GetInt());
		} else {
			SET(32, 352, 32, 32);
		}
		if (auto g = da.FindMember("status"); g !=  da.MemberEnd() && g->value.IsInt()) {
			status = (g->value.GetInt());
		} else {
			status = 0;
		}
		if (auto g = da.FindMember("range"); g != da.MemberEnd() && g->value.IsArray()) {
			range = { float(g->value[0].GetDouble()),float(g->value[1].GetDouble()) };
		} else {
			range = { 16.0f,16.0f };
		}
		if (auto g = da.FindMember("immortal"); g != da.MemberEnd() && g->value.IsBool()) {
			immortal = g->value.GetBool();
		} else {
			immortal = false;
		}
		if (auto g = da.FindMember("offset"); g != da.MemberEnd() && g->value.IsArray()) {
			auto len = sprite->get_src();
			offset = { float(g->value[0].GetDouble()),float(g->value[1].GetDouble()) };
			sprite->orgin(offset.x/len.w, offset.y / len.h);
		} else {
			auto len = sprite->get_src();
			offset = { len.w * 0.5f,len.h * 0.5f };
		}
		static std::map<std::string, int> color_map = {
			{"White",0},
			{"Red",1},
			{"Pink",2},
			{"Bule",3},
			{"Bule2",4},
			{"Green",5},
			{"Yellow", 6}
		};

		if (auto g = da.FindMember("color"); g !=  da.MemberEnd() && g->value.IsString()) {
			std::string c = g->value.GetString();
			color = color_map[c];
		} else {
			color = -1;
		}
	}
	call_back = NULL;
}

class Mini_Animation_realize : Object {//实现类
public:
	Mini_Animation_realize(int id, int color, int x, int y);
	~Mini_Animation_realize() {};
	void update();
	U_Ptr<Sprite> sprite;
	std::function<void(Mini_Animation_realize*)> call_back;
	int times = 0;
};
Mini_Animation_realize::Mini_Animation_realize(int id, int color, int x, int y) :Object(0) {
	switch (id) {
		case 0://敌机发弹
			sprite.reset(new Sprite(*consts.image_bullet, x - 32, y - 32, Mini_Animation_Z));
			sprite->opacity = 205;
			sprite->zoom(1.2f);
			sprite->src({ 0+64*color, 288,64, 64 });
			call_back = [](Mini_Animation_realize* ptr){
				ptr->times++;
				if (ptr->times >= 8) {
					ptr->idioctonia = true;
				};
			};
			break;
		case 1://敌机死亡
			sprite.reset(new Sprite(*consts.image_bullet2, x - 64, y - 64, Mini_Animation_Z));
			sprite->opacity = 225;
			switch (color){
				case 0:sprite->src({ 0, 0, 128, 128 }); break;
				case 1:sprite->src({ 0, 128, 128, 128 }); break;
				case 2:sprite->src({ 128, 128, 128, 128 }); break;
				case 3:sprite->src({ 128, 0, 128, 128 }); break;
			}
			
			call_back = [](Mini_Animation_realize* ptr) {
				ptr->sprite->zoom(1.05f);
				ptr->sprite->opacity -= 15;
				ptr->times++;
				if (ptr->times >= 12) {
					ptr->idioctonia = true;
				};
			};
			break;
		case 2://自己命中
			sprite.reset(new Sprite(*consts.image_bullet2, x - 64, y - 64, Mini_Animation_Z));
			sprite->opacity = 225;
			sprite->src({ 0, 128, 128, 128 });
			call_back = [](Mini_Animation_realize* ptr) {
				ptr->sprite->zoom(1.05f);
				ptr->sprite->opacity -= 15;
				ptr->times++;
				if (ptr->times >= 12) {
					ptr->idioctonia = true;
				};
			};
			break;
		case 3://被弹红色窗口
			Bitmap red_can(844, 900);
			red_can.fill_rect({ 255,50,50 }, { 0,0,844,900 });
			sprite.reset(new Sprite(red_can, LEFT_EDGE, UP_EDGE, 2000));
			sprite->opacity = 100;
			call_back = [](Mini_Animation_realize* ptr) {
				ptr->times++;
				if (ptr->times >= CHU_SHI_TIMES) {
					ptr->idioctonia = true;
				};
			};
			break;
	}
	times = 0;
}
void Mini_Animation_realize::update() {
	call_back(this);
}
Mini_Animation::Mini_Animation(int id, int color, int x, int y) {
	new Mini_Animation_realize(id, color, x, y);
}
