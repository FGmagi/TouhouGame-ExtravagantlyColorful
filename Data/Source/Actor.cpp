#include <stdafx.h>
#include <Actor.h>
#include <Input.h>
#include <Bullet.h>
#include <Complex.h>
#include <Monster.h>
#include <Thief.h>
#include <Game_Map.h>
#include <Music.h>
#include <Sprite_Map.h>
#include <Item.h>

#define GLORY_W 128
#define GLORY_H 128
extern int miss_times;
extern int boom_times;
Actor::Actor() {
	actor_bullet_port = new Actor_Bullet_port;
	glory->src({ 320,352,128,128 });
	glory->opacity=0;
	sprite->src({ int(x)-ACTOR_W/2,int(y)-ACTOR_H/2,ACTOR_W,ACTOR_H });
	Bitmap bitmap(844, 900);
	miss_times = 0;
	boom_times = 0;
	update();
}
Actor::~Actor() {
	delete actor_bullet_port;
}
template<int W,int H>
inline void demo(Sprite& sprite,float& px, float& py,int tx,int ty, int tw, int th) {
	//判定范围 x:26-38 y:40-52
	px = min(px, RIGHT_EDGE - 10);
	px = max(px, LEFT_EDGE + 10);
	py = min(py, DOWN_EDGE - 10);
	py = max(py, UP_EDGE + H/2 - 5);

	sprite.x = int(round(px)) - W / 2;
	sprite.y = int(round(py)) - H / 2;
	sprite.src({ tx, ty,tw,th });
};
THIEF_TEMPLATE(get_shader_, Sprite, S_Ptr<Shader>, shader_);
static int miss_times = 0;
extern std::vector<Item> Item_vec;
void Actor::miss() {
	if (miss_times == 0) { 
		Music::SE(7);
		get_spell = false;
		immortal = 999;
		miss_times = 1;
		Game_data::hp = max(Game_data::hp-1,0);
		Game_data::sp = 3;
		Game_data::pour = max(0, Game_data::pour-12);
		for (int i = 0; i < ITEM_MAX; i++) {
			if (Item_vec[i].active == true) {
				Item_vec[i].follow = false;
				Item_vec[i].fx = 0;
				Item_vec[i].fy = 2.70f;
			}
		}
		std::vector<int> fall = { 2,2,2,4 };
		if (Game_data::hp <= 1) {
			fall = {5};
		}
		for (auto j : fall) {
			int tx = (rand() % 128) - 64; tx = min(max(tx, 0), RIGHT_EDGE);
			int ty = (rand() % 128) - 64; ty = min(max(ty, 0), DOWN_EDGE);
			auto it = Item::Push(j,x + tx, y + ty);
			if (y >= 300)
				it->fy = -12.0f;
			if (y >= 750)
				it->fy = -13.5;
		}
		Mini_Animation(2, 0, x, y);
	};
}
void TEMP_bullet_update(Actor_Bullet_port* actor_bullet_port) {
	auto& kkk = actor_bullet_port->actor_bullet_vec;
	for (int i = 0; i < Bullet_MAX; i++) {
		kkk[i].update();
	}
}
int boom_times = 0;
extern int mon_size;
extern std::vector<S_Ptr<Monster>> monsters;

void map_dange(int n) {
	for (auto i = 0; i < mon_size; i++) {
		if (monsters[i] && monsters[i]->range.x > 0) {
			if (monsters[i]->r.x < RIGHT_EDGE && monsters[i]->r.x > LEFT_EDGE && monsters[i]->r.y > UP_EDGE && monsters[i]->r.y < DOWN_EDGE) {
				if (monsters[i]->boss) {
					if (monsters[i]->immortal) {
						monsters[i]->hp -= n / 20000;
						continue;
					}
				} else {
					if (monsters[i]->immortal) {
						monsters[i]->hp -= n / 5;
						continue;
					}
					if (monsters[i]->circle) {
						monsters[i]->hp -= n * 0.8f;
						continue;
					}
					monsters[i]->hp -= n;
				}
			}
		}
	}
}
void Actor::update() {
	#define During(begin,last) (boom_times >= (begin) && boom_times < (begin+last))
	if (boom_times) {
		if (boom_times == 1) {
			//Music::SE(12);
			boom_effect[0] = std::make_shared<Sprite>(*consts.image_boom_effect, x - 192, y - 192, 15);
			Bitmap bitmap(844, 900);
			bitmap.fill_rect({ 76, 158, 212, 255 }, { 0,0,844,900 });
			boom_effect[1] = std::make_shared<Sprite>(bitmap,29, 29, 98);
			Bitmap bitmap2(844,900);
			bitmap2.fill_rect({ 255, 255, 255, 200 }, { 0,0,844,900 });
			boom_effect[2] = std::make_shared<Sprite>(bitmap2, 29, 29, 99);
			boom_effect[0]->opacity = 0;
			boom_effect[1]->opacity = 0;
			boom_effect[2]->opacity = 0;
		}
		if (During(1,51)) {
			boom_effect[0]->opacity = min(boom_effect[0]->opacity + 6, 255);
			boom_effect[1]->opacity = min(boom_effect[1]->opacity + 1, 255);
		}
		if (During(120,20)) {
			boom_effect[2]->opacity += 8;
		}
		if (boom_times == 120) {
			map_dange(spell_danger / 2);
		}
		if (During(140, 40)) {
			boom_effect[2]->opacity -= 4;
		}
		if (boom_times == 140) {
			Bullet::clear();
			Music::SE(13);
		}
		if (During(185, 20)) {
			boom_effect[2]->opacity += 8;
		}
		if (boom_times == 185) {
			map_dange(spell_danger / 2);
		}
		if (During(205, 40)) {
			boom_effect[2]->opacity -= 4;
		}
		if (boom_times == 205) {
			Bullet::clear();
			Music::SE(13);
		}
		if (During(220, 50)) {
			boom_effect[0]->opacity = max(boom_effect[0]->opacity - 6, 0);
			boom_effect[1]->opacity = max(boom_effect[1]->opacity - 1, 0);
		}
		for (int i = 0; i < ITEM_MAX; i++) {
			if (Item_vec[i].active == true) {
				Item_vec[i].follow = true;
			}
		}
		boom_effect[0]->y -= 2;
		boom_effect[0]->angle(PI/180);
		boom_times++;
		if (During(270, 1)) {
			if (!(monsters[0] && monsters[0]->boss)) {
				Bullet::clear();
			}
			boom_effect[0].reset();
			boom_effect[1].reset();
			boom_effect[2].reset();
			boom_times=0;
		}
	}
	if (miss_times) {
		key_active = false;
		for (int i = 0; i < ITEM_MAX; i++) {
			if (Item_vec[i].active == true) {
				Item_vec[i].follow = false;
			}
		}
		miss_times++;
		if (miss_times <= CHU_SHI_TIMES) {
			sprite->opacity = max(0, sprite->opacity - 255.0 / CHU_SHI_TIMES);
			glory->opacity = 0;
			goto flag00;
		} 
		if (miss_times == 30 + CHU_SHI_TIMES) {
			x = 451.0f;
			y = 940.0f;
			sprite->opacity = 255;
			glory->opacity = 0;
			sprite->x = x - ACTOR_W / 2;
			sprite->y = y - ACTOR_H / 2;
			goto flag00;
		}
		if (miss_times > 30 + CHU_SHI_TIMES && miss_times <= 40 + 30 + CHU_SHI_TIMES) {
			y -= 3;
			sprite->x = x - ACTOR_W / 2;
			sprite->y = y - ACTOR_H / 2;
			goto flag00;
		}
		if (miss_times == 41 + 30 + CHU_SHI_TIMES) {
			crash_time = 0;
			immortal = 180;
			goto flag00;
		} 
		if (miss_times >= 56 + 30 + CHU_SHI_TIMES) {
			miss_times = 0;
			key_active = true;
			goto flag00;
		}
	}
	flag00:
	if (active) {
		if (immortal) {
			if (get_shader_(*sprite) == NULL || *get_shader_(*sprite) == Shader::code[0]) {
				Shader t_shader("A0-基础着色器", "B2-闪烁着色器");
				sprite->shader(t_shader);
			}
			get_shader_(*sprite)->Bind();
			float s = 0.0f;
			if (immortal % 10 >= 9) {
				s = 1.0f;
			}
			Uniform(4f, u_color, 1.0f, 0.00f,0.00f,s);
			Uniform(4f, u_range, 0.0f,0.0f, 1.0f,1.0f);
			immortal--;
			if (!immortal) {
				get_shader_(*sprite).reset();
			}
		}
		//移动
			if (Input::press(R_SHIFT) && key_active) {
				if (slow >= 0 && slow <= 255) {
					slow = min(slow + 28, 255);
				} else {
					slow = -slow;
				}
			} else {
				if (slow >= 0) {
					slow = -slow;
				} else {
					slow = min(slow + 28, 0);
				}
			}

			speed = (slow > 0) ? sp_low : sp_fast;

			#define f_l	if (fy != 1) {fy = 1;fx = 0;} else {if (fx >= FREQUENCY * 8-1)	fx = FREQUENCY * 4;else	fx += (fx > FREQUENCY * 4) ? 1 : 2;}
			#define f_r if (fy != 2) {fy = 2;fx = 0;} else {if (fx >= FREQUENCY * 8-1)	fx = FREQUENCY * 4;else	fx += (fx > FREQUENCY * 4) ? 1 : 2;}
			#define f_m if (fy != 0) {fy = 0;fx = 0;} else {if (fx >= FREQUENCY * 8-1)	fx = FREQUENCY * 4;else	fx += (fx > FREQUENCY * 4) ? 1 : 2;}
			int direction = 5;
			int incline = false;
			if (key_active) {
				if (Input::press(R_LEFT)) {
					direction = 4;
					f_l;
				} else
					if (Input::press(R_RIGHT)) {
						direction = 6;
						f_r;
					} else {
						f_m;
					}
				if (Input::press(R_DOWN)) {
					if (direction == 4) {
						incline = true;
						direction = 1;
					} else if (direction == 6) {
						incline = true;
						direction = 3;
					} else {
						direction = 2;
					}
				}
				if (Input::press(R_UP)) {
					if (direction == 4) {
						incline = true;
						direction = 7;
					} else if (direction == 6) {
						incline = true;
						direction = 9;
					} else {
						direction = 8;
					}
				}
			}else{
				direction = 5;
				f_m;
			}
			//if (fx <= FREQUENCY*1) 
				//speed *= 0.5f + 0.5f * fx / FREQUENCY / 1;
			recode_pos = { x,y };
			switch (direction) {
				case 1:
					x -= speed * 0.707106f;
					y += speed * 0.707106f;
					break;
				case 2:
					y += speed;
					break;
				case 3:
					x += speed * 0.707106f;
					y += speed * 0.707106f;
					break;
				case 4:
					x -= speed;
					break;
				case 5:
					break;
				case 6:
					x += speed;
					break;
				case 7:
					x -= speed * 0.707106f;
					y -= speed * 0.707106f;
					break;
				case 8:
					y -= speed;
					break;
				case 9:
					x += speed * 0.707106f;
					y -= speed * 0.707106f;
			}
		recode_pos = { (recode_pos.x + x) * 0.5f,(recode_pos.y + y) * 0.5f };
		//判定范围 x:26-38 y:40-52
		int tx = fx / FREQUENCY * 64, ty = fy * 96, tw = ACTOR_W, th = ACTOR_H;
		//虽然这里不合规没更新顶点，但因为紧接着src所以没关系，下同
		demo<ACTOR_W, ACTOR_H>(*sprite, x, y, tx, ty, tw, th);
		if (slow) {
			glory->opacity = abs(slow);
			glory->angle(3.1415926535f / 180.0f * 2);

			glory->x = x - GLORY_W / 2;
			glory->y = y - GLORY_H / 2;

			glory->active = true;
		} else {
			glory->active = false;
		}

		//Boom
		if (!(immortal || boom_times || miss_times)) {
			boom = Input::input(R_X) && Game_data::sp;
			if (boom) {
				use_boom();
				boom = false;
			}
		}
		//攻击
		atk = Input::press(R_Z) && key_active;
		if (atk_times) {
			atk_times--;
		}
		if (atk && !atk_times && !boom_times) {
			Music::SE(11);
			atk_times = ATK_SPACE;
			missile();
		}
		/*if( Input::input(R_F7) && key_active){//测试后门
			monsters[0]->hp-=(100000);
		}*/
	}
	//弹幕位置刷新
	TEMP_bullet_update(actor_bullet_port);
}
void Actor::missile() {//自机发弹
	#define take(a,b,c) {auto* bullet = actor_bullet_port->push();bullet->r = {x + a, y + b};bullet->danger = c;bullet->sprite->src({ 292, 352, 24, 75 });bullet->sprite->opacity = 255;}
	#define take2(a,b,c) {auto* bullet = actor_bullet_port->push();bullet->r = {x + a, y + b};bullet->danger = c;bullet->follow = true;bullet->sprite->src({291,352,24,26});bullet->sprite->opacity = 150;}
	if (Game_data::pour == 128) {
		take(-15, 0, 120);
		take(15, 0, 120);
		take2(-35, 32, 40);
		take2(35, 32, 40);
		return;
	}
	if (Game_data::pour >= 110) {
		take(-15, 0, 100);
		take(15, 0, 100);
		take2(-35, 32, 40);
		take2(35, 32, 40);
		return;
	}
	if (Game_data::pour >= 64) {
		take(-15, 0, 60);
		take(15, 0, 60);
		take2(-35, 32, 20);
		take2(35, 32, 20);
		return;
	}
	if (Game_data::pour >= 32) {
		take(-15, 0, 40);
		take(15, 0, 40);
		take2(-35, 32, 10);
		take2(35, 32, 10);
		return;
	}
	if (Game_data::pour >= 0) {
		take(-15, 0, 40);
		take(15, 0, 40);
		return;
	}
};
void Actor::use_boom() {
	crash_time = 0;
	immortal = 330;
	boom_times = 1;
	Game_data::sp--;
	get_spell = false;
}

Actor_Bullet::Actor_Bullet() {
	active = false;
	follow = false;
	danger = 0;
	r = { 0.0f,0.0f };
	v = { 0.0f,0.0f };
	range = { 12, 37.5 };
	sprite.reset(new Sprite(*consts.image_bullet, 0, 0, MY_Bullet_Z));
	sprite->src({ 292, 352, 24, 75 });
	sprite->active = false;
}
#define V 25.0f
void Actor_Bullet::ini() {
	r = { 0.0f,0.0f };
	v = { 0.0f,-V };
	active = true;
	follow = false;
	danger = 0;
	sprite->x = r.x-ACTOR_W/2;
	sprite->y = r.y-ACTOR_H/2;
	sprite->active = true;
}
void Actor_Bullet::del() {
	active = false;
	sprite->active = false;
}

extern int mon_size;
extern std::vector<S_Ptr<Monster>> monsters;
#define pow(a) ((a)*(a))
void Actor_Bullet::update() {
	if (active) {
		if (follow) {
			for (int i = mon_size - 1; i >= 0; i--) {
				if (monsters[i] && monsters[i]->range.x > 0) {
					if (monsters[i]->r.x < RIGHT_EDGE && monsters[i]->r.x > LEFT_EDGE && monsters[i]->r.y > UP_EDGE && monsters[i]->r.y < DOWN_EDGE) {
						Monster& enemy = *monsters[i];
						float ax = enemy.r.x - r.x;
						float ay = enemy.r.y - r.y;
						float distance = sqrt(pow(ax) + pow(ay));
						v.x = (ax / distance) * V;
						v.y = (ay / distance) * V;
						break;
					}
				}
			}
		}
		r += v;

		if (call_back) {
			call_back(this);
		}
		//更新顶点
		auto len = sprite->get_src();
		sprite->x = static_cast<int>(round(r.x - len.w * 0.5f));
		sprite->y = static_cast<int>(round(r.y - len.h * 0.5f));
		if (r.x < -100 || r.x > HEIGHT + 100 || r.y < -100 || r.y > HEIGHT + 100) {
			del();
		}
	}
}

Actor_Bullet_port::Actor_Bullet_port() {
	sprite_map.reset(new Sprite_Map(MY_Bullet_Z));
	actor_bullet_vec.resize(Bullet_MAX);
	for (int i = 0; i < Bullet_MAX; i++) {
		sprite_map->push(&(*actor_bullet_vec[i].sprite));
	}
};
Actor_Bullet* Actor_Bullet_port::push() {
	for (int i = index; i < Bullet_MAX; i++) {
		if (!actor_bullet_vec[i].active) {
			index = i;
			actor_bullet_vec[i].ini();
			return &actor_bullet_vec[i];
		}
	}
	for (int i = 0; i < index; i++) {
		if (!actor_bullet_vec[i].active) {
			index = i;
			actor_bullet_vec[i].ini();
			return &actor_bullet_vec[i];
		}
	}
	return NULL;

} 
void Actor::Global_Actor_Bullet_Collision(){
	auto& kkk = actor_bullet_port->actor_bullet_vec;
	for (int i = 0; i < Bullet_MAX; i++) {
		kkk[i].check_crash();
	}
}