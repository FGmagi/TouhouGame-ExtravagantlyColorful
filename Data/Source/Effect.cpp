//============================================================
//	Effect  着色器演出
//============================================================
#include <stdafx.h>
#include "Effect.h"
#include "OpenGL.h"
#include "Sprite.h"

Effect::Effect(int z): Object(z) {
	active = true;
	functor = NULL;
	function = NULL;
}
Effect::Effect(Functor* p, int z) : Object(z) {
	active = true;
	functor = p;
	functor->mul_effect = this;
	function = NULL;
}
Effect::Effect(void (*p)(void), int z) : Object(z) {
	active = true;
	functor = NULL;
	function = p;
}
void Effect::reset(Functor* p) {
	if (functor) delete functor;
	active = true;
	functor = p;
	functor->mul_effect = this;
	function = NULL;
	active = true;
}
void Effect::reset(void (*p)(void)) {
	if (functor) delete functor;
	functor = NULL;
	function = p;
	active = true;
}
bool Effect::empty() {
	return (functor == NULL && function == NULL);
}
Functor* Effect::get_functor(){
	return functor;
}
void Effect::update() {
	if (active) {
		if (functor) {
			functor->update();
		}
		if (function) {
			function();
		}
	}
}
Effect::~Effect() {
	if (functor) {
		delete functor;
		functor = NULL;
	}
	function = NULL;
}
Functor::Functor() {
	mul_effect = NULL;
}

//颜色渐变
void Effect_Froze(int last_times, Color4 color, int* rest) {
	static Color4 color_recode = Color4(0, 0, 0, 0);
	static Effect effect(999);
	class Froze_fun :public Functor {
	public:
		//恢复色彩
		Froze_fun(int last_times, Color4& color) {
			times = 0;
			this->last_times = last_times;
			this->color = color;
			sprite = new Sprite({ 0, 0, WIDTH, HEIGHT }, 999);
			if (last_times == 0) {
				Color4 color;
				color.r = float(this->color.r);
				color.g = float(this->color.g);
				color.b = float(this->color.b);
				color.a = float(this->color.a);
				color_recode = color;
				sprite->bitmap.fill_rect(color, { 0,0,WIDTH,HEIGHT });
				sprite->active = true;
			} else {
				sprite->active = false;
			}
		};
		~Froze_fun() {
			if (sprite) {
				mul_effect->active = false;
				color_recode = color;
				delete sprite;
				sprite = NULL;
				last_times = 0;
				times = 0;
			}
		};
		void update() {
			if (last_times == 0) { 
				if (rest) *rest = 0;
				mul_effect->active = false;
				return;
			}
			if (last_times < 0) {
				Color4 color;
				color.r = float(this->color.r);
				color.g = float(this->color.g);
				color.b = float(this->color.b);
				color.a = float(this->color.a);
				color_recode = color;
				sprite->bitmap.fill_rect(color, { 0,0,WIDTH,HEIGHT });
				sprite->active = true;
				sprite->update();
				sprite->active = false;
				if (rest) *rest = -1;
				return;
			}
			if (!sprite) {
				sprite = new Sprite({ 0, 0, WIDTH, HEIGHT }, 999);
				sprite->active = false;;
			}
			if (times < last_times) {
				times++;
				float t = float(times) / last_times;
				Color4 color;
				color.r = float(color_recode.r) * (1 - t) + float(this->color.r) * t;
				color.g = float(color_recode.g) * (1 - t) + float(this->color.g) * t;
				color.b = float(color_recode.b) * (1 - t) + float(this->color.b) * t;
				color.a = float(color_recode.a) * (1 - t) + float(this->color.a) * t;
				sprite->bitmap.fill_rect(color, { 0,0,WIDTH,HEIGHT });

				sprite->active = true;
				if (times != last_times) {
					sprite->update();
					sprite->active = false;
				} else {
					last_times = 0;
					times = 0;
				}
			}
			if (rest) {
				*rest = last_times - times;
			}
		}
		int times;
		int last_times;
		Sprite* sprite;
		Color4 color;
		int* rest;
	};
	effect.reset(new Froze_fun(last_times,color));
	static_cast<Froze_fun*>(effect.get_functor())->rest = rest;
	if (rest) *rest = last_times;
};

class Fade_fun :public Functor {
public:
	static S_Ptr<Effect> effect;
	static S_Ptr<Sprite> sprite;
	static bool fade_ready;
	void ini() {
		times = 0;
		last_times = 0;
		rest = NULL;
		sprite->bitmap.screenshot();
	}
	void update() {
		if (last_times == 0 || times == last_times) {
			sprite->opacity = 0;
			sprite->active = false;
			mul_effect->active = false;
			if (rest) *rest = 0;
		} else {
			sprite->opacity = 255-uc(float(times)/last_times*255);
			if (rest) *rest = times - last_times;
			times++;
		}
	}
	int times, last_times, * rest;
};	
S_Ptr<Effect> Fade_fun::effect;
S_Ptr<Sprite> Fade_fun::sprite;
bool Fade_fun::fade_ready;

void Effect_Set_Fade() {
	if (Fade_fun::effect == NULL) {
		Fade_fun::effect = std::make_shared<Effect>(new Fade_fun(),999);
		Fade_fun::sprite = std::make_shared<Sprite>(Rect{0,0,1280,960},999);
		Fade_fun::fade_ready = false;
		Fade_fun::effect->active = false;
		Fade_fun::sprite->active = false;
	}
	((Fade_fun*)(Fade_fun::effect->get_functor()))->ini();
	Fade_fun::sprite->active = true;
	Fade_fun::effect->active = false;
	Fade_fun::sprite->opacity = 255;
	Fade_fun::fade_ready = true;
};
void Effect_Do_Fade(int last_times, int* rest) {
	if (!Fade_fun::fade_ready)
		return;
	Fade_fun::effect->active = true;
	Fade_fun& f = *(Fade_fun*)(Fade_fun::effect->get_functor());
	f.times = 0;
	f.last_times = last_times;
	f.rest = rest;
	Fade_fun::fade_ready = false;
};
