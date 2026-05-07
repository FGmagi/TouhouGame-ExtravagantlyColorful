//============================================================
//	头文件引用
//============================================================
#include <stdafx.h>
#include <Window.h>
#include <Base.h>
#include <Sprite.h>
#include <Scene.h>
#include <Graphics.h>
#include <Debug.h>
//===========================================================================================
// 入口函数
//===========================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (setjmp(jump_buffer_1) == 0) {
		Base base;
		while (!Scene::empty()) {
			Scene::login();
			Scene::assign();
		}
		return 0;
	} else {
		return 1;
	}
};
