#pragma once

void Game_flow();
extern float game_variables[20];

struct Game_port { 
	Game_port();
	~Game_port();
};
struct Game_data {
	static int mark;//分数
	static int hp;//残机
	static int sp;//炸弹
	static int pour;//火力
	static int point;//点数
	static int rank;//Rank
	static int degree;//难度
	static int random_seed;//随机数种子
};