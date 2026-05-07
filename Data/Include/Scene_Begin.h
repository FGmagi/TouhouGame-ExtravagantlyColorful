//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Scene.h>
#include <Rect.h>
class Effect;
class Sprite;
class Model;
class Window_Selection;
class Message;
//===========================================================================================
// Window_Begin 开幕画面
//===========================================================================================
class Scene_Begin : public Scene {
public:
	Scene_Begin();
	~Scene_Begin();
	void son_loop();
	void main();
private:
	Sprite* background = MAKE<Sprite>(ZH_GROUND("A-标题背景"), 0, 0, 0);
	S_Ptr<Message> message[3];
	Window_Selection* selection = MAKE<Window_Selection>(3);
};
