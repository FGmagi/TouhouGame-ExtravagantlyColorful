#pragma once
class Bignum_;
class Bignum {
public:
	Bignum();
	Bignum(long);
	Bignum(const char*);
	Bignum (Bignum&);
	~Bignum();
	int size();

	Bignum& operator=(Bignum&);
	Bignum& operator+=(Bignum&);
	Bignum& operator-=(Bignum&);
	Bignum& operator*=(Bignum&);
	Bignum& operator/=(Bignum&);

	Bignum& operator+(Bignum&);
	Bignum& operator-(Bignum&);
	Bignum& operator*(Bignum&);
	Bignum& operator/(Bignum&);
private:
	Bignum_* ptr;
};

