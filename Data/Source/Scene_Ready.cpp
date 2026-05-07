//============================================================
//	角色选择界面 Scene_Ready
//============================================================
#include <stdafx.h>
#include <Scene_Ready.h>
#include <Scene_Begin.h>
#include <Graphics.h>
#include <Effect.h>
#include <Sprite.h>
#include <Input.h>
#include <Window_Selection.h>
#include <Actor.h>
#include <Music.h>
#include <Scene_Map.h>
#include <Game_Map.h>
//===========================================================================================
// 主循环
//===========================================================================================
void Scene_Ready::main() {
	{
		Effect_Froze(20, { 0,0,0,0 });
		Graphics::update(20);
	}
	/*S_Ptr<Sprite> ttt;
	Bitmap kkk(ZH_Bullet("bullet3"));
	ttt.reset(new Sprite(kkk, 0, 0, 50));*/
	do {
		Graphics::update();
		auto mes = selection->update();
		if (mes.recode != mes.tax) {
			Music::SE(1);
			(canvas[0][mes.recode])->active = true;
			(canvas[1][mes.recode])->active = false;
			(canvas[0][mes.tax])->active = false;
			(canvas[1][mes.tax])->active = true;
		}
		if (INPUT2_C) {
			Music::SE(2);
			Game_data::degree = mes.message + 1;
			Effect_Froze(20, { 0,0,0,255 });
			Graphics::update(20);
			Scene::reset(new Scene_Map);
		}
		if (INPUT_B) {
			Music::SE(3);
			Effect_Froze(20, { 0,0,0,255 });
			Graphics::update(20);
			Scene::reset(new Scene_Begin);
		}

	} while (Scene::judge(this));
}
//===========================================================================================
// 构造函数
//===========================================================================================
Scene_Ready::Scene_Ready() {
	for (int i = 0; i < 4; i++) {
		canvas[0][i] = std::make_shared<Sprite>(ZH_TITLE("B-灰底"), 87 + 36 * i, 100 + 191 * i, 1);
		canvas[1][i] = std::make_shared<Sprite>(ZH_TITLE("B-白底"), 87 + 36 * i, 100 + 191 * i, 1);
		canvas[1][i]->active = false;
	}
	canvas[0][0]->active = false;
	canvas[1][0]->active = true;

	canvas[2][0] = std::make_shared<Sprite>(ZH_TITLE("B-EASY"), 87 + 36 * 0, 100 + 191 * 0, 2);
	canvas[2][1] = std::make_shared<Sprite>(ZH_TITLE("B-NORMAL"), 87 + 36 * 1, 100 + 191 * 1, 2);
	canvas[2][2] = std::make_shared<Sprite>(ZH_TITLE("B-HARD"), 87 + 36 * 2, 100 + 191 * 2, 2);
	canvas[2][3] = std::make_shared<Sprite>(ZH_TITLE("B-LUNATIC"), 87 + 36 * 3, 100 + 191 * 3, 2);
}