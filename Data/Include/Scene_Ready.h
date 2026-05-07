//============================================================
//	头文件引用
//============================================================
#pragma once
#include <Scene.h>
class Window_Selection;
class Sprite;
//============================================================
//	选择界面 Scene_Ready
//============================================================
class Scene_Ready :public Scene {
public:
	//基本函数
	Scene_Ready();
	~Scene_Ready() {};
	void main();
private:
	S_Ptr<Sprite> background = std::make_shared<Sprite>(ZH_GROUND("A-标题背景"), 0, 0, 0);
	S_Ptr<Window_Selection> selection = std::make_shared<Window_Selection>(4);
	S_Ptr<Sprite> canvas[3][4];
};
