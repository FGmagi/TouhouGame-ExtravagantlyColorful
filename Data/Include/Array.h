//============================================================
//	头文件引用
//============================================================
#pragma once
template <typename T>
inline void exchange(T& a,T& b) {
	T temp = a;
	a = b;
	b = temp;
}
//不建议存储唯一指针，存储建议手动delete
template <typename T>
class Array {
public:
	// [] 内为默认值
	//大小n[0]，容量c[10+]
	Array(int n = 0, int c = 10) {
		if (n > c) c = n;
		this->size_ = n;
		capacity = c;
		ptr = new T[c];
		memset(ptr, 0, sizeof(ptr));
	};
	//大小n，填充t，容量c[10+]
	Array(int n, T& t, int c = 10):Array(n,c) {
		for (int i = 0; i < size_; i++) {
			ptr[i] = t;
		}
	};
	//存入对象唯一指针，析构前务必手动delete
	~Array() {
		delete[] ptr;
	};
	//添加到末尾
	Array& push_back(T& n) {
		if (size_ == capacity) {
			increment(2 * size_);
		}
		ptr[size_] = n;
		size_++;
		return *this;
	};
	Array& push_back(Array& src) {
		if (capacity < size_ + src.size_) {
			increment(size_ + src.size_);
		}
		for (int i = size_; i < size_ + src.size_; i++) {
			ptr[i] = src.ptr[i-size_];
		}
		size_ = size_ + src.size_;
		return *this;
	};
	//插入到某下标
	Array& insert(int n, T& src) {
		if (size_ + 1 <= capacity) {
			for (int i = size_; i > n; i--) {
				ptr[i] = ptr[i - 1];
			}
			ptr[n] = src;
			size_++;
			return *this;
		} else {
			capacity = (2 * size_);
			T* temp = new T[capacity];
			memcpy(temp, ptr, sizeof(T) * n);
			temp[n] = src;
			memcpy(&temp[n + 1], &ptr[n], sizeof(T) * (capacity - n));
			delete ptr;
			ptr = temp;
			size_++;
			return *this;
		}
	};
	Array& insert(int n, Array& src) {
		if (capacity >= size_ + src.size_) {
			for (int i = size_ + src.size_; i >= n + src.size_; i--) {
				ptr[i] = ptr[i - src.size_];
			}
			for (int i = n; i < n + src.size_; i++) {
				ptr[i] = src.ptr[i - n];
			}
			size_ += src.size_;
			return *this;
		} else {
			if (capacity < size_ + src.size_) {
				capacity = (size_ + src.size_);
			}
			T* temp = new T[capacity];
			memcpy(temp, ptr, sizeof(T) * n);
			memcpy(&temp[n], src, sizeof(T) * src.size_);
			memcpy(&temp[n + src.size_], &ptr[n], sizeof(T) * (ptr.capacity - n - src.size_));
			delete ptr;
			ptr = temp;
			size_ += src.size_;
			return *this;
		}
	};
	//遍历获取目标，没找到返回-1
	int find(T& dst) {
		for (int i = 0; i < size_; i++) {
			if (ptr[i] == dst) {
				return i;
			}
		}
		return -1;
	};
	//删除末尾
	void pop_back() {
		ptr[size_] = NULL;
		size_--;
	};
	//删除指定下标
	void del(int n) {
		for (int i = n; i < size_ - 1; i++) {
			ptr[i] = ptr[i + 1];
		}
		ptr[size_] = NULL;
		size_--;
	};
	//删除指定范围下标,from开始下标，num删除的数量
	void del(int from, int num) {
		for (int i = from; i < size_ - num; i++) {
			ptr[i] = ptr[i + num];
		}
		for (int i = size_ - num; i < size_; i++) {
			ptr[i] = NULL;
		}
		ptr[size_] -= num;
	};
	//清空所有数据
	void clear() {
		for (int i = 0; i < size_; i++) {
			ptr[i] = NULL;
		}
		size_ = 0;
	};
	//获取长度
	const int size() {
		return size_;
	};

	//下标获取
	T& operator[](int n) {
		if (n >= capacity)
			increment(n);
		return ptr[n];
	};
	//拷贝函数（浅拷贝）
	Array(Array& src) {
		size_ = src.size_;
		capacity = src.capacity;
		ptr = src.ptr;
	};
	//赋值（浅拷贝）
	Array& operator=(Array& src) {
		if (ptr) delete[] ptr;
		size_ = src.size_;
		capacity = src.capacity;
		ptr = src.ptr;
		return *this;
	};
	//赋值（深拷贝）
	Array& clone(Array& src) {
		delete[] ptr;
		size_ = src.size_;
		capacity = src.capacity;
		ptr = new T[capacity];
		memset(ptr, 0, sizeof(T) * capacity);
		for (int i = 0; i < size_; i++) {
			ptr[i] = src.ptr[i];
		}
		return *this;
	};
	//添加到末尾（同push_back）
	Array& operator+=(T& src) {
		return push_back(src);
	};
	Array& operator+=(Array& src) {
		return push_back(src);
	};
	//添加到源末尾
	Array& operator<<(Array& src) {
		return push_back(src);
	};
	//添加到目标末尾
	Array& operator>>(Array& dst) {
		dst.push_back(*this);
		return *this;
	};
	//扩容到n，若n小于容量则不操作，返回容量
	int increment(int n) {
		if (n <= capacity) {
			return capacity;
		}
		T* temp = new T[n];
		memcpy(temp,ptr,sizeof(T)*capacity);
		memset(&temp[size_], 0, sizeof(T) * (n - capacity));
		delete ptr;
		ptr = temp;
		capacity = n;
		return capacity;
	};
	//整理空间，删去多余容量，n[0]表示期望留空容量，n为负或大于当前剩余容量无效，结束时返回容量
	int arrange(int n = 0) {
		if (n >= capacity - size_) return capacity;
		if (n < 0) return capacity;
		T* temp = new T[size_ + n];
		memcpy(temp, ptr, sizeof(T) * (size_+n));
		delete ptr;
		ptr = temp;
		capacity = size_ + n;
		return capacity;
	}
	//排序函数（升序冒泡）ju[false]为真时降序
	void sort(bool ju = false) {
		if (ju == false) {
			for (int i = 1; i < size_; i++) {
				for (int j = i; j > 0; j--) {
					if (ptr[j] < ptr[j - 1]) {
						exchange(ptr[j], ptr[j - 1]);
					}
				}
			}
		} else {
			for (int i = 1; i < size_; i++) {
				for (int j = i; j > 0; j--) {
					if (ptr[j] > ptr[j - 1]) {
						exchange(ptr[j], ptr[j - 1]);
					}
				}
			}
		}
	}
	//返回容量
	int max_size() {
		return capacity;
	}
	//函数是否为空
	int empty() {
		return size_ == 0;
	}
	//与src交换
	Array& swap(Array src) {
		exchange(ptr, src.ptr);
		return *this;
	}
	//逆序
	Array& reverse() {
		T* temp = new T[size_];
		memcpy(temp, ptr, sizeof(T) * size_);
		for (int i = 0; i < size_; i++) {
			ptr[i] = temp[size_ - i];
		}
		delete temp;
		return *this;
	}
	//返回内部数据指针（慎用）
	T* point() {
		return ptr;
	}
private:
	//有效元素数量
	int size_;
	//容积
	int capacity;
	//数据指针
	T* ptr;
};

