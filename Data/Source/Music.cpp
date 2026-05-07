// 头文件声明
#include <stdafx.h>
#include <Music.h>
#include <Effect.h>
#include <Base.h>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

typedef struct {
	const wchar_t* src;
	float volume;
}box;
ISoundSource* get_bgm(int i, float volume_double);
ISoundSource* get_se(int i);
extern box BOX_MAP[];
static ISound* BGM_IS;
static int BGM_ID;
static irrklang::ISoundEngine* engine;
static S_Ptr<Effect> effect;
#define FT 40
class Fun_in :public Functor {
public:
	Fun_in() {};
	void update() {
		if (times == 0) {
			irrklang::ISoundSource* shootSound = get_bgm(BGM_ID, 0.0f);
			BGM_IS = engine->play2D(shootSound, true,false,true);
			times++;
		}else
		if (times < FT) {
			BGM_IS->setVolume(consts.BGM_volume * BOX_MAP[BGM_ID].volume * 0.01f * times / FT);
			times++;
		}else
		if (times == FT) {
			BGM_IS->setVolume(consts.BGM_volume * BOX_MAP[BGM_ID].volume * 0.01f * times / FT);
			times++;
		}
	}
	int times = 0;
};
class Fun_out :public Functor {
public:
	Fun_out(int i) {
		to_id = i;
	};
	void update() {
		if (times == 0) {
			times++;
		}else
		if (times < FT) {
			BGM_IS->setVolume(consts.BGM_volume * BOX_MAP[BGM_ID].volume * 0.01f * (FT -times) / FT);
			times++;
		}else
		if (times == FT) {
			times++;
			BGM_IS->stop();
			BGM_IS->drop();
			BGM_ID = to_id;
			mul_effect->reset(new Fun_in);
		}
	}
	int to_id;
	int times = 0;
};

void Music::BGM(int i) {
	if (i == BGM_ID) return;
	if (engine == NULL) {
		engine = irrklang::createIrrKlangDevice();
		effect.reset(new Effect(0));
	}
	if (BGM_IS) {
		(*effect).reset(new Fun_out(i));
	} else {
		BGM_ID = i;
		(*effect).reset(new Fun_in());
	}
};

void Music::SE(int i) {
	if (engine == NULL) {
		engine = irrklang::createIrrKlangDevice();
		effect.reset(new Effect(0));
	}
	irrklang::ISoundSource* shootSound = get_se(i);
	engine->play2D(shootSound,false);
};

// id:序号 a名字 v音量
static ISoundSource* shootSound[4];
static box BOX_MAP[4] = {
	{L"",0},//0
	{L"Music/BGM/Novus Initium.mp3", 0.7f},//1
	{L"Music/BGM/Ice and Snow.mp3", 0.8f },//2
	{L"Music/BGM/Find Them.mp3", 0.9f},//3
};
ISoundSource* get_bgm(int i, float volume_double) {
	if (shootSound[1] == NULL) {
		char dstbuffer[100];
		for (int i = 1; i < 4; i++) {
			irrklang::makeUTF8fromUTF16string(BOX_MAP[i].src, dstbuffer, 100);
			shootSound[i] = engine->addSoundSourceFromFile(dstbuffer);
			shootSound[i]->setDefaultVolume(consts.BGM_volume * BOX_MAP[i].volume * 0.01f);
		}
	} else {
		shootSound[i]->setDefaultVolume(consts.BGM_volume * BOX_MAP[i].volume * 0.01f * volume_double);
	}
	return shootSound[i];
};
void Music::BGM_Volume(int i) {
	consts.BGM_volume = i;
	for (int j = 1; j < 4; j++) {
		if (shootSound[j])
			shootSound[j]->setDefaultVolume(consts.BGM_volume * BOX_MAP[j].volume * 0.01f);
	}
	if (BGM_IS) {
		BGM_IS->setVolume(consts.BGM_volume * BOX_MAP[BGM_ID].volume * 0.01f);
	}
};
#define num_of_se 15
static ISoundSource* shootSound2[num_of_se];
static box BOX_MAP2[num_of_se] = {
	{L"",0},//0
	{L"Music/SE/选择 系统音.wav", 0.5f},//1
	{L"Music/SE/确定 系统音.wav", 0.5f },//2
	{L"Music/SE/取消 系统音.wav", 0.5f},//3
	{L"Music/SE/炸裂.wav", 0.2f},//4
	{L"Music/SE/擦弹.wav", 0.3f},//5
	{L"Music/SE/击破boss.wav",0.5f},//6
	{L"Music/SE/biu~.wav", 0.5f},//7
	{L"Music/SE/残机1up.wav",0.5f},//8
	{L"Music/SE/暂停菜单.wav", 0.6f},//9
	{L"Music/SE/获得Item.wav",0.15f },//10
	{L"Music/SE/命中.wav",0.0f },//11
	{L"Music/SE/BOOM1.ogg",1.0f },//12
	{L"Music/SE/BOOM2.ogg",1.0f },//13
	{L"Music/SE/倒计时.ogg",0.6f },//14
};
ISoundSource* get_se(int i){
	if (shootSound2[1] == NULL) {
		char dstbuffer[100];
		for (int i = 1; i < num_of_se; i++) {
			irrklang::makeUTF8fromUTF16string(BOX_MAP2[i].src, dstbuffer, 100);
			shootSound2[i] = engine->addSoundSourceFromFile(dstbuffer);
			shootSound2[i]->setDefaultVolume(consts.SE_volume * BOX_MAP2[i].volume * 0.01f);
		}
	}
	return shootSound2[i];
};
void Music::SE_Volume(int i) {
	consts.SE_volume = i;
	for (int j = 1; j < num_of_se;j++) {
		if (shootSound2[j])
			shootSound2[j]->setDefaultVolume(consts.SE_volume * BOX_MAP2[j].volume * 0.01f);
	}
};
