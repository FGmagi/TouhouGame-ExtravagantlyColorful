//============================================================
//	NUM数字类
//============================================================
#pragma once
template <class T>
class NUM {
public:
	T value;
	T recode;
	bool revise;
	// 构造函数
	NUM() {
		value = 0;
		recode = 0;
		revise = false;
	};
	NUM(const NUM& i) {
		value = i.value;
		recode = 0;
		revise = true;
	};
	// 以整数形式返回
	operator T() {
		return value;
	}
	// 数值不一致则返回真，请配合assign更正
	bool judge() {
		return (revise);
	};
	void assign(T i) {
		value = i;
		recode = i;
		revise = false;
	};
	void assign() {
		recode = value;
		revise = false;
	};

	T& operator=(T i) {
		recode = value;
		value = i;
		if (value != recode) {
			revise = true;
		}
		return value;
	};
	bool operator==(T other) {
		return (value == other);
	};
	bool operator!=(T other) {
		return (value != other);
	};

	T& operator+=(T i) {
		recode = value;
		value += i;
		if (value != recode) {
			revise = true;
		}
		return value;
	};
	T& operator-=(T i) {
		recode = value;
		value -= i;
		if (value != recode) {
			revise = true;
		}
		return value;
	};
	T& operator*=(T i) {
		recode = value;
		value *= i;
		if (value != recode) {
			revise = true;
		}
		return value;
	};
	T& operator/=(T i) {
		recode = value;
		value /= i;
		if (value != recode) {
			revise = true;
		}
		return value;
	};
	T* operator&() {
		return &value;
	};
	//============================================================
	//	自增value
	//============================================================
	T& operator++() {
		recode = value;
		++value;
		revise = true;
		return value;
	};
	T& operator--() {
		recode = value;
		--value;
		revise = true;
		return value;
	};
	T& operator++(int) {
		recode = value;
		++value;
		revise = true;
		return recode;
	};
	T& operator--(int) {
		recode = value;
		--value;
		revise = true;
		return recode;
	};
};
