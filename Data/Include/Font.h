#pragma once
#include <Base.h>

typedef unsigned short WORD;
class Font1;
class Message;
class Font {
public:
	Font();
	~Font();
	struct ivec2 {
		int w;
		int h;
	};
	struct ivec3 {
		int x;
		int y;
		int w;
	};
	static ivec3 find(WORD);
	friend Base;
	friend class Message;
	static const char* read_file(std::string& str, const char* src);
private:
	//绑定字库纹理
	static void Bind();
	//获取字库宽高度
	static Font::ivec2 get_wh();
};

