#include "stdafx.h"
#include "Sprite.h"
#include "Item.h"
#include "Actor.h"
#include "Game_Map.h"
#include <Music.h>
#include <cmath>
#define Item_W 32
#define Item_H 32
#define FALL_TIMES 65//物品掉落变速时间
std::vector<Item> Item_vec;
int index;
extern S_Ptr<Actor> actor;

Item* Item::Push(int id, float x, float y) {
	Item* p = NULL;
	for (int i = index; i < ITEM_MAX; i++) {
		if (Item_vec[i].active==false) {
			p = &Item_vec[i];
			index = i;
			goto flag;
		}
	}
	for (int i = 0; i < index; i++) {
		if (Item_vec[i].active == false) {
			p = &Item_vec[i];
			index = i;
			goto flag;
		}
	}

	flag:
	p->id = id;
	p->r.x = x;
	p->r.y = y;
	p->follow = false;
	p->fx = 0;
	p->uniform = 5;
	if (p->r.y > 50) {
		p->fy = -7.2f;
	} else {
		p->fy = -6.0f;
	}
	p->active = true;
	p->sprite->src({ id * 32,371,Item_W,Item_H });
	p->sprite->active = true;
	return p;
}

Item::Item() {
	id = 0;
	r.x = 0;
	r.y = 0;
	follow = 0;
	fx = 0;
	uniform = 0;
	active = false;
	sprite.reset(new Sprite(*consts.image_item, 0, 0, Item_Z));
	sprite->active = false;
}
void Item::del() {
	active = false;
	sprite->active = false;
}
void Item::update() {
	if (active) {
		float ax = actor->x - r.x;
		float ay = actor->y - r.y;
		if (follow) {
			float distance = sqrt(ax * ax + ay * ay);
			fx = (ax / distance) * 15.0f + distance * 0.001;
			fy = (ay / distance) * 15.0f + distance * 0.001;
			r.x += fx;
			r.y += fy;
		} else {
			if (actor->y <= 300) {//回收线
				follow = 2;
			} else {
				if (fy < 0) {
					r.x += fx;
					r.y += fy;
					fy += 0.18f;
					goto flag0;
				}
				if (uniform){
					uniform --;
					goto flag0;
				}
				if (fy <= 2.7f) {
					r.x += fx;
					r.y += fy;
					fy += 0.1667f;
					goto flag0;
				}
				r.x += fx;
				r.y += fy;
			}
		}
		flag0:
		sprite->x = r.x - Item_W / 2;
		sprite->y = r.y - Item_H / 2;
		if (r.x + Item_W / 2 < 0 || r.x > HEIGHT + Item_H / 2 || r.y + 150 < 0 || r.y > HEIGHT + Item_H / 2) {
			del();
			return;
		}
	}
}

bool Item::check_crash() {
	if (active) {
		#define cross(rx,ry)(abs(ax) <= rx/2+ACTOR_W/2 && abs(ay) <= ry/2+ACTOR_H/2)
		float ax = actor->x - r.x;
		float ay = actor->y - r.y;

		float doub = (920 - max(actor->y, 300)) / 620.0f * 0.5f + 0.5f;
		if (follow == 2) {
			doub = 1.0f;
		}
		if (actor->slow) {//吸引
			if (!follow && abs(ax) <= 120 && abs(ay) <= 120) {
				follow = 1;
			}
		} else {
			if (!follow && abs(ax) <= 80 && abs(ay) <= 80) {
				follow = 1;
			}
		}
		if (id == 0 && cross(32, 32)) {//残机
			Music::SE(8);
			if (Game_data::hp >0) Game_data::hp += 1;
			return true;
		}
		if (id == 1 && cross(32, 32)) {//炸弹
			Music::SE(8);
			Game_data::sp += 1;
			return true;
		}
		if (id == 2 && cross(28, 29)) {//火力
			Music::SE(10);
			if (Game_data::pour < 128) {
				Game_data::pour += 1;
				Game_data::mark += 50 * doub;
			} else {
				Game_data::mark += 150 * doub;
			}
			return true;
		}
		if (id == 3 && cross(28, 29)) {//蓝点
			Music::SE(10);
			Game_data::point += 1;
			Game_data::mark += 1500 * doub;
			return true;
		}
		if (id == 4 && cross(28, 29)) {//大火力
			Music::SE(10);
			if (Game_data::pour < 128) {
				Game_data::pour = min(Game_data::pour+5,128);
				Game_data::mark += 100 * doub;
			} else {
				Game_data::mark += 300 * doub;
			}
			return true;
		}
		if (id == 5 && cross(32, 32)) {//复活
			Music::SE(8);
			Game_data::pour = 128;
			return true;
		}
		if (id == 6 && cross(28, 29)) {//灰点
			Game_data::mark += 50 * doub;
			return true;
		}
	}
	return false;
}
Item_port::Item_port() {
	Item_vec.resize(ITEM_MAX);
	index = 0;
};
Item_port::~Item_port() {
	Item_vec.clear();
};
