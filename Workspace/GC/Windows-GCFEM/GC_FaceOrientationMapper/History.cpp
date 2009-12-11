#include "stdafx.h"

 
//Returns true is "r" is similar to "this".
//"r" is similar to "this" if "r" is located within 30 pixels of "this".
bool MyRect::similar(const MyRect &r) const
{
	int dx = x - r.x;
	int dy = y - r.y;
	int dist = dx*dx+dy*dy;
	dist = (int)sqrt((double)dist);
	if(dist < 30)
		return true;
	else
		return false;
}

void History::UpdateTime()
{
	//v2 = v1; //v2.swap(v1);
	//v1 = v0; //v1.swap(v0);
	//v0.clear();
	for(int i=SIZE-1; i>0; --i)
	{
		v[i] = v[i-1];
	}
	v[0].clear();
}

bool History::CheckContinuity(MyRect &rect)
{
	v[0].push_back(rect);
	for(int i=1; i<SIZE; ++i)
	{
		if(!RectInVect(rect, v[i]))
			return false;
	}

	return true;

	/*
	v0.push_back(rect);
	if(!RectInVect(rect, v1))
		return false;	
	else
	{
		if(!RectInVect(rect, v2))
			return true;
		else
			return true;
	}
	*/
	
}

//Returns true is the vector "v" contains a rect that is similar to "r".
//If s0, "r" is replaced by that rect.
bool History::RectInVect(MyRect &r, const vector<MyRect> &v)
{
	vector<MyRect>::const_iterator iter;
	for(iter=v.begin(); iter!=v.end(); ++iter)
	{
		if(r.similar(*iter)) {r = *iter; return true;}
	}

	return false;
}
