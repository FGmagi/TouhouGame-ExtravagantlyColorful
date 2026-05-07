//============================================================
//	头文件声明
//============================================================
#include <stdafx.h>
#include <Scene.h>
#include <Scene_Begin.h>
#include <typeinfo>
S_Ptr<Scene> Scene::p1,Scene::p2;
//============================================================
//	数据结构
//============================================================
Scene_port::Scene_port() {
	Scene::p1.reset(new Scene_Begin);
	Scene::p2 = Scene::p1;
}
Scene_port::~Scene_port() {
	Scene::p1.reset();
	Scene::p2.reset();
}
//===========================================================================================
// Scene 界面类
//===========================================================================================
void Scene::reset(Scene* p) {
	p1.reset(p);
}
void Scene::reset() {
	p1.reset();
}
void Scene::assign() {
	if (p2 != p1) {
		p2 = p1;
	}
}
void Scene::login() {
	p1->main();
}
bool Scene::empty() {
	return (p1 == NULL);
}
bool Scene::judge(Scene* p) {
	return (p1 == p2);
}
bool Scene::operator==(Scene* p) {
	return (typeid(*this).name() == typeid(*p).name());
};
Scene::~Scene() {};