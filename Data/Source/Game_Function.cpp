//============================================================
//	头文件引用
//============================================================
#include <stdafx.h>
#include <Game_Function.h>
#include <Actor.h>
#include <Monster.h>
#include <Bullet.h>
#include <Game_Map.h>
#include <cmath>
extern S_Ptr<Actor> actor;
Monster& Push(int id);

#define Rank (Game_data::rank)
#define HARD (Game_data::degree)
#define E_normal(kkk) {distance = Euclidean({dir_dx,dir_dy});float nv = (kkk) / distance;dir_vx = dir_dx * nv;dir_vy = dir_dy * nv;}; 
#define M_normal(kkk) {distance = Manhattan({dir_dx,dir_dy});float nv = (kkk) / distance;dir_vx = dir_dx * nv;dir_vy = dir_dy * nv;}; 
#define DEG(i) ((i)*3.1415926535/180)
void missile_con(Bullet* temp, float v1, float a) {
	const float distance = Euclidean(temp->v);
	if (distance > v1) {
		temp->v.x -= a * temp->v.x / distance;
		temp->v.y -= a * temp->v.y / distance;
	}
}
void missile_con2(Bullet* temp, float v1, float a) {
	const float distance = Euclidean(temp->v);
	if (distance < v1) {
		temp->v.x += a * temp->v.x / distance;
		temp->v.y += a * temp->v.y / distance;
	}
}
//============================================================
//	发弹装置 参数：ID 发弹模式，r 发弹位置，par 参数列表，
//============================================================
//增减速度的模
void missile(int id, vec2 r,std::vector<float> par) {//敌机发弹
	if (r.x < LEFT_EDGE || r.x > RIGHT_EDGE || r.y < UP_EDGE || r.y >DOWN_EDGE) {
		return;
	}
	Actor& plane = *actor;

	int en_x = r.x;
	int en_y = r.y;
	float dir_dx = (actor->x - r.x);
	float dir_dy = (actor->y - r.y);
	float dir_vx = 0;
	float dir_vy = 0;
	float dir_fx = 0;
	float dir_fy = 0;
	float distance = 0;
	Bullet* temp = NULL;
	int color = -1;
	switch (id) {
		case 0: {//自机狙，参数：【初速度，颜色，[末速度,加速度]】
			E_normal(par[0]);
			temp = Bullet::push(int(round(par[1])), en_x, en_y, dir_vx, dir_vy);
			if (par.size() >= 4) {
				float a = par[2], b = par[3];
				temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };
			}
			break;
		}
		case 1: {//三棱 散弹 参数：【速度，末速度,加速度，角度列表...】
			E_normal(par[0]);
			Bullet::push(1, en_x, en_y, dir_vx, dir_vy);
			for (int i = 3; i < par.size(); i++) {
				float a, b;
				temp = Bullet::push(1, en_x, en_y, dir_vx, dir_vy);
				change(temp->v, sin(par[i]), cos(par[i]));
				a = par[1], b = par[2];
				temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };

				temp = Bullet::push(1, en_x, en_y, dir_vx, dir_vy);
				change(temp->v, -sin(par[i]), cos(par[i]));
				a = par[1], b = par[2];
				temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };
			}
			break;
		}
		case 2: {//周角弹 参数：【初始速度，密度(0.1f-1.0f)，[末速度，加速度]】
			dir_vx = par[0]; dir_vy = 0;
			par[1] = min(max(0.1f, par[1]), 1.0f);
			for (float i = 0; i < 2 * PI; i += par[1]) {
				temp = Bullet::push(1, en_x, en_y, dir_vx, dir_vy);
				change(temp->v, sin(i), cos(i));
				if (par.size() >= 3) {
					float a = par[2], b = par[3];
					temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };
				}
			}
			break;
		}
		case 3: {//圆锥 参数：【速度，数量】
			float theta;
			M_normal(par[0]);
			if (dir_vx)
				theta = atan(dir_vy / dir_vx);
			else
				theta = PI / 2;
			int tx = 0;
			int num = round(par[1]);
			int ty = -(15 * PI / 180) * num;//ty每次增量为2被的初始值
			for (int i = 0; i <= num; i++) {
				temp = Bullet::push(1, en_x, en_y, tx, ty);
				temp->a.x = dir_vx; temp->a.y = dir_vy;
				if (theta > 0) {
					change(temp->v, sin(theta), cos(theta));
				} else {
					change(temp->v, -sin(-theta), cos(-theta));
					ty += 30 * PI / 180;
				}
			}
			break;
		}
		case 4: {//双重直线【速度，间距】
			//E_normal(par[0]);
			Bullet::push(2, en_x - par[1], en_y, 0.0f, par[0]);
			Bullet::push(2, en_x + par[1], en_y, 0.0f, par[0]);
			break;
		}
		case 5: {//半周角弹 参数：【初始速度，密度(0.1f-1.0f)，区间 [m,M]】
			dir_vx = par[0]; dir_vy = 0;
			par[1] = min(max(0.1f, par[1]), 1.0f);
			for (float i = par[2]; i <= par[3] + 0.001f; i += par[1]) {
				temp = Bullet::push(2, en_x, en_y, dir_vx, dir_vy);
				change(temp->v, -sin(i), -cos(i));
			}
			break;
		}
		case 6: {//周角均匀弹 参数：【初始速度，起始位置，弹道数量（正负表示顺逆），颜色】
			dir_vx = par[0]; dir_vy = 0;
			if (par[2] > 0) {
				for (float i = par[1]; i < 2 * PI + par[1]; i += 2 * PI / par[2]) {
					temp = Bullet::push(par[3], en_x, en_y, dir_vx, dir_vy);
					change(temp->v, -sin(i), -cos(i));
				}
			} else {
				for (float i = par[1]; i > -2 * PI + par[1]; i += 2 * PI / par[2]) {
					temp = Bullet::push(par[3], en_x, en_y, dir_vx, dir_vy);
					change(temp->v, -sin(i), -cos(i));
				}
			}
			break;
		}
		case 9: {//周角弹 随机 参数：【初始速度，密度(0.1f-1.0f)，[末速度，加速度]】
			dir_vx = par[0]; dir_vy = 0;
//			par[1] = min(max(0.1f, par[1]), 1.0f);
			float t = (rand() % 90) * PI / 180;
			for (float i = t; i < 2 * PI + t; i += par[1]) {
				temp = Bullet::push(1, en_x, en_y, dir_vx, dir_vy);
				change(temp->v, sin(i), cos(i));
				if (par.size() >= 3) {
					float a = par[2], b = par[3];
					temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };
				}
			}
			break;
		}
		case 10: {//螺旋弹 参数：【颜色，初始半径，切向速度，径向速度,静止时间，密度】
			for (float i = 0 * PI / 180; i < 360 * PI / 180; i += par[5] * PI / 180) {
				float R0 = par[1];
				float theta0 = i;
				float Vt = par[2];
				float Vr = par[3];
				float T0 = par[4];
				vec2 r = { float(R0 * cos(theta0) + en_x),float(R0 * sin(theta0) + en_y) };
				temp = Bullet::push(par[0], r.x, r.y);
				temp->call_back = [en_x, en_y, T0, Vr, Vt, R0, theta0](Bullet* ptr) {
					double R = R0, theta = theta0;
					if (ptr->times >= T0) {
						int t = ptr->times - T0;
						R += Vr * t;
						theta += Vt / Vr * log(Vr * t / R0 + 1);
					}
					ptr->r = { float(R * cos(theta) + en_x),float(R * sin(theta) + en_y) };
				};
			}
			break;
		}
		case 11: {//螺旋弹2 参数：【颜色，初始半径，角速度，径向速度,静止时间，密度】
			for (float i = 0 * PI / 180; i < 360 * PI / 180; i += par[5] * PI / 180) {
				temp = Bullet::push(par[0]);
				float R0 = par[1];
				float theta0 = i;
				float omega = par[2];
				float Vr = par[3];
				float T0 = par[4];
				temp->call_back = [en_x, en_y, T0, Vr, omega, R0, theta0](Bullet* ptr) {
					double R = R0, theta = theta0;
					if (ptr->times >= T0) {
						int t = ptr->times - T0;
						R += Vr * t;
						theta += omega * t;
					}
					ptr->r = { float(R * cos(theta) + en_x),float(R * sin(theta) + en_y) };
				};
			}
			break;
		}
		case 12: {//周角弹 参数：【起始，初始速度，密度(0.1f-1.0f)，[末速度，加速度],ID】
			dir_vx = par[1]; dir_vy = 0;
			float t = par[0];
			if (par[4] > 0) {
				for (float i = t; i < 2 * PI + t; i += par[2] + 0.000001f) {
					temp = Bullet::push(int(round(par[5])), en_x, en_y, dir_vx, dir_vy);
					change(temp->v, sin(i), cos(i));
					if (par.size() >= 3) {
						float a = par[3], b = par[4];
						temp->call_back = [a, b](Bullet* ptr) {missile_con2(ptr, a, b); };
					}
				}
			} else {
				for (float i = t; i < 2 * PI + t; i += par[2] + 0.000001f) {
					temp = Bullet::push(int(round(par[5])), en_x, en_y, dir_vx, dir_vy);
					change(temp->v, sin(i), cos(i));
					if (par.size() >= 3) {
						float a = par[3], b = -par[4];
						temp->call_back = [a, b](Bullet* ptr) {missile_con(ptr, a, b); };
					}
				}
			}
			break;
		}
		case 13: {//开花弹 参数【速度x,y，散弹等级】
			dir_vx = par[0]; dir_vy = par[1];
			float n = par[2];
			temp = Bullet::push(8, en_x, en_y, dir_vx, dir_vy);
			temp->call_back = [dir_vx, dir_vy, n](Bullet* ptr) {
				if (ptr->times >= 40 && ptr->times < 80) {
					ptr->v.x = dir_vx * (1.0f - sin(PI / 2 * (ptr->times - 40) / 40.0f));
					ptr->v.y = dir_vy * (1.0f - sin(PI / 2 * (ptr->times - 40) / 40.0f));
				}
				if (ptr->times == 80) {
					if (n == 0) {
						missile(12, ptr->r, { 0, 6.0f,30 * PI / 180,3.50f,-0.03f,6 });
					} else {
						for (int i = -30; i < 330; i += 60) {
							vec2 v = { 3.0f * cos(i * PI / 180),3.0f * sin(i * PI / 180) };
							missile(14, ptr->r, { v.x,v.y});

						}
					}
					ptr->del();
				}
			};
			break;
		}
		case 14: {//开花弹 参数【速度x,y，散弹等级】
			dir_vx = par[0]; dir_vy = par[1];
			temp = Bullet::push(12, en_x, en_y, dir_vx, dir_vy);
			temp->call_back = [dir_vx, dir_vy](Bullet* ptr) {
				if (ptr->times >= 40 && ptr->times < 80) {
					ptr->v.x = dir_vx * (1.0f - sin(PI / 2 * (ptr->times - 40) / 40.0f));
					ptr->v.y = dir_vy * (1.0f - sin(PI / 2 * (ptr->times - 40) / 40.0f));
				}
				if (ptr->times == 80) {
					missile(12, ptr->r, { 0, 3.0f,30 * PI / 180,1.50f,-0.05f,6 });
					ptr->del();
				}
			};
			break;
		}

		case 15: {//旋涡樱花 参数【次数,方向】
			float R = 20 * par[0], theta, theta2;
			for (int i = 0; i < 20; i++) {
				if (par[1] > 0) {
					theta = (i / 20.0f) * 2 * PI - 1.5f * par[0] * PI / 180;
					theta2 = theta - 1.50f * PI / 180;
				} else {
					theta = (i / 20.0f) * 2 * PI + 1.5f * par[0] * PI / 180;
					theta2 = theta + 1.50f * PI / 180;
				}
				if (HARD <= 2) {
					temp = Bullet::push(13, en_x + R * cos(theta), en_y - R * sin(theta));
				} else {
					temp = Bullet::push(14, en_x + R * cos(theta), en_y - R * sin(theta));
				}
				temp->sprite->angle(theta2 - 90 * PI / 180);
				dir_vx = 3.20f * cos(theta2);
				dir_vy = -3.20f * sin(theta2);
				float t = 12 * par[0];
				temp->call_back = [dir_vx, dir_vy, t](Bullet* ptr) {
					if (ptr->times >= 100 + t && ptr->times < 160 + t) {
						ptr->v.x += dir_vx / 60;
						ptr->v.y += dir_vy / 60;
					}
				};
			}
			break;
		}
		case 16: {//直降樱花 参数【速度】
			temp = Bullet::push(15, en_x, -3, par[0], par[1]);
			break;
		}
		case 17: {//旋转半狙樱花 参数【R,v,n】
			if (par[2] > 0) {
				float R = par[0], theta = 0;
				distance = Euclidean({ dir_dx, dir_dy });
				dir_vx = par[1] * (dir_dx - R * cos(theta)) / distance;
				dir_vy = par[1] * (dir_dy - R * sin(theta)) / distance;
				vec2 v = { dir_vx,dir_vy };
				change(v, 1.0f, 0.0f);
				for (; theta < 2 * PI; theta += 30 * PI / 180 + 0.00001f) {
					vec2 v2 = v;
					change(v2, sin(theta - 90 * PI / 180), cos(theta - 90 * PI / 180));
					temp = Bullet::push(16, en_x + R * cos(theta), en_y + R * sin(theta), v2.x, v2.y);
				}
			} else {
				float R = par[0], theta = -0.75 * PI;
				distance = Euclidean({ dir_dx, dir_dy });
				dir_vx = par[1] * (dir_dx - R * cos(theta)) / distance;
				dir_vy = par[1] * (dir_dy - R * sin(theta)) / distance;
				vec2 v = { dir_vx,dir_vy };
				change(v, sin(0.75f * PI), cos(0.75f * PI));
				for (; theta < 1.25 * PI; theta += 30 * PI / 180 + 0.00001f) {
					vec2 v2 = v;
					change(v2, sin(theta + 90 * PI / 180), cos(theta + 90 * PI / 180));
					temp = Bullet::push(16, en_x + R * cos(theta), en_y + R * sin(theta), v2.x, v2.y);
				}
			}
			break;
		}
		case 18: {//圆周樱花小 参数【v】
			for (float theta = 0; theta < 2 * PI; theta += 5 * PI / 180 + 0.00001f) {
				vec2 v = { par[0] * cos(theta),par[0] * sin(theta) };
				temp = Bullet::push(1, en_x, en_y, v.x, v.y);
			}
			break;
		}
		case 19: {//传说中的光玉 参数【v】
			int be = rand() % 360;
			for (float theta = be*PI/180; theta < (360+be) * PI / 180; theta += 30 * PI / 180 + 0.00001f) {
				vec2 v = { par[0] * cos(theta),par[0] * sin(theta) };
				temp = Bullet::push(17, en_x, en_y, v.x, v.y);
			}
			break;
		}
	}
	if (color < 0 && temp) {
		color = (temp->color);
	}
	if (color > 0) {
		Mini_Animation mini(0, color, en_x, en_y);
	}
};
//============================================================
//	关卡规划
//============================================================
//设置初始坐标，窗口(64,-64)-(832,896) 转换后为 0 - 768 (64*12)
#define POS(ar,tx,ty) ar.r.x = tx+64+32;ar.r.y=ty-64+32; 
//原边框（29，29，873，930） 
#define POS2(ar,tx,ty) ar.r.x = tx;ar.r.y=ty;

#define During(a) (ptr->times == (a))
#define During(a,b) (ptr->times >= (a) && ptr->times < (b))
template<>
void locus<1>(int x,int y,int i,int enemy_id) {
	Monster& mon = Push(enemy_id);
	POS(mon, x, y);
	mon.v = { 0.0f,2.0f };
	mon.a = { 0.0f,0.1f };
	game_variables[0] += 1;
	int t = game_variables[0];
	mon.call_back = [enemy_id, i,t](Monster* ptr) {
		if (ptr->times == 30)
			ptr->a.y = -ptr->a.y;
		if (ptr->times > 30 && ptr->v.y < 0) {
			ptr->v.y = 0;
			ptr->a.y = 0;
		}
		if (ptr->times == 180) {
			ptr->v.x = i;
		}
		int kk = (HARD >= 3) ? 35 : 55;
		if (ptr->times == kk + t) {
			float v = Rank * 0.01f + 1.0f;;
			missile(2, ptr->r, { 8.0f, (4 - HARD) * 0.15f + 0.18f,v,0.05f });
		}
	};
}
template<>
void locus<2>(int x, int y,int enemy_id) {
	Monster& mon = Push(enemy_id);
	POS(mon, x, y);
	mon.v = { 0.0f,4.0f };
	mon.a = { 0.0f,0.0f };
	mon.call_back = [](Monster* ptr) {
		if (ptr->times == 29)
			ptr->a = {0.0f,-0.5f};
		if (ptr->v.y < 0) {
			ptr->v.y = 0;
			ptr->a.y = 0;
		}
		if (ptr->times >= 40 && ptr->times % (30 - HARD * 5) == 0 && ptr->times <= 240) {
			float v = Rank * 0.1f + 3.60f;
			if (HARD == 4) {
				missile(1, ptr->r, { HARD + 6.0f,v,0.05f,15.0f / 180.0f * PI,30.0f / 180.0f * PI,50.0f / 180.0f * PI });
			} else
			if (HARD == 3) {
				missile(1, ptr->r, { HARD + 6.0f,v,0.05f,15.0f / 180.0f * PI,30.0f / 180.0f * PI });
			} else 
			if (HARD == 2) {
				missile(1, ptr->r, { HARD + 4.0f,v - 2.0f,0.05f,15.0f / 180.0f * PI });
			} else {
				missile(1, ptr->r, { HARD + 4.0f,v - 2.0f,0.05f });
			}
		}
		if (ptr->times == 180) {
			ptr->v.y = 2.50f;
		}
	};
}

//坐标 xy，方向 dir 左1右2，编号 enemyid
template<>
void locus<3>(int x, int y,int dir, int enemy_id) {
	Monster& mon = Push(enemy_id);
	POS2(mon, x, y);
	mon.immortal = true;
	mon.call_back = [dir](Monster* ptr){
		if (ptr->times == 0) {
			if (dir == 1) ptr->v.x = 5.0f;
			if (dir == 2) ptr->v.x = -5.0f;
		}
		if (ptr->times == 25) {
			if (dir == 1) ptr->v.x = 0.0f;
			if (dir == 2) ptr->v.x = 0.0f;
		}
		if (ptr->times == 110 + HARD*10) {
			ptr->immortal = false;
		}
		if (During(30,500) && ptr->times % 15 == 0) {
			if (!(ptr->times > 240 && HARD == 1)) {
				missile(4, ptr->r, { 6.0f,24.0f });
			}
			if (HARD >= 3 && ptr->times % 30 == 0) {
				missile(5, ptr->r, { 4.50f,0.25f,-45 * PI / 180,225 * PI / 180 });
			}
		}
		if (ptr->times == 240) {
			if (dir == 1) ptr->v = { 3.50f,0.2f };
			if (dir == 2) ptr->v = { -3.50f,0.2f };
		}
	};
}

//坐标 xy，编号 enemyid
template<>
void locus<4>(int x, int y, int enemy_id) {
	Monster& mon = Push(enemy_id);
	mon.v = { 0.0f,5.0f };
	POS2(mon, x, y);
	mon.immortal = true;
	game_variables[0] = 0.0f;
	game_variables[1] = 0.0f;
	mon.call_back = [](Monster* ptr) {
		if (During(28, 38)) {
			ptr->v.y -= 0.5f;
		}
		if (ptr->times == 38) {
			ptr->v.y = 0.0f;
		}
		if (ptr->times == HARD * 20 + 120) {
			ptr->immortal = false;
			ptr->hp -= (4-HARD) * 250;
		}
		if (During(50, 1000) && ptr->times % (5 + pow(4-HARD)) == 0) {
			missile(6, ptr->r, { 5.0f,game_variables[0],4.0f,2.0f });
			missile(6, ptr->r, { 5.0f,-game_variables[0],-4.0f,2.0f });
			game_variables[0] += 12 * PI / 180;
		}
		if (ptr->times == 1000) {
			ptr->v.y = 4.0f;
		}
	};
}
//坐标 xy
template<>
void locus<5>(int x, int y,vec2 v) {
	Monster& mon = Push(7);
	//mon.immortal = true;
	mon.v = v;
	POS2(mon, x, y);
	mon.call_back = [v](Monster* ptr) {
		/*if (ptr->times == HARD * 10-10) {
			ptr->immortal = false;
		}*/
		if ((During(50, 120) || (HARD>=3 && ptr->times >=50)) && ptr->times % (30 - HARD * 5) == 0) {
			missile(0, ptr->r, { 5.0f,4 });
		}
		if (During(30, 40)) {
			ptr->v.x -= v.x / 10;
			ptr->v.y -= v.y / 10;
		}
		if (During(40, 41)) {
			ptr->v = { 0,0 };
		}
		if (v.y > 1) {
			if (During(170, 180)) {
				ptr->v.y += v.y / 10;
			}
		}else if (During(70, 80)) {
			ptr->v.x += v.x / 10;
		}
	};
}
//坐标 xy
template<>
void locus<6>(int x, int y) {
	Monster& mon = Push(6);
	mon.v = { 0.0f,5.0f };
	POS2(mon, x, y);
	game_variables[0] = (rand() % 360) * PI / 180;
	game_variables[1] = (rand() % 360) * PI / 180;
	game_variables[2] = (rand() % 360) * PI / 180;
	game_variables[3] = (rand() % 360) * PI / 180;
	mon.call_back = [](Monster* ptr) {
		if (During(28, 38)) {
			ptr->v.y -= 0.5f;
		}
		if (ptr->times == 38) {
			ptr->v.y = 0.0f;
		}

		if (During(50, 1000) && ptr->times % (20 + 10 * (4-HARD)) == 0) {
			vec2 tf;
			tf = { ptr->r.x - 64,ptr->r.y - 64 };
			missile(6, tf, { 5.0f,game_variables[0],3.50f,4.0f });
			game_variables[0] += 10 * PI / 180;
			tf = { ptr->r.x - 64,ptr->r.y + 64 };
			missile(6, tf, { 5.0f,game_variables[1],3.50f,4.0f });
			game_variables[1] += 10 * PI / 180;
			tf = { ptr->r.x + 64,ptr->r.y + 64 };
			missile(6, tf, { 5.0f,game_variables[2],3.50f,4.0f });
			game_variables[2] += 10 * PI / 180;
			tf = { ptr->r.x + 64,ptr->r.y - 64 };
			missile(6, tf, { 5.0f,game_variables[3],3.50f,4.0f });
			game_variables[3] += 10 * PI / 180;
		}

		if (ptr->times == 1000) {
			ptr->v.y = -1.20f;
		}
	};
}
//坐标 xy,数组开始位置 first
template<>
void locus<7>(int x, int y, int first) {
	Monster& mon = Push(6);
	if (first >= 8) {
		mon.v = { 0.0f,8.0f };
	} else {
		mon.v = { 0.0f,5.0f };
	}
	POS2(mon, x, y);
	mon.immortal = true;
	float v = mon.v.y;
	mon.call_back = [first,v](Monster* ptr) {
		game_variables[first] = (rand() % 360) * PI / 180;
		game_variables[first + 1] = (rand() % 360) * PI / 180;
		game_variables[first + 2] = (rand() % 360) * PI / 180;
		game_variables[first + 3] = (rand() % 360) * PI / 180;
		if (During(28, 38)) {
			ptr->v.y -= v/10;
		}
		if (ptr->times == 38) {
			ptr->v.y = 0.0f;
		}
		if (ptr->times == HARD * 50) {
			ptr->immortal = false;
		}
		if (During(50, 1000) && ptr->times % (20 + 10 * (4 - HARD)) == 0) {
			float tv = 5.0f;
			if (HARD >= 3) {
				tv = 3.0f;
			}
			vec2 tf;
			tf = { ptr->r.x - 64,ptr->r.y - 64 };
			missile(6, tf, { tv,game_variables[first+0],3.5f,4.0f });
			tf = { ptr->r.x - 64,ptr->r.y + 64 };
			missile(6, tf, { tv,game_variables[first+1],3.5f,4.0f });
			tf = { ptr->r.x + 64,ptr->r.y + 64 };
			missile(6, tf, { tv,game_variables[first+2],3.5f,4.0f });
			tf = { ptr->r.x + 64,ptr->r.y - 64 };
			missile(6, tf, { tv,game_variables[first+3],3.5f,4.0f });
		}

		if (ptr->times == 1000) {
			ptr->v.y = -1.20f;
		}
	};
}
//坐标 xy
template<>
void locus<8>(int x, int y) {
	Monster& mon = Push(8);
	mon.v = { 0.0f,10.0f };
	POS2(mon, x, y);
	mon.immortal = true;
	game_variables[0] = 0;
	mon.call_back = [](Monster* ptr) {
		if (During(28, 53)) {
			ptr->v.y -= 0.4f;
		}
		if (ptr->times == 53) {
			ptr->v.y = 0.0f;
		}
		if (During(2000, 2010)) {
			ptr->v.y = 0.32f;
		}
		if (ptr->times == 100) {
			ptr->immortal = false;
		}
		if (During(40, 2500) && ptr->times % (15 - HARD * 2 + 8) == 0) {
			/*if (!(ptr->times > 240 && HARD == 1)) {
				missile(4, ptr->r, { 6.0f,24.0f });
			}*/
			missile(5, ptr->r, { 4.50f,
				((4 - HARD) * 0.5f + 7.50f) * PI / 180,
				-82 * PI / 180 + game_variables[0] + (4 - HARD) * 2 * PI / 180,
				263 * PI / 180 + game_variables[0] - (4 - HARD) * 2 * PI / 180,
				});
			if (HARD <= 3) {
				if (ptr->r.x <= 451) {
					game_variables[0] -= 3 * PI / 180;
				} else {
					game_variables[0] += 3 * PI / 180;
				}
			} else {
				if (ptr->r.x <= 451) {
					game_variables[0] += 3 * PI / 180;
				} else {
					game_variables[0] -= 3 * PI / 180;
				}
			}
		}
	};
}
//坐标 xy
template<>
void locus<9>(int x, int y) {
	Monster& mon = Push(9);
	mon.v = { 0.0f,5.0f };
	POS2(mon, x, y);
	mon.immortal = true;
	mon.call_back = [](Monster* ptr) {
		if (During(28, 38)) {
			ptr->v.y -= 0.5f;
		}
		if (ptr->times == 38) {
			ptr->v.y = 0.0f;
		}
		if (ptr->times == HARD*30+130) {
			ptr->immortal = false;
		}

		if (During(50, 1000) && ptr->times % (30 + 10 * (4 - HARD)) == 0) {
			vec2 tf;
			for (int i = 0; i < 360; i += 90) {
				tf = { ptr->r.x - 64,ptr->r.y - 64 };
				missile(6, tf, { 3.0f,((rand() % 90) + i) * PI / 180,2 + HARD * 0.5f,4.0f });
				if (i == 0 && HARD >= 2) missile(9, tf, { 5.0f, (20 - HARD * 5 + 25) * PI / 180,1.2f,0.02f });
				game_variables[0] += 10 * PI / 180;
				tf = { ptr->r.x - 64,ptr->r.y + 64 };
				missile(6, tf, { 3.0f,((rand() % 90) + i) * PI / 180,2 + HARD * 0.5f,4.0f });
				if (i == 0 && HARD >= 2) missile(9, tf, { 5.0f, (20 - HARD * 5 + 25)* PI / 180,1.2f,0.02f });
				game_variables[1] += 10 * PI / 180;
				tf = { ptr->r.x + 64,ptr->r.y + 64 };
				missile(6, tf, { 3.0f,((rand() % 90) + i) * PI / 180,2 + HARD * 0.5f,4.0f });
				if (i == 0 && HARD >= 2) missile(9, tf, { 5.0f, (20 - HARD * 5 + 25) * PI / 180,1.2f,0.02f });
				game_variables[2] += 10 * PI / 180;
				tf = { ptr->r.x + 64,ptr->r.y - 64 };
				missile(6, tf, { 3.0f,((rand() % 90) + i) * PI / 180,2 + HARD * 0.5f,4.0f });
				if (i == 0 && HARD >= 2) missile(9, tf, { 5.0f, (20 - HARD * 5 + 25) * PI / 180,1.2f,0.02f });
				game_variables[3] += 10 * PI / 180;
			}
		}

		if (ptr->times == 1000) {
			ptr->v.y = -1.20f;
		}
	};
}
extern std::vector<S_Ptr<Monster>> monsters;
template<>
void locus<10>(int x, int y, vec2 v) {
	Monster& mon = Push(11);
	mon.v = v;
	POS2(mon, x, y);
	mon.immortal = true;
	int a = /*rand() & */0x1;
	mon.call_back = [a, v](Monster* ptr) {
		if (During(0, 80)) {
			ptr->v.x -= v.x / 80;
			ptr->v.y -= v.y / 80;
		}
		if (ptr->times == 80) {
			ptr->v.x = 0;
			ptr->v.y = 0;
		}
		if (During(100, 170) && (ptr->times - 100) % 10 == 0) {
			int t = ptr->times - 100;
			missile(11, ptr->r, { 18,30.0f + t / 10 * 30.0f, (0.1f + HARD * 0.05f) *PI/180,1.4f ,120.0f,25.0f-(HARD*3.4f) });
		}
		if (During(250, 330)) {
			ptr->v.x += v.x / 80;
			ptr->v.y += v.y / 80;
		}
		if (ptr->times == 170) {
			ptr->immortal = false;
		}
	};
}
template<>
void locus<11>(int x, int y, vec2 v) {
	Monster& mon = Push(11);
	mon.v = v;
	POS2(mon, x, y);
	mon.immortal = true;
	int a = /*rand() & */0x1;
	mon.call_back = [a,v](Monster* ptr) {
			if (During(0, 50)) {
				ptr->v.x -= v.x / 50;
				ptr->v.y -= v.y / 50;
			}
			if (ptr->times == 50) {
				ptr->v.x = 0;
				ptr->v.y = 0;
			}
			if (During(130, 180)) {
				ptr->v.x += v.x / 50;
				ptr->v.y += v.y / 50;
			}
			if (During(70, 100) && (ptr->times - 70) % 10 == 0) {
				int t = ptr->times - 70;
				missile(10, ptr->r, { 18,35.0f + t/10 * 25.0f,1.6f + HARD * 0.1f,1.8f ,float(30 + t),15 + (8-HARD*2.0f) });
			}
			if (During(70, 100)) {
				for (int i = 0; i < Bullet::pool->max; i++) {
					if (Bullet::at(i).sprite->x <= 600 && Bullet::at(i).sprite->x > 300) {
						int kkk = 0;
					};
				}
				
			}
			if (During(130, 330) && (ptr->times - 130) % (22 - HARD * 3) == 0) {
				missile(10, ptr->r, { 18,25.0f,1.50f,0.6f+HARD*0.1f ,float(30 + (ptr->times - 130)),20 + (6 - HARD * 1.5f) });
			}
			if (ptr->times == 100) {
			ptr->immortal = false;
		}
	};
}
template<>
void locus<12>(int x, int y, vec2 v) {
	Monster& mon = Push(11);
	mon.v = v;
	POS2(mon, x, y);
	mon.immortal = true;
	int a = /*rand() & */0x1;
	mon.call_back = [a, v](Monster* ptr) {
		if (During(0, 50)) {
			ptr->v.x -= v.x / 50;
			ptr->v.y -= v.y / 50;
		}
		if (ptr->times == 50) {
			ptr->v.x = 0;
			ptr->v.y = 0;
		}
		if (During(130, 180)) {
			ptr->v.x += v.x / 50;
			ptr->v.y += v.y / 50;
		}
		int k = 10;
		if (HARD == 1) k = 25;
		if (During(70, 100) && (ptr->times - 70) % k == 0) {
			int t = ptr->times - 70;
			missile(10, ptr->r, { 18,35.0f + t / 10 * 25.0f,0.80f,0.90f ,float(30 + t),20.0f + (10 - HARD * 2.5f) });
		}
		if (During(130, 330) && (ptr->times - 130) % (12 - HARD * 3 + k) == 0) {
			missile(10, ptr->r, { 18,25.0f,0.50f,1.0f ,float(30 + (ptr->times - 130)),35 + (8 - HARD * 2.0f) });
		}
		if (ptr->times == 100) {
			ptr->immortal = false;
		}
	};
}


int move_by;
void move(Monster* ptr, int index) {
	if (HARD <= 2) return;
	if (move_by == 0) {
		if (index) {
			ptr->v = { 4.0f, 0.0f };
			move_by = 2;
		} else {
			ptr->v = { 2.0f, 0.50f };
			move_by = 1;
		}
		return;
	}
	if (move_by == 1) {
		if (index) {
			ptr->v = { -2.0f, -0.5f };
			move_by = 0;
		} else {
			ptr->v = { 2.0f, -0.5f };
			move_by = 2;
		}
		return;
	}
	if (move_by == 2) {
		if (index) {
			ptr->v = { -4.0f, 0.0f };
			move_by = 0;
		} else {
			ptr->v = { -2.0f, 0.50f };
			move_by = 1;
		}
		return;
	}
}
template<>
void locus<13>(Monster* ptr) {//仿春终
	if (ptr->hp <= 50000) {
		if (ptr->times == 0) {
			ptr->immortal = false;
		}
		if (During(0, 40) && ptr->times % (6 - HARD) == 0) {
			missile(15, monsters[0]->r, { ptr->times / 2.0f,1.0f });
		}
		if (ptr->times == 40) {
			move(ptr, rand() % 2 - 1);
		}
		if (ptr->times == 140) {
			ptr->v = { 0,0 };
		}
		if (During(240, 280) && ptr->times % (6 - HARD) == 0) {
			missile(15, monsters[0]->r, { (ptr->times - 240) / 2.0f,-1.0f });
		}
		if (ptr->times == 280) {
			move(ptr, rand() % 2 - 1);
		}
		if (ptr->times == 380) {
			ptr->v = { 0,0 };
		}
		if (ptr->times >= 480) {
			ptr->times = 0;
		}
	} else {
		if (ptr->times == 0) {
			ptr->immortal = false;
		}
		if (During(0, 40) && ptr->times % (6 - HARD) == 0) {
			missile(15, monsters[0]->r, { ptr->times / 2.0f,1.0f });
		}
		if (ptr->times == 40) {
			move(ptr, rand() % 2 - 1);
		}
		if (ptr->times == 140) {
			ptr->v = { 0,0 };
		}
		if (During(300, 340) && ptr->times % (6 - HARD) == 0) {
			missile(15, monsters[0]->r, { (ptr->times - 300) / 2.0f,-1.0f });
		}
		if (ptr->times == 340) {
			move(ptr, rand() % 2 - 1);
		}
		if (ptr->times == 440) {
			ptr->v = { 0,0 };
		}
		if (ptr->times == 600) {
			ptr->times = 0;
		}
	}
}
template<>
void locus<14>(Monster* ptr) {//仿春终
	if (ptr->times % 20 * (9 - 2*HARD) == 0) {
		for (int i = 29; i < 873; i += 200) {
			float x = i + (rand() % 200) - 100;
			vec2 r = { x,50 };
			float vy = (rand() % 150) * 0.01f + (1.0f+HARD*0.25f);
			float vx = ((rand() % 10) - 5) * 0.1f;
			missile(16, r, { vx,vy });
		}
	}
}
template<>
void locus<15>(Monster* ptr) {//仿春终
	if (ptr->times % 100 == 0) {
		//螺旋弹2 参数：【颜色，初始半径，角速度，径向速度,静止时间，密度】
		float v = (rand() % 200) * 0.01f + 1.50f;
		missile(17, ptr->r, { 50,v,1 });
	}
	if ( ptr->times % 100 == 40) {
		//螺旋弹2 参数：【颜色，初始半径，角速度，径向速度,静止时间，密度】
		float v = (rand() % 200) * 0.01f + 1.50f;
		missile(17, ptr->r, { 50,v,-1 });
	}
	if (ptr->times % 100 == 75 && (HARD>=3)) {
		//螺旋弹2 参数：【颜色，初始半径，角速度，径向速度,静止时间，密度】
		missile(0, { ptr->r.x - 80, ptr->r.y - 80 }, { 3.00f,16 });
		missile(0, { ptr->r.x - 80, ptr->r.y + 80 }, { 3.00f,16 });
		missile(0, { ptr->r.x + 80, ptr->r.y + 80 }, { 3.00f,16 });
		missile(0, { ptr->r.x + 80, ptr->r.y - 80 }, { 3.00f,16 });
	}
}
template<>
void locus<16>(Monster* ptr) {//仿春终
	if (ptr->times % 120 == 50) {
		//螺旋弹2 参数：【颜色，初始半径，角速度，径向速度,静止时间，密度】
		float v = 5.0f;
		if (HARD == 4) {
			missile(18, ptr->r, { v });
		}
		missile(19, ptr->r, { v });
	}
}