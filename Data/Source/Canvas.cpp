//============================================================
//	Canvas 画板
//============================================================ 
#include <stdafx.h>
#include <Canvas.h>
#include <Base.h>
#include <Thief.h>
#include <Bitmap.h>

Canvas::Canvas() {
	fbo.Bind();
	texture.Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);//开启半透明混色
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//混色方案
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}
Canvas::Canvas(const Canvas& src) {
	fbo = src.fbo;
	texture = src.texture;
}
Canvas& Canvas::operator=(const Canvas& src) {
	fbo = src.fbo;
	texture = src.texture;
	return *this;
}

void Canvas::Bind() {
	fbo.Bind();
	glViewport(0, 0, Canvas::get_width(), Canvas::get_height());
}

THIEF_TEMPLATE(Steal,Bitmap,Texture, texture);
void Canvas::get_Bitmap(Bitmap& dst) {
	dst = Bitmap(WIDTH, HEIGHT);
	Texture& temp = Steal(dst);
	temp = texture;
}
int Canvas::get_width() {
	return WIDTH;
}
int Canvas::get_height() {
	return HEIGHT;
}