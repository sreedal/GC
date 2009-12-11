
#include "stdafx.h"

class MyRect
{
public:
	int x, y;
	int w, h;
	bool similar(const MyRect &r) const; //"r" is similar to "this"
};



class History
{
public:
	enum {SIZE=5};
	void UpdateTime();
	bool CheckContinuity(MyRect &rect);
private:
	vector<MyRect> v[SIZE];
	vector<MyRect> v0, v1, v2;
	bool RectInVect(MyRect &r, const vector<MyRect> &v);
};

