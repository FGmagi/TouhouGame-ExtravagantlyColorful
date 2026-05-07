#include <stdafx.h>
#include <Rect.h>
Rect::Rect() {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}
Rect::Rect(const Rect& n) {
	x = n.x;
	y = n.y;
	w = n.w;
	h = n.h;
};
Rect::Rect(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
};
Rect& Rect::operator = (const Rect& n) {
	x = n.x;
	y = n.y;
	w = n.w;
	h = n.h;
	return *this;
};
bool Rect::operator!=(const Rect& src){
	return !(x == src.x && y == src.y && w == src.w && h == src.h);
}
int Rect::wh() {
	return (w * h);
}
Rect::~Rect() {

};
