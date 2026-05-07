//============================================================
//	头文件引用
//============================================================
#include <stdafx.h>
#include "Sprite_Map.h"
#include "Debug.h"
typedef Sprite_Map2::iterator It;
//============================================================
//	精灵集合 轻量级
//============================================================
//Sprite_Map
void Sprite_Map::update() {
	//鬼知道编译器会不会自己优化
	int s = map.size();
	for (int i = 0; i < s; i++) {
		map[i]->update();
	}
}
void Sprite_Map::push(Sprite* p) {
	map.push_back(p);
	p->set_idle(false);
}
//============================================================
//	精灵集合 重量级
//============================================================
Sprite_Map2::Sprite_Map2(Bitmap tbitmap,int z) :Object(z),bitmap(tbitmap) {
	vao.Bind();
	vbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, 17 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);//预留位置
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));//位置
	glVertexAttribDivisor(0, 1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));//位置
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));//纹理
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));//纹理
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(16 * sizeof(GLfloat)));//透明度
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	vao.relieve();
}
It&& Sprite_Map2::begin() {
	return It(0, this);
}
It&& Sprite_Map2::end() {
	return It(map.size(), this);
}
void Sprite_Map2::update() {
	if (revise == 1) {//扩容并全部修改
		for (int i = 0; i < map.size(); i++) {
			Position(*this, i);
			map[i].second = false;
		}
		vbo.Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);//预留位置
		revise = 0;
	}
	if (revise == 2) {//部分修改
		vbo.Bind();
		for (int i = 0; i < map.size(); i++) {
			if (map[i].second) {
				Position(*this, i);
				glBufferSubData(GL_ARRAY_BUFFER, i * 17 * sizeof(GLfloat), 17 * sizeof(GLfloat), &vertices[i]);
				map[i].second = false;
			}
		}
		revise = 0;
	}
	if (revise == 3) {//扩容但不全部修改
		for (int i = 0; i < map.size(); i++) {
			if (map[i].second) {
				Position(*this, i);
				map[i].second = false;
			}
		}
		vbo.Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);//预留位置
		revise = 0;
	}
	vao.Bind();
	bitmap.Bind();
	Shader::code[6].Bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP,0, 4, map.size());
	vao.relieve();
}
void Sprite_Map2::Ask() {
	revise = 1;
}
void Sprite_Map2::Ask(Sprite& src) {
	for (int i = 0; i < map.size();i++) {
		if (&map[i].first == &src) {
			map[i].second = true;
			if (revise != 1 && revise != 3) {
				revise = 2;
			}
			return;
		};
	}
	print("未能匹配到Sprite");
	return;
}
void Sprite_Map2::Ask(It& src){
	map[src.index].second = true;
	if (revise != 1 && revise != 3) {
		revise = 2;
	}
}
It&& Sprite_Map2::push(Sprite src) {
	for (int i = 0; i < 17; i++) {
		vertices.emplace_back(0.0f);
	}
	src.active = false;
	src.set_idle(false);
	if (revise != 1) {
		revise = 3;
	}
	std::pair<Sprite, bool> temp{ src, true };
	map.push_back(temp);
	return It(map.size() - 1, this);
}
Sprite_Map2::~Sprite_Map2() {};

inline void swap(float& a, float& b) {
	float c = a;
	a = b;
	b = c;
}
//重设 Object 位置算法
void to_Postion2(float* dst, int tx, int ty, int w, int h, Complex<float>* n, Complex<float>* orgin);
void to_TexCoord2(float* dst, int x, int y, int w, int h, int world_width, int world_height);
void Sprite_Map2::Position(Sprite_Map2 &p,int i) {
	Sprite &sp = p.map[i].first;
	float *vertices = &p.vertices[i*17];
	int tx = sp.x;	int ty = sp.y;
	auto &lens = sp.lens;
	if (sp.mask_) {
		tx += lens.x; ty += lens.y;
	}
	to_Postion2(&vertices[0], tx, ty, lens.w, lens.h, &sp.complex, &sp.orgin_);
	to_TexCoord2(&vertices[0], lens.x, lens.y, lens.w, lens.h, bitmap.get_width(), bitmap.get_height());
	vertices[16] = sp.opacity/255.0f;
}
void Sprite_Map2::iterator::operator ++() {
	if (index == p->map.size()) {
		print("数组越界");
		ERROR_1;
	}
	index++;
};
Sprite& Sprite_Map2::iterator::operator *() {
	if (index == p->map.size()) {
		print("数组越界");
		ERROR_1;
	}
	return p->map[index].first;
};
Sprite* Sprite_Map2::iterator::operator ->() {
	if (index == p->map.size()) {
		print("数组越界");
		ERROR_1;
	}
	return &(p->map[index].first);
};
Sprite_Map2::iterator::iterator(int a, Sprite_Map2* b) {
	index = a;
	p = b;
};

#include <Canvas.h>
//修改如下：
//将四个坐标xy与四个纹理xy交错，并交换第三顶点与第四顶点
void to_Postion2(float* dst, int tx, int ty, int w, int h, Complex<float>* n, Complex<float>* orgin) {
	ty = (-ty);
	int world_width = Canvas::get_width();
	int world_height = Canvas::get_height();
	float x = normal_x(tx, world_width);
	float y = normal_y(ty, world_height);
	float w2 = float(w) / WIDTH * 2;//世界坐标宽度的一半
	float h2 = float(h) / HEIGHT * 2;
	if (n == NULL || (n->real == 1.0f && n->imag == 0.0f)) {
		dst[0] = x;			dst[1] = y;
		dst[4] = x;			dst[5] = y - h2;
		dst[12] = x + w2;	dst[13] = y - h2;
		dst[8] = x + w2;	dst[9] = y;
	} else {
		if (orgin == NULL) {
			w2 /= 2;
			h2 /= 2;
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
			dst[4] = OB.real; dst[5] = OB.imag;
			dst[12] = OC.real; dst[13] = OC.imag;
			dst[8] = OD.real; dst[9] = OD.imag;
		} else {
			Complex<float>& complex = *n;
			Complex<float> O(x + w2 * orgin->real, y - h2 * orgin->imag);//原点
			Complex<float> OA(-w2 * (orgin->real), h2 * (orgin->imag));//左上角
			Complex<float> OB(-w2 * (orgin->real), -h2 * (1 - orgin->imag));//左下角
			Complex<float> OC(w2 * (1.0f - orgin->real), -h2 * (1.0f - orgin->imag));//右下角
			Complex<float> OD(w2 * (1.0f - orgin->real), h2 * (orgin->imag));//右上角
			OA = OA * complex + O;
			OB = OB * complex + O;
			OC = OC * complex + O;
			OD = OD * complex + O;
			dst[0] = OA.real; dst[1] = OA.imag;
			dst[4] = OB.real; dst[5] = OB.imag;
			dst[12] = OC.real; dst[13] = OC.imag;
			dst[8] = OD.real; dst[9] = OD.imag;
		}
	}
}
void to_TexCoord2(float* dst, int x, int y, int w, int h, int world_width, int world_height) {
	dst[2] = (float)(x) / world_width;
	dst[3] = (float)(world_height - y) / world_height;
	dst[6] = (float)(x) / world_width;
	dst[7] = (float)(world_height - y - h) / world_height;

	dst[14] = (float)(x + w) / world_width;
	dst[15] = (float)(world_height - y - h) / world_height;
	dst[10] = (float)(x + w) / world_width;
	dst[11] = (float)(world_height - y) / world_height;
}