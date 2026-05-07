//============================================================
//	Graphics 画面显示
//============================================================
#include <stdafx.h>
#include <Window.h>
#include <Canvas.h>
#include <Base.h>
#include <Graphics.h>
#include <Object.h>
#include <Input.h>
#include <Scene.h>
#include <Message.h>
#include <Scene_Begin.h>
#include <Thief.h>
#include <Debug.h>
//============================================================
//	数据结构
//============================================================
struct Light {
	int x, y, range;
	Color4 color;
}; 
class FPS {
public:
	FPS();
	int update();
	bool active;
	Message fps;//显示FPS
	DWORD time_now, time_recode;
	long code_now, code_recode;
};
struct Graphics1 {
public:
	//画面参数
	Color4 color;
	HSL hsl;
	std::map<int, Light> light;
	bool switches[3]; //光照,RGBA,HSL开关
	long Code_Frame_Count;//总帧数
	FPS fps;
	int wait_times;
	Canvas G;
	VAO vao;
}static *ptr;
void set_wnd_mode();
void Check_input(Graphics1* ptr);
//============================================================
//	画面刷新
//============================================================
Graphics::Graphics() {
	ptr = new Graphics1;
	ptr->switches[0] = 0;
	ptr->switches[1] = 0;
	ptr->switches[2] = 0;
	ptr->color.set(0.0f, 0.0f, 0.0f, 0.0f);
	ptr->Code_Frame_Count = 0;//总帧数
	ptr->fps.active = consts.F2_open;
	ptr->fps.fps.active = consts.F2_open;
}
Graphics::~Graphics() {
	delete ptr;
	ptr = NULL;
}
void Graphics::light(Light& l, int id) {
	ptr->light[id] = l;
};
void Graphics::color(Color4 color) {
	ptr->color = color;
	ptr->switches[1] = true;
}
void Graphics::color() {
	ptr->color.a = 0;
	ptr->switches[1] = false;
}
void Graphics::hsl(HSL hsl) {
	ptr->hsl = hsl;
	ptr->switches[2] = true;
}
void Graphics::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
};
long Graphics::code() {
	return ptr->Code_Frame_Count;
};
int& Graphics::wait_time() {
	return ptr->wait_times;
};

void Graphics::update(int times) {
	ptr->wait_times = times;
	while (ptr->wait_times) {
		if (ptr->fps.update() >= 15) {//刷新帧率
			ptr->wait_times--;
			//消息处理
			glfwPollEvents();
			//窗口关闭监测
			if (Window::should_close()) {
				longjmp(jump_buffer_1, 1);
			}
			//活动窗口监测
			Input::active(GetForegroundWindow() == Window::hwnd());
			Input::clear();	//清空输入
			Check_input(ptr);//部分输入检查
			set_wnd_mode();//设置全屏模式
			ptr->G.Bind();//切换G缓冲
			clear();		//清空画面
			Object::rend();	//对象渲染
			draw1();		//画面渲染
			FBO::relieve();	//切换主缓冲
			glViewport(0, 0, Window::get_width(), Window::get_height());
			clear();		//清空画面
			draw2();		//景象渲染
			Window::swap();	// 交换缓存 (双缓存)
			ptr->Code_Frame_Count++;//帧数++
		}
	};
};

//主缓冲画面渲染
void Graphics::draw1() {
	if (ptr->switches[0]) {//光照 x,y,r,g,b,a,range
		ptr->vao.Bind();
		Shader::code[1].Bind();
		#define fu(t) float(a.color.t)/255  
		for (auto i = ptr->light.begin(); i != ptr->light.end(); i++) {
			Light& a = i->second;
			Uniform(2f, light, a.x, a.y);
			Uniform(1f, Range, a.range);
			Uniform(4f, RGBA, fu(r), fu(g), fu(b), fu(a));
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
	}
	if (ptr->switches[1]) {//颜色叠加
		ptr->vao.Bind();
		Shader::code[2].Bind();
		Uniform(4f, RGBA, ptr->color.r / 255.0f, ptr->color.g / 255.0f, ptr->color.b / 255.0f, ptr->color.a / 255.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	if (ptr->switches[2]) {//色相转换
		ptr->vao.Bind();
		Shader::code[3].Bind();
		Uniform(3f, HSL, ptr->hsl.h, ptr->hsl.s, ptr->hsl.l);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}
THIEF_TEMPLATE(Steal,Canvas, Texture, texture);
void Graphics::draw2() {
	VAO vao;
	vao.Bind();
	Steal(ptr->G).Bind();
	Shader::code[4].Bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
void set_wnd_mode() {
	if ((Input::press(R_ALT) && Input::input(R_ENTER)) || (Input::press(R_ENTER) && Input::input(R_ALT))) {
		if (Window::is_fullscreen()) {
			Window::resize(0);
		} else {
			Window::resize(-1);
		}
		Window::swap();
		Input::block(R_ALT);
		Input::block(R_ENTER);
	}
}
void Check_input(Graphics1 *ptr) {
	if (Input::press(R_F12)) {// && !(typeid(*Scene::p1).name() == typeid(Scene_Begin).name())) {
		Scene::reset(new Scene_Begin);
		ptr->wait_times = 0;
	}
	if (Input::input(R_F2)) {
		consts.F2_open = !consts.F2_open;
		::ptr->fps.active = consts.F2_open;
		::ptr->fps.fps.active = consts.F2_open;
	}
}
//===========================================================================================
// Graphics1 数据管理
//===========================================================================================
FPS::FPS():fps(1240, 925,5000) {
	time_now = 0;
	time_recode = 0;
	code_now = 0;
	code_recode = 0;
	fps->color = get_color(0);
};
int FPS::update() {
	time_now = GetTickCount64();
	code_now = Graphics::code();
	int dt = time_now - time_recode;
	if (active) {
		if (dt >= 1000) {
			fps = int(round(float(code_now - code_recode) / dt * 1000.0f));
			time_recode = time_now;
			code_recode = code_now;
		}
	}
	return dt;
}