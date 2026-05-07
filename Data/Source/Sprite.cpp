//============================================================
//	Sprite 类
//  每帧约可渲染 375 M 
//============================================================ 
#include <stdafx.h>
#include <Base.h>
#include <Sprite.h>
#include <Shader.h>
//============================================================
//	构造函数
//============================================================ 
Sprite::Sprite(const char* str, int tx, int ty, int tz) :
	Object(tz), complex(1.0f, 0.0f),bitmap(str){
	x.assign(tx);
	y.assign(ty);
	opacity.assign(255);
	active = true;
	lens.x = 0;
	lens.y = 0;
	orgin_ = { 0.5f,0.5f };
	revise = false;
	lens.w = bitmap.get_width();
	lens.h = bitmap.get_height();
	mask_ = false;
	//上传基础数据
	preinstall();
};
Sprite::Sprite(Rect&& pos, int tz):
	Object(tz), complex(1.0f, 0.0f), bitmap(pos.w, pos.h) {
	x.assign(pos.x);
	y.assign(pos.y);
	opacity.assign(255);
	active = true;
	lens.x = 0;
	lens.y = 0;
	orgin_ = { 0.5f,0.5f };
	revise = false;
	lens.w = pos.w;
	lens.h = pos.h;
	mask_ = false;
	//上传基础数据
	preinstall();
}
Sprite::Sprite(Bitmap& bitmap, int tx, int ty, int tz) :
	Object(tz), complex(1.0f, 0.0f), bitmap(bitmap) {
	x.assign(tx);
	y.assign(ty);
	opacity.assign(255);
	active = true;
	lens.x = 0;
	lens.y = 0;
	orgin_ = { 0.5f,0.5f };
	revise = false;
	lens.w = bitmap.get_width();
	lens.h = bitmap.get_height();
	mask_ = false;
	//上传基础数据
	preinstall();
};
Sprite::Sprite(const Sprite& other):Object(const_cast<Sprite*>(&other)->get_depth()),complex(other.complex),bitmap(other.bitmap) {
	*this = other;
};
Sprite& Sprite::operator =(const Sprite& other) {
	x.assign(other.x.value);
	y.assign(other.y.value);
	opacity.assign(other.opacity.value);
	active = other.active;
	lens = other.lens;
	orgin_ = other.orgin_;
	revise = other.revise;
	mask_ = other.mask_;
	vao = other.vao;
	vbo = other.vbo;
	shader_ = other.shader_;
	return *this;
};
Sprite::~Sprite() {
	shader_.reset();
}
//============================================================
//	读取图片
//============================================================ 
void Sprite::src(Rect&& rect) {
	src(std::move(rect), mask_);
}
void Sprite::src(Rect&& rect, int mask) {
	if (rect != lens) {
		this->lens.x = rect.x;
		this->lens.y = rect.y;
		this->lens.w = rect.w;
		this->lens.h = rect.h;
		this->mask_ = mask;
		revise = true;
	}
}
void Sprite::Position() {
	int tx = x;	int ty = y;
	if (mask_) { 
		tx += lens.x; ty += lens.y;
	}
	float vertices[20];
	to_Postion(&vertices[0], tx, ty, lens.w, lens.h, &complex,&orgin_);
	to_TexCoord(&vertices[8], lens.x, lens.y, lens.w, lens.h, bitmap.get_width(), bitmap.get_height());
	to_Opacity(&vertices[16], opacity);	vao.Bind();

	vbo.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(GLfloat), 20 * sizeof(GLfloat), vertices);
	vao.relieve();
	revise = false;
}
bool Sprite::judge() {
	bool a = false;
	if (x.judge()) {
		a = true;
		x.assign();
	}
	if (y.judge()) {
		a = true;
		y.assign();
	}
	if (opacity.judge()) {
		a = true;
		opacity.assign();
	}
	if (revise) {
		a = true;
	}
	return a;
}
//============================================================
//	渲染位图
//============================================================ 
void Sprite::update() {
	if (active) {
		if (judge()) {
			Position();
		}
		vao.Bind();
		bitmap.Bind();
		if (shader_ == NULL) {
			Shader::code[0].Bind();
		} else {
			shader_->Bind();
		}
		if (call_back) {
			call_back();
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		vao.relieve();
	}
};
void Sprite::clear() {
	bitmap.clear();
}
//============================================================
//	设置缩放率
//============================================================ 
void Sprite::zoom(float n) {
	complex.real *= n;
	complex.imag *= n;
	revise = true;
}
void Sprite::zoom() {
	float magnitude = complex.magnitude();
	complex.real /= magnitude;
	complex.imag /= magnitude;
	revise = true;
}
void Sprite::orgin(float x,float y) {
	orgin_.real = x;
	orgin_.imag = y;
	revise = true;
}
//============================================================
//	设置旋转角度
//============================================================ 
void Sprite::angle(float n) {
	Complex temp(cos(n), sin(n));
	complex *= temp;
	revise = true;
}
void Sprite::angle() {
	complex.real = complex.magnitude();
	complex.imag = 0.0f;
	revise = true;
}
//设置着色器
void Sprite::shader(Shader& src) {
	shader_.reset(new Shader(src));
};
//============================================================
//	上传基础数据到 GPU
//============================================================ 
void Sprite::preinstall() {
	float vertices[20];
	to_Postion(&vertices[0], x, y, lens.w, lens.h);
	to_TexCoord(&vertices[8]);
	to_Opacity(&vertices[16],opacity);

	vao.Bind();
	vbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);//预留位置
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));//位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));//纹理
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)(16 * sizeof(GLfloat)));//透明度
	glEnableVertexAttribArray(2);
	vao.relieve();
};
