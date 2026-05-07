//============================================================
//	Complex ¿‡ 
//============================================================
#pragma once
template <typename T> class Complex {
public:
	T real, imag;
	Complex() {
		real = 0;
		imag = 0;
	};
	Complex(const Complex& complex) {
		real = complex.real;
		imag = complex.imag;
	};
	Complex(const Complex&& complex) {
		real = complex.real;
		imag = complex.imag;
	};
	Complex(T a) {
		real = a;
		imag = 0;
	};
	Complex(T a, T b) {
		real = a;
		imag = b;
	};
	float magnitude() {
		return sqrt(float(real * real + imag * imag));
	}

	Complex& operator=(const Complex& complex) {
		real = complex.real;
		imag = complex.imag;
		return *this;
	}
	bool operator==(const Complex& complex) {
		return (real == complex.real && imag == complex.imag);
	}
	bool operator!=(const Complex& complex) {
		return (real != complex.real || imag != complex.imag);
	}

	Complex& operator+=(const Complex& complex) {
		real += complex.real;
		imag += complex.imag;
		return *this;
	}
	Complex& operator-=(const Complex& complex) {
		real -= complex.real;
		imag -= complex.imag;
		return *this;
	}
	Complex& operator*=(const Complex& complex) {
		T temp = real * complex.real - imag * complex.imag;
		imag = real * complex.imag + imag * complex.real;
		real = temp;
		return *this;
	}

	Complex operator+(const Complex& complex) {
		Complex temp;
		temp.real = real + complex.real;
		temp.imag = imag + complex.imag;
		return temp;
	}
	Complex operator-(const Complex& complex) {
		Complex temp;
		temp.real = real - complex.real;
		temp.imag = imag - complex.imag;
		return temp;
	}
	Complex operator*(const Complex& complex) {
		Complex temp;
		temp.real = real * complex.real - imag * complex.imag;
		temp.imag = real * complex.imag + imag * complex.real;
		return temp;
	}
};
