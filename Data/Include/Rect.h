#pragma once
class Rect {
public:
	int x, y, w, h;
	Rect();
	Rect(const Rect&);
	Rect(int x, int y, int w, int h);
	Rect& operator = (const Rect&);
	bool operator != (const Rect&);
	int wh();
	~Rect();
};

