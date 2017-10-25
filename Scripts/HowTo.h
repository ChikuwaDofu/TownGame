#pragma once

#include"Suken.h"
#include"Picture.h"

const int HT = 9;

class CHowTo{
private:
	static CPicture g_HtP[HT];
	static CPicture g_AR;
	static CPicture g_AL;

	int htNum;

public:
	bool end;

	void Set();
	void Reset();
	void Draw();
};
