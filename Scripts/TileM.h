#pragma once

#include"Suken.h"
#include"Picture.h"

const int BLOCKS_X = 10;
const int BLOCKS_Y = 10;
const int CROSS_L = 40;
const int GRID = 60;
const int B_SIZE = 40;
const int I_SIZE = 20;
const int TOWNS = 2;
const int TLVS = 3;
const int TD_TYPES = 2;
const int BUILDINGS = 3;
const int RESOURCES = 3;
const int TERRAINS = 2;
const int TRADE = 2;

enum ETerrain{
	PLAIN, FOREST
};

enum ETown{
	WILD = -1, FARM = 0, F_VIL = 1
};

enum EResource{
	MONEY, FOOD, WOOD
};

enum ETrade{
	VALUE, POWER
};

enum EInfo{
	NO, FOUND, TOWN
};

enum EMode{
	KEEP, CLOSE, EST, REMV, DEV, BUILD, DEMO
};

enum ETDType{
	NEW, LVUP
};

struct STownData{
	STownData();

	int income[TOWNS][TD_TYPES][RESOURCES];
	int cost[TOWNS][TD_TYPES][RESOURCES];
	int trade[TOWNS][TD_TYPES][TRADE];
};

struct SBuildingData{
	SBuildingData();

	int income[TOWNS][BUILDINGS][RESOURCES];
	int cost[TOWNS][BUILDINGS][RESOURCES];
	int trade[TOWNS][BUILDINGS][TRADE];
};

struct STile{
	STile();

	int x;
	int y;
	ETerrain terrain;
	ETown town;
	int townLv;
	bool built[BUILDINGS];

	int produce[RESOURCES];
	int trade[TRADE];
};

struct STown{
	STown();

	int income[RESOURCES];
	int resource[RESOURCES];
	int trade[TRADE];
	int devSum;

	void Set();
};

struct SInfoBox{
	SInfoBox();
	bool open;
	EMode mode;
	STown townInfo;
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_building;
	static CPicture g_box;
	static CPicture g_shade;
	static CPicture g_resource;
	static CPicture g_trade;
	static CPicture g_build;
	static CPicture g_demolish;
	STownData tData;
	SBuildingData bData;

	void DrawIB();
};

struct SFoundBox :public SInfoBox{
	SFoundBox(ETerrain type, STown town);
	ETown town; //Managerで使う（引数とは別物）
	ETerrain terrain;
	bool CheckEnough(ETown type);
	void PutButton(int x, int y, ETown type, int money);
	void DrawData(int x, int y, ETown type);
	void DrawFB(int money);
};

struct STownBox :public SInfoBox{
	static CPicture g_lvUp;

	STownBox(STown town, STile tile);
	STile tileInfo;
	int devLv;
	int buildNum;
	bool CheckDEnough(int lv);
	bool CheckBEnough(int bNum);
	void PutRemoveButton(int x, int y);
	void DrawDev(int x, int y, int lv);
	void PutDevButton(int x, int y, int lv);
	void DrawBuildings(int x, int y, int build);
	void PutBuildingButton(int x, int y, int bNum, bool isBuilt);
	void DrawTB();
};

class CTileManager{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_frame;
	static CPicture g_resource;
	static CPicture g_trade;
	static CPicture g_num;

	STownData tData;
	SBuildingData bData;
	STile tile[100];//0 1
					//2 3
	STown town;
	SFoundBox *fbox;
	STownBox *tbox;

	EInfo boxStatus;
	bool openInfo;
	int infoNum;

public:
	CTileManager(){};
	void Set();
	void Draw();
	void OpenInfo();
	void CloseInfo();
};