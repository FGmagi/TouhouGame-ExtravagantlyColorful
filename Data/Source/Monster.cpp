#include <stdafx.h>
#include <Monster.h>
#include <Sprite.h>
#include <Actor.h>
#include <Bullet.h>
#define CIRCLE_W 128
#define CIRCLE_H 128

Monster::Monster(int id) {
	sprite.reset(new Sprite(*consts.image_enemy,0,0 ,MONSTERS_Z));
	if (id) {
		set_kind(id);
	}
}
void Monster::update() {
	if (active) {
		r += v;
		v += a;
		if (call_back) {
			call_back(this);
		}

		if (v.x > 0.00001f) {
			if (fx >= FREQUENCY * 8) {
				if (fx < FREQUENCY * 12 - 1)
					fx++;
			} else {
				fx = FREQUENCY * 8;
			}
		} else if (v.x < -0.00001f) {
			if (fx >= FREQUENCY * 4 && fx < FREQUENCY * 8) {
				if (fx < FREQUENCY * 8 - 1)
					fx++;
			} else {
				fx = FREQUENCY * 4;
			}
		} else {
			if (fx >= FREQUENCY * 0 && fx < FREQUENCY * 4) {
				if (fx < FREQUENCY * 4 - 1)
					fx++;
			} else {
				fx = FREQUENCY * 0;
			}
		}
		auto len = sprite->get_src();
		sprite->x = static_cast<int>(round(r.x - len.w * 0.5f));
		sprite->y = static_cast<int>(round(r.y - len.h * 0.5f));
		if (circle) {
			circle->angle(3.1415926535f / 180.0f * 2);
			if (boss) {
				circle->x = r.x - 256 / 2;
				circle->y = r.y - 256 / 2;
			} else {
				circle->x = r.x - CIRCLE_W / 2;
				circle->y = r.y - CIRCLE_H / 2;
			}
		}
		if (boss) {
			static int boss_direction = 4;
			if (v.x > 0.00001f)
				boss_direction = 6;
			if (v.x < -0.00001f)
				boss_direction = 4;

			if (boss_direction == 6)
				sprite->src({ 192, 320, 192, 192 });
			else
				sprite->src({ 0, 320, 192, 192 });
		} else {
			sprite->src({ fx / FREQUENCY * 64, color * 64, 64, 64 });
		}times++;
	}
}
void Monster::set_kind(int id) {
	if (id == 12) boss = true;
	Doc* dom = &*consts.enemy_data;
	if (auto f = dom->FindMember(std::to_string(id).c_str()); f != dom->MemberEnd() && f->value.IsObject()) {
		auto& da = f->value;
		if (auto g = da.FindMember("range"); g != da.MemberEnd() && g->value.IsArray()) {
			range = { float(g->value[0].GetDouble()),float(g->value[1].GetDouble()) };
		} else {
			range = { 32.0f,32.0f };
		}
		if (auto g = da.FindMember("hp"); g != da.MemberEnd() && g->value.IsInt()) {
			hp = g->value.GetInt();
		} else {
			hp = 100;
		}
		if (auto g = da.FindMember("color"); g != da.MemberEnd() && g->value.IsString()) {
			static std::map<std::string, int> color_map = {
				{"Bule",0},
				{"Red",1},
				{"Green",2},
				{"Yellow", 3},
				{"Blank", 4}
			};
			color = color_map[g->value.GetString()];
		} else {
			color = 0;
		}
		if (auto g = da.FindMember("mark"); g != da.MemberEnd() && g->value.IsInt()) {
			mark = g->value.GetInt();
		} else {
			mark = 0;
		}
		if (auto g = da.FindMember("range"); g != da.MemberEnd() && g->value.IsArray()) {
			range = { float(g->value[0].GetDouble()),float(g->value[1].GetDouble()) };
		} else {
			range = { 32.0f,32.0f };
		}
		if (auto g = da.FindMember("circle"); g != da.MemberEnd() && g->value.IsInt()) {
			int i = g->value.GetInt();
			if (i == 1) {
				circle.reset(new Sprite(*consts.image_bullet2, int(r.x - CIRCLE_W / 2), int(r.y - CIRCLE_H / 2), MONSTERS_CIRCLE_Z));
				circle->src({ 0,256,128,128 });
			}
			if (i == 2) {
				circle.reset(new Sprite(*consts.image_bullet2, int(r.x - CIRCLE_W / 2), int(r.y - CIRCLE_H / 2), MONSTERS_CIRCLE_Z)); \
					circle->src({ 128,256,128,128 });
				circle->zoom(0.7f);
			}
			if (i == 3) {
				circle.reset(new Sprite(*consts.image_bullet2, int(r.x - CIRCLE_W / 2), int(r.y - CIRCLE_H / 2), MONSTERS_CIRCLE_Z)); \
					circle->src({ 0,384,256,256 });
				circle->zoom(1.2f);
			}
		}
		if (auto g = da.FindMember("items"); g != da.MemberEnd() && g->value.IsArray()) {
			for (int i = 0; i < g->value.Size(); i++) {
				fall_items.push_back(g->value[i].GetInt());
			}
		}
		
	}
	sprite->src({ 0, color * 64, 64, 64 });
}