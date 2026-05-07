//============================================================
//	OpenGL 相关对象
//============================================================
#pragma once
#include <Shader.h>
class OpenGL {
public:
	operator unsigned();
	virtual void Bind() = 0;
protected:
	unsigned value;
	unsigned* remark;
};

class VAO:public OpenGL {
public:
	VAO();
	~VAO();
	VAO(const VAO& src);//浅拷贝
	VAO& operator=(const VAO& src);//浅拷贝
	void Bind();
	static void relieve();
	static unsigned global;
};

class VBO :public OpenGL {
public:
	VBO();
	~VBO();
	VBO(const VBO& src);
	VBO& operator=(const VBO& src);
	void Bind();
	static void relieve();
	static unsigned global;
};

class FBO :public OpenGL {
public:
	FBO();
	~FBO();
	FBO(const FBO& src);
	FBO& operator=(const FBO& src);
	void Bind();
	static void relieve();
	static unsigned global;
};

class Texture :public OpenGL {
public:
	Texture();
	~Texture();
	Texture(const Texture& src);
	Texture& operator=(const Texture& src);
	void Bind();
	static void relieve();
	static unsigned global;
};
