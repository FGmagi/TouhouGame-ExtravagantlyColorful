//============================================================
//	着色器实现
//============================================================
#pragma once
#include <Base.h>
#define Uniform(type,name,...) glUniform##type(glGetUniformLocation(Shader::global, #name),__VA_ARGS__)
class Shader;
//着色器GC接口
class Shader_port {
	Shader_port();
	~Shader_port();
	friend Base;
};
//核心着色器
class Shader_code {
public:
	Shader& operator[](int);
	friend class Shader_port;
	friend class Shader;
private:
	Shader* ptr;
};
//普通着色器
class Shader {
public:
	static Shader_code code;
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const Shader& src);//浅拷贝
	Shader& operator=(const Shader& src);//浅拷贝
	bool operator==(const Shader& src) { return (value == src.value); };//对比
	~Shader();
	bool empty();
	operator unsigned() { return value; };
	void Bind();
	void reset(const char* vertexPath, const char* fragmentPath);
	static unsigned global;
private:
	unsigned value;
	unsigned* remark;
};