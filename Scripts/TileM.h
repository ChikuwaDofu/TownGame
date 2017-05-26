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
const int BUILDINGS = 3;
const int RESOURCES = 3;
const int TERRAINS = 2;

enum ETerrain{
	PLAIN, FOREST
};

enum ETown{
	WILD = -1, FARM = 0, F_VIL = 1
};

enum EResource{
	MONEY, FOOD, WOOD
};

enum EInfo{
	NO, FOUND, TOWN
};

enum EMode{
	KEEP, CLOSE, EST, REMV, DEV, BUILD, DEMO
};

struct STownData{
	STownData();

	int income[TOWNS][TLVS][RESOURCES];
	int cost[TOWNS][TLVS][RESOURCES];
};

struct SBuildingData{
	SBuildingData();

	int income[TOWNS][TLVS][RESOURCES];
	int cost[TOWNS][TLVS][RESOURCES];
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
};

struct STown{
	int resource[RESOURCES];
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
	static CPicture g_build;
	static CPicture g_demolish;
	STownData tData;
	SBuildingData bData;

	void DrawIB();
};

struct SFoundBox :public SInfoBox{
	SFoundBox(ETerrain type, STown town);
	ETown town; //ManagerÇ≈égÇ§Åià¯êîÇ∆ÇÕï ï®Åj
	ETerrain terrain;
	bool CheckEnough(ETown type);
	void PutButton(int x, int y, ETown type, int money);
	void DrawData(int x, int y, ETown type);
	void DrawFB(int money);
};

struct STownBox :public SInfoBox{
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
	void PutBuildButton(int x, int y, int bNum);
	void PutDemolishButton(int x, int y, int bNum);
	void DrawTB();
};

class CTileManager{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_frame;
	static CPicture g_resource;
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
	CTileManager(){}
	void Set();
	void Draw();
	void OpenInfo();
	void CloseInfo();
};