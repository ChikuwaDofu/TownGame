#pragma once

#include"Suken.h"
#include"Picture.h"

const int BLOCKS_X = 10;
const int BLOCKS_Y = 10;
const int GRID = 60;
const int TOWNS = 2;
const int TLVS = 6;

enum ETerrain{
	PLAIN, SEA
};

enum ETown{
	WILD = -1, FARM = 0, PORT = 1
};

enum EInfo{
	NO, FOUND, TOWN
};

struct STownData{
	int income_m[TOWNS][TLVS];
	int cost_m[TOWNS][TLVS];
};

struct STile{
	int x;
	int y;
	ETerrain terrain;
	ETown town;
	int townLv;
};

struct STown{
	int money;
};

struct SInfoBox{
	SInfoBox();
	bool close;
	bool select;
	bool open;
	ETerrain terrain;
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_box;
	FILE* f_tData;
	STownData tData;

	void DrawIB();
};

struct SFoundBox :public SInfoBox{
	SFoundBox(ETerrain type);
	ETown town;
	void PutButton(int x, int y, ETown type);
	void DrawFB(int money);
};

struct STownBox :public SInfoBox{
	STownBox(ETerrain type);
	bool remove;
	bool develop;
	int devLv;
	void PutRemoveButton(int x, int y);
	void PutDevButton(int x, int y, int lv);
	void DrawTB(ETown town, int lv, int money);
};

class CTileManager{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_frame;

	FILE* f_tData;

	STownData tData;
	STile tile[100];//0 1
					//2 3
	STown town;
	SFoundBox *fbox;
	STownBox *tbox;

	EInfo boxStatus;
	bool openInfo;
	int infoNum;

public:
	CTileManager(){}
	void Set();
	void Draw();
	void OpenInfo();
	void CloseInfo();
};