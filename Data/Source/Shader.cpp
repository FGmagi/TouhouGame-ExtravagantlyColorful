//============================================================
//	头文件声明
//============================================================
#include <stdafx.h>
#define SHADER_MAX 8 //着色器数量
#define prefix "./Data/Shader/"
#define Add_Prefix(dst,str,size) char dst[size] = prefix;memcpy(&dst[sizeof(prefix) - 1], str, size - sizeof(prefix) + 1);
#include <Shader.h>
#include <fstream>
#include <sstream>
#include <Debug.h>
//============================================================
//	数据结构
//============================================================
unsigned Shader::global;
Shader& Shader_code::operator[](int n) {
	return ptr[n];
}
Shader_code Shader::code;
//============================================================
//	着色器实现
//============================================================
Shader_port::Shader_port() {
	Shader::code.ptr = new Shader[SHADER_MAX];
	Shader::code.ptr[0].reset("A0-基础着色器", "A0-基础着色器");
	Shader::code.ptr[1].reset("A1-矩形着色器", "A1-矩形着色器");
	Shader::code.ptr[2].reset("A2-颜色叠加器", "A2-颜色叠加器");
	Shader::code.ptr[3].reset("A3-色相转换器", "A3-色相转换器");
	Shader::code.ptr[4].reset("A4-画面渲染器", "A4-画面渲染器");
	Shader::code.ptr[5].reset("A5-文字加载器", "A5-文字加载器");
	Shader::code.ptr[6].reset("A6-精灵渲染集", "A6-精灵渲染集");
	Shader::code.ptr[7].reset("A0-基础着色器", "A7-遮罩处理");

	Shader::code.ptr[0].Bind();
}
Shader_port::~Shader_port() {
	delete[] Shader::code.ptr;
}
Shader::Shader() {
	value = 0;
	remark = NULL;
}
bool Shader::empty() {
	return (value == 0 && remark == NULL);
}
Shader::Shader(const char* str1, const char* str2){
	value = 0;
	remark = NULL;
	reset(str1, str2);
	remark = new unsigned{ 1 };
}
Shader::Shader(const Shader& src) {//浅拷贝
	if (src.remark != NULL) {
		value = src.value;
		remark = src.remark;
		*remark += 1;
	} else {
		value = 0;
		remark = NULL;
	}
};
Shader& Shader::operator=(const Shader& src) {//浅拷贝
	if (remark != NULL) {
		*remark -= 1;
		if (*remark == 0) {
			glDeleteProgram(value);
			delete remark;
			remark = NULL;
		}
	}
	if (src.remark != NULL) {
		value = src.value;
		remark = src.remark;
		*remark += 1;
	} else {
		value = 0;
		remark = NULL;
	}
	return *this;
}
void Shader::reset(const char* str1, const char* str2) {
	if (remark != NULL) {//更改
		*remark -= 1;
		if (*remark == 0) {
			glDeleteProgram(value);
			delete remark;
			remark = NULL;
		}
	}
	// 0. 添加路径前缀
	unsigned int vertex, fragment;
	std::string vertexPath(prefix);
	std::string fragmentPath(prefix);
	vertexPath += str1; vertexPath += ".vert";
	fragmentPath += str2; fragmentPath += ".frag";
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		print("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertex, 1, &vShaderCode, NULL);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		print_e(infoLog);
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		print_e(infoLog);
	}
	// Shader Program
	this->value = glCreateProgram();
	glAttachShader(this->value, vertex);
	glAttachShader(this->value, fragment);
	glLinkProgram(this->value);
	// Print linking errors if any
	glGetProgramiv(this->value, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->value, 512, NULL, infoLog);
		print_e(infoLog);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	remark = new unsigned{ 1 };
};
// 析构函数
Shader::~Shader() {
	if (remark != 0) {
		*remark -= 1;
		if (*remark == 0) {
			glDeleteProgram(value);
			delete remark;
			remark = NULL;
		}
	}
};
//选择着色器程序
void Shader::Bind() {
	if (global != value) {
		global = value;
		glUseProgram(value);
	}
};