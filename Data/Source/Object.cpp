//============================================================
//	所有位图对象 Object 基类
//============================================================
#include <stdafx.h>
#include <Object.h>
#include <Canvas.h>
#include <Complex.h>
#include <Effect.h>
#include <Thief.h>
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
//-----------------------------------------------------------
//	函数声明
//-----------------------------------------------------------
typedef	std::list<Object*> list;
typedef	list::iterator list_It;
typedef	std::map<int,list> map;
typedef	std::map<int, list>::iterator map_It;
void push(int depth, Object* p);
void del(Object*);
map* impl;
//============================================================
//	Object类
//============================================================
Object::Object(int depth) {
	push(depth, this);
	this->depth = depth;
}
void Object::set_depth(int depth) {
	if (depth != get_depth()) {
		del(this);
		push(depth, this);
	}
}
int Object::get_depth() {
	return depth;
}
Object::~Object() {
	del(this);
}
void Object::rend() {
	for (auto it1 = impl->begin(); it1 != impl->end(); it1++) {
		list& temp = it1->second;
		for (auto it2 = temp.begin(); it2 != temp.end();) {
			(*it2)->update();
			if ((*it2)->idioctonia) {
				delete static_cast<Object*>(*it2++);
				continue;
			}
			it2++;
		}
	}
}
void Object::set_idle(bool n) {
	if (n != idle) {
		if (n == true) {
			push(depth, this);
		} else {
			del(this);
		}
	}
};
void Object::update() {
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
//-----------------------------------------------------------
//	对象管理池
//-----------------------------------------------------------
THIEF_TEMPLATE(Steal,Object, map_It, map_note);
THIEF_TEMPLATE(Steal2, Object, list_It, list_note);
THIEF_TEMPLATE(Steal3, Object, bool, idle);
void push(int depth, Object* p) {
	if (!p->get_idle()) {
		list& L = (*impl)[depth];
		Steal(*p) = impl->find(depth);

		L.emplace_back(p);
		Steal2(*p) = L.end();
		Steal2(*p)--;
		Steal3(*p) = true;
	}
};
void del(Object* p) {
	if (impl != NULL && p->get_idle()) {
		list& temp = Steal(*p)->second;
		temp.erase(Steal2(*p));
		if (temp.empty()) {
			impl->erase(Steal(*p));
		};
		Steal3(*p) = false;
	}
};
//-----------------------------------------------------------
//	Object接口GC
//-----------------------------------------------------------
Object_port::Object_port() {
	impl = new map;
}
Object_port::~Object_port() {
	for (auto i = impl->begin(); i != impl->end(); i++) {
		i->second.clear();
	}
	delete impl;
	impl = NULL;
}
//-----------------------------------------------------------
//	其他常用函数
//-----------------------------------------------------------
void to_Postion(float* dst, int tx, int ty, int w, int h, Complex<float>* n, Complex<float>* orgin) {
	ty = (-ty);
	int world_width = Canvas::get_width();
	int world_height = Canvas::get_height();
	if (n == NULL || (n->real == 1.0f && n->imag == 0.0f)) {
		float x = normal_x(tx, world_width);
		float y = normal_y(ty, world_height);
		float w2 = float(w) / WIDTH * 2;//世界坐标宽度的一半
		float h2 = float(h) / HEIGHT * 2;
		dst[0] = x;			dst[1] = y;
		dst[2] = x;			dst[3] = y - h2;
		dst[4] = x + w2;	dst[5] = y - h2;
		dst[6] = x + w2;	dst[7] = y;
	} else {
		if (orgin == NULL) {
			float x = normal_x(tx, world_width);
			float y = normal_y(ty, world_height);
			float w2 = float(w) / WIDTH;//世界坐标宽度的一半
			float h2 = float(h) / HEIGHT;
			Complex<float>& complex = *n;
			Complex<float> O(x + w2, y - h2);//原点
			Complex<float> OA(-w2, h2);//左上角
			Complex<float> OB(-w2, -h2);//左下角
			Complex<float> OC(w2, -h2);//右下角
			Complex<float> OD(w2, h2);//右上角
			OA = OA * complex + O;
			OB = OB * complex + O;
			OC = OC * complex + O;
			OD = OD * complex + O;
			dst[0] = OA.real; dst[1] = OA.imag;
			dst[2] = OB.real; dst[3] = OB.imag;
			dst[4] = OC.real; dst[5] = OC.imag;
			dst[6] = OD.real; dst[7] = OD.imag;
		} else {
			Complex<float>& const complex = *n;
			Complex<float> O(tx + w * (orgin->real), ty - h * (orgin->imag));
			Complex<float> OA(-w * (orgin->real), h * (orgin->imag));//左上角
			Complex<float> OB(-w * (orgin->real), -h * (1.0f - orgin->imag));//左下角
			Complex<float> OC(w * (1.0f - orgin->real), -h * (1.0f - orgin->imag));//右下角
			Complex<float> OD(w * (1.0f - orgin->real), h * (orgin->imag));//右上角
			OA = OA * complex + O;
			OB = OB * complex + O;
			OC = OC * complex + O;
			OD = OD * complex + O;

			dst[0] = normal_x(OA.real, world_width); dst[1] = normal_y(OA.imag, world_height);
			dst[2] = normal_x(OB.real, world_width); dst[3] = normal_y(OB.imag, world_height);
			dst[4] = normal_x(OC.real, world_width); dst[5] = normal_y(OC.imag, world_height);
			dst[6] = normal_x(OD.real, world_width); dst[7] = normal_y(OD.imag, world_height);
		}
	}
}
void to_TexCoord(float* dst, int x, int y, int w, int h,int world_width,int world_height) {
	dst[0] = (float)(x)/ world_width;
	dst[1] = (float)(world_height - y) / world_height;
	dst[2] = (float)(x) / world_width;
	dst[3] = (float)(world_height - y - h) / world_height;
	dst[4] = (float)(x + w) / world_width;
	dst[5] = (float)(world_height - y - h) / world_height;
	dst[6] = (float)(x + w) / world_width;
	dst[7] = (float)(world_height - y) / world_height;
}
void to_TexCoord(float* dst) {
	dst[0] = 0.0f;	dst[1] = 1.0f;
	dst[2] = 0.0f;	dst[3] = 0.0f;
	dst[4] = 1.0f;	dst[5] = 0.0f;
	dst[6] = 1.0f;	dst[7] = 1.0f;
}
void to_Opacity(float* dst, unsigned char opacity) {
	float temp = (float)opacity / 255.0f;
	dst[0] = temp;
	dst[1] = temp;
	dst[2] = temp;
	dst[3] = temp;
};

void Object_Deposit(Object* p, int times) {
	struct box {
		Object* p;
		int times;
	};
	static std::list<struct box> List;
	class FU :public Functor {
		void update() {
			for (auto i = List.begin(); i != List.end(); i++) {
				i->times--;
				if (i->times == 0) {
					delete i->p;
					List.erase(i);
				}
			}
		}
	};
	static std::unique_ptr<Effect> effect;
	effect.reset(new Effect(new FU, 10000));
	List.push_back({ p,times });
};