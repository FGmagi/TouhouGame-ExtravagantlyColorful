#include <stdafx.h>
#include <OpenGL.h>

unsigned VAO::global;
unsigned VBO::global;
unsigned FBO::global;
unsigned Texture::global;
//======================================================
// VAO
//======================================================
OpenGL::operator unsigned() { 
	return value; 
};

VAO::VAO() {
	value = 0;
	remark = new unsigned{ 1 };
	glGenVertexArrays(1, &value);
};
VAO::~VAO() {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteVertexArrays(1, &value);
		delete remark;
		if (global == value) {
			relieve();
		}
	}
};
VAO::VAO(const VAO& src) {
	value = src.value;
	remark = src.remark;
	*remark += 1;
};
VAO& VAO::operator=(const VAO& src) {//Ç³¿½±´
	*remark -= 1;
	if (*remark == 0) {
		glDeleteVertexArrays(1, &value);
		delete remark;
	}
	value = src.value;
	remark = src.remark;
	*remark += 1;
	return *this;
}
void VAO::Bind() {
	if (global != value) {
		global = value;
		glBindVertexArray(value);
	}
};
void VAO::relieve() {
	if (global != 0) {
		global = 0;
		glBindVertexArray(0);
	}
}
//======================================================
// VBO
//======================================================
VBO::VBO() {
	value = 0;
	remark = new unsigned{ 1 };
	glGenBuffers(1, &value);
};
VBO::~VBO() {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteBuffers(1, &value);
		delete remark;
		if (global == value) {
			relieve();
		}
	}
};
VBO::VBO(const VBO& src) {
	value = src.value;
	remark = src.remark;
	*remark += 1;
};
VBO& VBO::operator=(const VBO& src) {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteBuffers(1, &value);
		delete remark;
	}
	value = src.value;
	remark = src.remark;
	*remark += 1;
	return *this;
}
void VBO::Bind() {
	if (global != value) {
		global = value;
		glBindBuffer(GL_ARRAY_BUFFER, value);
	}
};
void VBO::relieve() {
	if (global != 0) {
		global = 0;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
//======================================================
// FBO
//======================================================
FBO::FBO() {
	value = 0;
	remark = new unsigned{ 1 };
	glGenFramebuffers(1, &value);
};
FBO::~FBO() {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteFramebuffers(1, &value);
		delete remark;
		if (global == value) {
			relieve();
		}
	}
};
FBO::FBO(const FBO& src) {
	value = src.value;
	remark = src.remark;
	*remark += 1;
};
FBO& FBO::operator=(const FBO& src) {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteFramebuffers(1, &value);
		delete remark;
	}
	value = src.value;
	remark = src.remark;
	*remark += 1;
	return *this;
}
void FBO::Bind() {
	if (global != value) {
		global = value;
		glBindFramebuffer(GL_FRAMEBUFFER, value);
	}
};
void FBO::relieve() {
	if (global != 0) {
		global = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
//======================================================
// Texture
//======================================================
Texture::Texture() {
	value = 0;
	remark = new unsigned{ 1 };
	glGenTextures(1, &value);
};
Texture::~Texture() {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteTextures(1, &value);
		delete remark;
		if (global == value) {
			relieve();
		}
	}
};
Texture::Texture(const Texture& src) {
	value = src.value;
	remark = src.remark;
	*remark += 1;
};
Texture& Texture::operator=(const Texture& src) {
	*remark -= 1;
	if (*remark == 0) {
		glDeleteTextures(1, &value);
		delete remark;
	}
	value = src.value;
	remark = src.remark;
	*remark += 1;
	return *this;
}
void Texture::Bind() {
	if (global != value) {
		global = value;
		glBindTexture(GL_TEXTURE_2D, value);
	}
};
void Texture::relieve() {
	if (global != 0) {
		global = 0;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
