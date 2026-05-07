//============================================================
//	轻量级智能指针，可延后初始化（懒汉模式）
//============================================================
#pragma once
#define NULL 0
template <typename T>
class Ref {
public:
	Ref() {
		ptr = NULL;
	};
	~Ref() {
		dispose();
	};
	operator bool () {
		return (bool)(ptr);
	}
	T& operator *() {
		if (!ptr) initial();
		return (*ptr);
	}
	T* operator ->() {
		if (!ptr) initial();
		return ptr;
	}
	bool empty() {
		return (ptr == NULL);
	}
	void initial() {
		dispose();
		ptr = new T();
	};
	template <typename... U>
	void initial(U... arg) {
		dispose();
		ptr = new T(arg...);
	}
	Ref& operator = (T *i) {
		dispose();
		ptr = i;
		return *ptr;
	};
	void dispose() {
		if (ptr != NULL) {
			delete ptr;
			ptr = NULL;
		}
	};
private:
	Ref(const Ref<T>& i) = delete;
	T* ptr;
};

