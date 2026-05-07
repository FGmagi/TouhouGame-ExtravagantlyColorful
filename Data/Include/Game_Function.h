#pragma once
#include <Base.h>
#include <vector>
#include <cmath>
#include <tuple>
#define pow(a) ((a)*(a))
void missile(int id, vec2 r, std::vector<float> par);//敌机发弹
template <int N,class ...T> void locus(T...);

//计算欧氏距离
inline float Euclidean(vec2 a, vec2 b) {//输入两坐标
	float distance = sqrt(pow(a.x - b.x) + pow(a.y - b.y));
	return distance;
};
inline float Euclidean(vec2 d) {//输入差值
	float distance = sqrt(pow(d.x) + pow(d.y));
	return distance;
};
//计算曼哈顿距离
inline float Manhattan(vec2 a, vec2 b) {//输入两坐标
	float distance = (fabs(a.x - b.x) + fabs(a.y - b.y));
	return distance;
}
inline float Manhattan(vec2 d) {//输入差值
	float distance = (fabs(d.x) + fabs(d.y));
	return distance;
}
//旋转向量 Dst为目标向量，S为旋转角度的正弦值，C为余弦值
inline void change(vec2& Dst, float S, float C) {
	Dst = { Dst.x * float(C) - Dst.y * float(S),Dst.x * float(S) + Dst.y * float(C) };
}
