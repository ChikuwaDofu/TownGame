#pragma once

#include"Suken.h"
#include"Picture.h"

struct SEndData{
	SEndData();

	int townPop[9];
	bool spA[8];
	int spLv[8];
	int resPro[4];
	int goods[22];
	int tIncome;
};

class CResult{
private:
	static CPicture g_resB;
	static CPicture g_town;
	static CPicture g_resource;
	static CPicture g_goods;
	static CPicture g_sArea;

public:
	SEndData data;
	bool back;

	void Set();
	void Draw();
};
