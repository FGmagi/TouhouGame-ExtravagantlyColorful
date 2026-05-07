#include <stdafx.h>
#include <Color.h>
Color4::Color4() {
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}
Color4& Color4::operator=(Color4 n) {
	r = n.r;
	g = n.g;
	b = n.b;
	a = n.a;
	return *this;
}
Color4::Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
};
void Color4::set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
void Color4::clear() {
	r = 0;
	g = 0;
	b = 0;
	a = 0;
};
int Color4::rgba() {
	return ((int(a) << 24) | (int(b) << 16) | (int(g) << 8) | int(r));
}
Color4::~Color4() {

};

float fuck(float n ) {
	if (n < 0.0f) n = 0.0f;
	if (n > 1.0f) n = 1.0f;
	return n;
}

HSL::HSL() {
	h = 0.0f;
	s = 0.0f;
	l = 0.0f;
};
HSL::HSL(float h, float s, float l) {
	this->h = h;
	this->s = s;
	this->l = l;
};
void HSL::set(float h, float s, float l) {
	this->h = h;
	this->s = s;
	this->l = l;
}
void HSL::clear() {
	h = 0.0f;
	s = 0.0f;
	l = 0.0f;
};
HSL::~HSL() {

};

Color4 get_color(int id) {
	Color4 n = { 0,0,0,255 };
	switch (id) {
	case 0://°×É«
		n.set(255, 255, 255, 255);
		break;
	case 8://ºÚÉ«
		n.set(0,0,0,255);
		break;
	case 11://»ÒÉ«
		n.set(164, 164, 164, 255);
		break;
	}

	return n;
};
