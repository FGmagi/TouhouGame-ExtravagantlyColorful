//显示字体设置
#include <stdafx.h>
#include <Message.h>
#include <Font.h>
#include <OpenGL.h>
#include <Canvas.h>

typedef unsigned char uc;
int fuck(std::string str);
inline WORD get_char(std::string::iterator& ch);

Message::Message(int tx, int ty, int tz) :
	Object(tz) {
	active = true;
	x.assign(tx);
	y.assign(ty);

	ptr = new Message_Box;
	leght = 0;
	revise = false;

	ptr->width = -1;
	ptr->height = -1;
	ptr->color.set(255,255,255,255);
	ptr->left = 0;
	ptr->right = 0;
	ptr->top = 0;
	ptr->bottom = 0;
	ptr->texsp = 0;
	ptr->rowsp = 0;
	ptr->font_size = 24;
	ptr->italic = false;
	ptr->slender = false;
	ptr->bold = 0;
	ptr->outlinecolor.set(255, 255, 255, 255);

	//位置、纹理均采用左上右下坐标处理，颜色使用rgba
	vao.Bind();
	vbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, (12 * 1) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);//预留位置
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));//位置
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));//纹理
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));//颜色
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	vao.relieve();
}
Message::Message(const char* str, int tx, int ty, int tz) :
	Object(tz) {
	active = true;
	x.assign(tx);
	y.assign(ty);

	ptr = new Message_Box;
	leght = 0;
	revise = false;

	ptr->width = -1;
	ptr->height = -1;
	ptr->color.set(255, 255, 255, 255);
	ptr->left = 0;
	ptr->right = 0;
	ptr->top = 0;
	ptr->bottom = 0;
	ptr->texsp = 0;
	ptr->rowsp = 0;
	ptr->font_size = 24;
	ptr->italic = false;
	ptr->slender = false;
	ptr->bold = 0;
	ptr->outlinecolor.set(255, 255, 255, 255);

	//位置、纹理均采用左上右下坐标处理，颜色使用rgba
	vao.Bind();
	vbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, (12 * 1) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);//预留位置
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));//位置
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));//纹理
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));//颜色
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	vao.relieve();
	*this = (str);
}

Message::~Message(){
	delete ptr;
}

int Message::get_leght() {
	return leght;
}

void Message::produce() {
	Font::ivec2 Font_Map = Font::get_wh();//获取字库大小
	const int width = (ptr->width < 0) ? 999999 : ptr->width;//文字框宽度
	const int height = (ptr->height < 0) ? 999999 : ptr->height;//文字框高度

	int font_size = ptr->font_size;//字体大小
	float texsp = ptr->texsp;//间距
	float rowsp = ptr->rowsp;//行距
	Color4 color = ptr->color;//字体颜色

	ivec2 pos = { ptr->left,ptr->top };//文字相对左上角坐标
	Font::ivec3 key;//纹理坐标（计算）
	WORD character = 0;//字
	float* vertices = new float[12 * text.size()];//GPU数据

	int last_w = 0;//上一字节占位
	int guess_w = 0;//预计占位
	leght = 0;//当前已写总字数（叠加）

	for (auto ch = text.begin(); ch != text.end();) {
		//检测标志符
		if (*ch == *"\n"){
			pos.x = ptr->left;
			pos.y += font_size * (1 + rowsp);
			last_w = 0;
			ch++;
			continue;
		}
		//特殊字符
		if (*ch == *"\\") {
			ch++;
			if (*ch == *"C" || *ch == *"c") {
				if (*ch == *"[") {
					int i = 0; ch++;
					if (*ch != *"]") i = i + *ch - *"0"; ch++;
					if (*ch != *"]") i = i * 10 + *ch - *"0"; ch++;
					if (*ch != *"]") i = i * 10 + *ch - *"0"; ch++;
					color = get_color(i);
					continue;
				}
			}
			if (*ch == *"S" || *ch == *"s") {
				if (*ch == *"[") {
					int i = 0; ch++;
					if (*ch != *"]") i = i + *ch - *"0"; ch++;
					if (*ch != *"]") i = i * 10 + *ch - *"0"; ch++;
					if (*ch != *"]") i = i * 10 + *ch - *"0"; ch++;
					font_size = i;
					continue;
				}
			}
		}
		//获取字符
		character = get_char(ch);
		key = Font::find(character);
		//计算位置
		guess_w = (float(font_size) * key.w / 48);//预计占位
		pos.x += last_w * (1 + texsp);
		if (pos.x >= width - ptr->right) {//换行
			pos.y += font_size * (1 + rowsp);
			pos.x = ptr->left;
		}
		if (pos.y >= height - ptr->bottom) {
			break;
		}
		last_w = guess_w;
		
		int kx = pos.x + x;
		int ky = pos.y + y;
		//坐标
		float fs = float(font_size) * key.w / 48;
		int gs = (key.y == 0) ? float(font_size) * 52 / 48 : font_size;
		vertices[leght * 12] = normal_x(kx, Canvas::get_width());
		vertices[leght * 12 + 1] = normal_y(-ky, Canvas::get_height());
		vertices[leght * 12 + 2] = vertices[leght * 12] + (fs / WIDTH * 2);
		vertices[leght * 12 + 3] = vertices[leght * 12 + 1] - (float(gs) / HEIGHT * 2);
		//纹理
		int ts = (key.y == 0) ? 52 : 48;
		vertices[leght * 12 + 4] = (float)(key.x) / Font_Map.w;
		vertices[leght * 12 + 5] = (float)(Font_Map.h - key.y ) / Font_Map.h;
		vertices[leght * 12 + 6] = (float)(key.x + key.w) / Font_Map.w;
		vertices[leght * 12 + 7] = (float)(Font_Map.h - key.y - ts) / Font_Map.h;
		//颜色
		vertices[leght * 12 + 8] = (float)color.r / 255;
		vertices[leght * 12 + 9] = (float)color.g / 255;
		vertices[leght * 12 + 10] = (float)color.b / 255;
		vertices[leght * 12 + 11] = (float)color.a / 255;

		//字数迭代
		leght++;
	}
	//写入GPU
	vao.Bind();
	vbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, 12 * leght * sizeof(GLfloat), vertices, GL_STATIC_DRAW);//预留位置
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));//位置
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));//纹理
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));//颜色
	vao.relieve();
	revise = false;
	delete vertices;
}

void Message::update(){
	if (active && !text.empty()) {
		Shader::code[5].Bind();
		if (revise || x.judge() || y.judge()) {
			x.assign();
			y.assign();
			produce();
		}
		Uniform(1i, italic, int(ptr->italic));
		Uniform(1i, bold, int(ptr->bold));
		Uniform(1i, slender, int(ptr->slender));
		Uniform(4f, outlinecolor, float(ptr->outlinecolor.r) / 255, float(ptr->outlinecolor.g) / 255, float(ptr->outlinecolor.b) / 255, float(ptr->outlinecolor.a) / 255);
		vao.Bind();
		Font::Bind();
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4,leght);
		vao.relieve();
	}
}
void Message::clear() {
	text.clear();
}
void Message::read_file(const char* str) {
	Font::read_file(text,str);
	produce();
}

Message::Message_Box* Message::operator ->() {
	revise = true;
	return ptr;
}

//载入文字
std::string& Message::operator *() {
	return text;
};
void Message::operator=(int a) {
	clear();
	text = std::to_string(a);
	produce();
}
void Message::operator=(long a) {
	clear();
	text = std::to_string(a);
	produce();
}
void Message::operator=(char a) {
	clear();
	text = a;
	produce();
}
void Message::operator=(const char* a) {
	clear();
	text = a;
	produce();
}
void Message::operator=(std::string& a) {
	clear();
	text = a;
	produce();
}

void Message::operator+=(int a) {
	clear();
	text += std::to_string(a);
	produce();
}
void Message::operator+=(long a) {
	clear();
	text += std::to_string(a);
	produce();
}
void Message::operator+=(char a) {
	clear();
	text += a;
	produce();
}
void Message::operator+=(const char* a) {
	clear();
	text += a;
	produce();
}
void Message::operator+=(std::string& a) {
	clear();
	text += a;
	produce();
}

int fuck(std::string str) {
	int n = 0;
	for (auto i = str.begin(); i != str.end();) {
		if (uc(*i) <= 127) {
			n++; i++;
		} else {
			n++;
			i += 2;
		}
	}
	return n;
}
WORD get_char(std::string::iterator& ch) {
	WORD character;
	if (uc(*ch) <= 127) {
		character = uc(*ch);
		ch++;
	} else {
		character = uc(*ch) << 8; ch++;
		character |= uc(*ch); ch++;
	}
	return character;
};