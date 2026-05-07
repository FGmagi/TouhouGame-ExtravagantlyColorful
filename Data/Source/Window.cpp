//============================================================
//	头文件声明
//============================================================ 
#include <stdafx.h>
#pragma execution_character_set("utf-8")
#include <Window.h>
#include <thread>
#include <Debug.h>
#include <stb_image.h>

#define NAME "天花乱坠"
#define LNAME L NAME
#define ICONFILE "Graphics/Icons/Game_Icon.png"

static int win_width, win_height;
static GLFWwindow* glfw_hwindow;
static HWND win_hwnd;
//============================================================
//	数据结构
//============================================================
void callback(GLFWwindow*, int, int);
void set_wh(int w, int h) {
	win_width = w;
	win_height = h;
}
//============================================================
//	Window 程序窗口
//============================================================ 
Window::Window(unsigned w, unsigned h){
	//数据指针
	win_width = w;	win_height = h;
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	glfw_hwindow = glfwCreateWindow(w, h, NAME, NULL, NULL);
	if (glfw_hwindow == NULL) {
		print("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(glfw_hwindow);
	glfwSetFramebufferSizeCallback(glfw_hwindow, callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		print("Failed to initialize GLAD");
		glfw_hwindow = NULL;
		glfwTerminate();
	}

	//设置 Icon
	GLFWimage icon;	int channels;
	icon.pixels = stbi_load(ICONFILE, &icon.width, &icon.height, &channels, 0);
	glfwSetWindowIcon(glfw_hwindow, 1, &icon);
	if (icon.pixels != NULL) {
		stbi_image_free(icon.pixels);
	}
	setGL();
	win_hwnd = FindWindow(NULL, L"天花乱坠");
};
Window::~Window() {
	if (glfw_hwindow) {
		glfw_hwindow = NULL;
		glfwTerminate();
	}
};
bool Window::setGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//黑色背景
	glDepthFunc(GL_GEQUAL);		// Z大的在上
	glDepthRange(-1.0f, 1.0f);
	glEnable(GL_SCISSOR_TEST);//裁剪空间
	glScissor(0, 0, WIDTH,HEIGHT);

	glDisable(GL_DEPTH);//禁用深度缓存
	glEnable(GL_BLEND);//开启半透明混色
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//混色方案
	glfwSwapInterval(1);//设置FPS模式
	glfwSetWindowAttrib(glfw_hwindow, GLFW_RESIZABLE, GLFW_FALSE);//不可自主修改大小
	glfwSetWindowAttrib(glfw_hwindow, GLFW_RESIZABLE, GLFW_FALSE);
	return true;
};
//============================================================
//	Window 接口
//============================================================ 
void Window::resize(int mode) {
	static int win_mode = 0;
	if (win_mode == mode) {
		return;
	} else {
		win_mode = mode;
	}
	int x, y;
	GLFWmonitor* monitor = NULL;
	const GLFWvidmode* mo = NULL;
	glfwGetWindowPos(glfw_hwindow, &x, &y);
	switch (mode) {
	case -1:
		monitor = glfwGetPrimaryMonitor();
		mo = glfwGetVideoMode(monitor);
		win_width = mo->width;
		win_height = mo->height;
		glfwSetWindowMonitor(glfw_hwindow, monitor, 0, 0, mo->width, mo->height, mo->refreshRate);
		glfwSwapInterval(1);//设置FPS模式
		return;
	case 0:
		win_width = 1280;
		win_height = 960;
		break;
	case 1:
		win_width = 960;
		win_height = 720;
		break;
	case 2:
		win_width = 640;
		win_height = 480;
		break;
	}
	if (is_fullscreen()) {
		monitor = glfwGetPrimaryMonitor();
		mo = glfwGetVideoMode(monitor);
		x = (mo->width - win_width) / 2;
		y = (mo->height - win_height) / 2;
	}
	glfwSetWindowMonitor(glfw_hwindow, NULL, x, y, win_width, win_height, NULL);
	glfwSwapInterval(1);//设置FPS模式
};
void Window::swap() {
	glfwSwapBuffers(glfw_hwindow);
};
HWND Window::hwnd() {
	return win_hwnd;
};
bool Window::should_close() {
	return glfwWindowShouldClose(glfw_hwindow);
};
int Window::get_width() {
	return win_width;
}
int Window::get_height() {
	return win_height;
}
bool Window::is_fullscreen() {
	return glfwGetWindowMonitor(glfw_hwindow) != nullptr;
}
void callback(GLFWwindow* window, int width, int height) {
	set_wh(width, height);
	glScissor(0, 0, width, height);
	glViewport(0, 0, width, height);
}