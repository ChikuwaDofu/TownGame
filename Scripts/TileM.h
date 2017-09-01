#pragma once

#include"Suken.h"
#include"Picture.h"

const int BLOCKS_X = 10;
const int BLOCKS_Y = 10;
const int CROSS_L = 40;
const int GRID = 60;
const int B_SIZE = 40;
const int I_SIZE = 20;
const int R_SIZE = 40; //資源アイコン
const int RB_SIZE = 50; //川の建造物
const int TOWNS = 3;
const int TLVS = 3;
const int TD_TYPES = 2;
const int BUILDINGS = 3;
const int R_BUILDS = 3;
const int RESOURCES = 3;
const int TERRAINS = 4;
const int TRADE = 2;
const int STATS = 1;
const int UD_END = 0; //上下の端
const int LR_END = 1; //左右の端
const int R_DIR = 6;
const int MNR_TYPE = 3; //鉱産資源の種類

enum ETerrain{
	PLAIN, FOREST, HILL, RIVER
};

enum ETown{
	WILD = -1, FARM = 0, F_VIL = 1, COMM = 2
};

enum EResource{
	MONEY, FOOD, WOOD
};

enum ETrade{
	VALUE, POWER
};

enum ESta{
	POP, BDG
};

enum EInfo{
	NO, FOUND, TOWN, RBUILD
};

enum EMode{
	KEEP, CLOSE, EST, REMV, DEV, DG, BUILD, DEMO
};

enum ETDType{
	NEW, LVUP
};

enum ERDir {
	NS, WE, NW, NE, SE, SW
};

enum EMineral {
	STONE, GOLD, IRON
};

struct STownData{
	STownData();

	int income[TOWNS][TD_TYPES][RESOURCES];
	int cost[TOWNS][TD_TYPES][RESOURCES];
	int trade[TOWNS][TD_TYPES][TRADE];
};

struct SBuildingData{
	SBuildingData();

	char name[TOWNS][BUILDINGS][100];
	int income[TOWNS][BUILDINGS][RESOURCES];
	int cost[TOWNS][BUILDINGS][RESOURCES];
	int trade[TOWNS][BUILDINGS][TRADE];
};

struct SRBuildingData {
	SRBuildingData();

	char name[R_BUILDS][100];
	int cost[R_BUILDS][RESOURCES];
};

struct STile{
	STile();

	int x;
	int y;
	ETerrain terrain;
	EMineral mineral;
	ETown town;
	int townLv;
	bool built[BUILDINGS];
	int produce[RESOURCES];
	int trade[TRADE];
	bool fac[1];
	bool connect[BLOCKS_X * BLOCKS_Y];
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
	static CPicture g_shade;
	static CPicture g_resource;
	static CPicture g_trade;
	static CPicture g_build;
	static CPicture g_demolish;
	static CPicture g_mineral;
	STownData tData;
	SBuildingData bData;

	void DrawIB();
};

struct SFoundBox :public SInfoBox{
	static CPicture g_boxF;

	SFoundBox(ETerrain type, STown town);
	ETown town; //Managerで使う（引数とは別物）
	ETerrain terrain;
	bool CheckEnough(ETown type);
	void PutButton(int x, int y, ETown type, int money);
	void DrawData(int x, int y, ETown type);
	void DrawFB(int money, EMineral mineral);
};

struct STownBox :public SInfoBox{
	bool clickDemo;
	static CPicture g_boxT;
	static CPicture g_lvUp;
	static CPicture g_lvDn;
	static CPicture g_demoL[2];

	STownBox(STown town, STile tile);
	STile tileInfo;
	//int devLv;
	int buildNum;
	bool CheckDEnough();
	bool CheckBEnough(int bNum);
	void PutRemoveButton(int x, int y);
	void DrawDev(int x, int y/*, int lv*/);
	void PutDevButton(int x, int y/*, int lv*/);
	void DrawDG(int x, int y/*, int lv*/);
	void PutDGButton(int x, int y/*, int lv*/);
	void DrawBuildings(int x, int y, int build);
	void PutBuildingButton(int x, int y, int bNum, bool isBuilt);
	void DrawTB();
};

struct SRiverBox :public SInfoBox{
	static CPicture g_boxR;
	static CPicture g_RB;
	SRBuildingData rbData;

	STile tileInfo;
	int buildNum;
	SRiverBox(STile tile);
	void DrawRB(ETown u, ETown d, ETown l, ETown r);
	void PutRBButton(int x, int y, int bnum, bool isBuilt);
};

class CTileManager{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_frame;
	static CPicture g_resource;
	static CPicture g_trade;
	static CPicture g_stats;
	static CPicture g_num;
	static CPicture g_river;
	static CPicture g_hill;
	static CPicture g_mineral;

	STownData tData;
	SBuildingData bData;
	SRBuildingData rbData;
	STile tile[BLOCKS_X * BLOCKS_Y];//0 1
									//2 3
	STown town;
	SFoundBox *fbox;
	STownBox *tbox;
	SRiverBox *rbox;

	EInfo boxStatus;
	bool openInfo;
	int infoNum;
	//bool forest[BLOCKS_X * BLOCKS_Y];
	bool tflag[BLOCKS_X * BLOCKS_Y];

	//void CheckConnect(int n);
	void DrawRiver(int n);

public:
	CTileManager(){};
	void Set();
	void Draw();
	void OpenInfo();
	void CloseInfo();
};