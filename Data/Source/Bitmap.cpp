//============================================================
//	Bitmap 类 智能计数
//============================================================ 
#include <stdafx.h>
#include <Bitmap.h>
#include <Base.h>
#include <Canvas.h>
#include <Window.h>
#include <Graphics.h>
//============================================================
//	构造函数
//============================================================ 
Bitmap::Bitmap(const char* str) {
	texture.Bind();
	load_image(str);
};
Bitmap::Bitmap(int w, int h) {
	texture.Bind();
	this->width = w;
	this->height = h;
	load_image(NULL);
}
Bitmap::Bitmap(const Bitmap& src) {
	*this = src;
}
Bitmap& Bitmap::operator=(const Bitmap& src) {
	width = src.width;
	height = src.height;
	texture = src.texture;
	return *this;
}
/*Bitmap& Bitmap::operator=(Canvas& src) {
	width = WIDTH;
	height = HEIGHT;
	texture = src.texture;
	file_name = "";
	return *this;
}*/
void Bitmap::clone(Bitmap& src) {
	width = src.width;
	height = src.height;
	load_image(NULL);
	glCopyImageSubData(src.texture, GL_TEXTURE_2D, 0, 0, 0, 0,
		texture, GL_TEXTURE_2D, 0, 0, 0, 0, src.get_width(), src.get_height(), 1);
}
/*void Bitmap::clone(Canvas& src) {
	width = WIDTH;
	height = HEIGHT;
	load_image(NULL);
	glCopyImageSubData(src.texture, GL_TEXTURE_2D, 0, 0, 0, 0,
		texture, GL_TEXTURE_2D, 0, 0, 0, 0, WIDTH, HEIGHT, 0);
}*/
void Bitmap::load_image(const char* str) {
	int format= GL_RGBA;
	int* data = NULL;
	if (str != NULL) {//载入数据
		data = (int*)stbi_load(&str[0], &width, &height, &format, 0);	
		format = GL_RGB + format - 3;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data != NULL) {
		stbi_image_free(data);
	}
}
//============================================================
//	获取数据
//============================================================ 
int Bitmap::get_width() {
	return width;
}
int Bitmap::get_height() {
	return height;
}
//============================================================
//	设置 blt
//============================================================ 
void Bitmap::blt(Bitmap& src,Rect rect, int dst_x, int dst_y){
	glCopyImageSubData(src.texture, GL_TEXTURE_2D, 0, rect.x, src.height - rect.y - rect.h, 0,
		texture, GL_TEXTURE_2D, 0, dst_x, height - dst_y - rect.h, 0, rect.w, rect.h, 1);
}
void Bitmap::fill_rect(Color4 color, Rect rect) {
	texture.Bind();
	int* data = new int[rect.wh()];		
	int rgba = (color.rgba());
	for (int i = 0; i < rect.wh(); i++) {
		data[i] = rgba;
	}
	glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, height - rect.y - rect.h, rect.w, rect.h, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
//============================================================
//	设置 HSL
//============================================================ 
void Bitmap::hsl(HSL t) {
	//附加FBO
	FBO fbo;
	VAO vao;
	fbo.Bind();
	vao.Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glViewport(0, 0, width, height);
	//修改纹理
	texture.Bind();
	Shader::code[3].Bind();
	Uniform(3f, HSL, t.h, t.s, t.l);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	FBO::relieve();
}
//============================================================
//	清空位图
//============================================================ 
void Bitmap::clear() {
	texture.Bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
//============================================================
//	截图
//============================================================ 
#include <Sprite.h>
void Bitmap::screenshot() {
	Window::swap();
	int* p = new int[Window::get_width() * Window::get_height()];
	glReadPixels(0, 0, Window::get_width(), Window::get_height(), GL_RGBA, GL_UNSIGNED_BYTE, p);
	texture.Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::get_width(), Window::get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, p);	texture.Bind();
	width = Window::get_width();
	height = Window::get_height();
	delete p;
}
void Bitmap::Bind() {
	texture.Bind();
}

