#include <stdafx.h>
#include "Bignum.h"
#include <Array.h>
#define MAXN 9999
#define MAXSIZE 50
#define DLEN 4
#define max(a,b) ((a>b) ? a: b)

class Bignum_ {
public:
	Array<int> a;
	bool symbol;
	Bignum_() :a(0,MAXSIZE),symbol(false){};
};

Bignum::Bignum() {
	ptr = new Bignum_;
}
Bignum::Bignum(long n) {
	ptr = new Bignum_;
}
Bignum::Bignum(const char* c) {
	ptr = new Bignum_;
}
Bignum::Bignum(Bignum& n) {
	ptr = new Bignum_;
	ptr->a = n.ptr->a;
	ptr->symbol = n.ptr->symbol;
}
Bignum::~Bignum() {
	delete ptr;
}
int Bignum::size() {
	return ptr->a.size();
}

Bignum& Bignum::operator=(Bignum& n) {
	ptr = new Bignum_;
	ptr->a = n.ptr->a;
	ptr->symbol = n.ptr->symbol;
	return *this;
};
Bignum& Bignum::operator+=(Bignum& n) {
	Array<int>& a = ptr->a;
	Array<int>& b = n.ptr->a;
	int m = max(size(), n.size());
	for (int i = 0; i < m; i++) {
		a[i] += b[i];
		if (a[i] > MAXN) {
			a[i + 1]++;
			a[i] -= MAXN + 1;
		}
	}
	return *this;
};
Bignum& Bignum::operator-=(Bignum& n) {
	return *this;
};
Bignum& Bignum::operator*=(Bignum& n) {
	return *this;
};
Bignum& Bignum::operator/=(Bignum& n) {
	return *this;
};

Bignum& Bignum::operator+(Bignum& n) {
	return *this;
};
Bignum& Bignum::operator-(Bignum& n) {
	return *this;
};
Bignum& Bignum::operator*(Bignum& n) {
	return *this;
};
Bignum& Bignum::operator/(Bignum& n) {
	return *this;
};
