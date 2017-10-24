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
const int G_SIZE = 40;
const int RB_SIZE = 50; //川の建造物
const int BRB_SIZE = 60;
const int TOWNS = 9;
const int TLVS = 3;
const int TD_TYPES = 2;
const int BUILDINGS = 3;
const int R_BUILDS = 3;
const int SP_BUILDS = 6;
const int RESOURCES = 4;
const int TERRAINS = 6;
const int GOODS = 22;
const int ITEMS = 2;
const int TRADE = 2;
const int STATS = 2;
const int UD_END = 0; //上下の端
const int LR_END = 1; //左右の端
const int R_DIR = 6;
const int MNR_TYPE = 3; //鉱産資源の種類
const int ONLY = 2;
const int SP_AREAS = 8;

enum ETerrain{
	PLAIN, FOREST, HILL_S, HILL_G, HILL_I, RIVER
};

enum ETown{
	WILD, FARM, F_VIL, COMM, MINE_S, MINE_G, MINE_I, PAS_S, PAS_C, PAS_P
};

enum EResource{
	MONEY, FOOD, WOOD, STONE
};

enum ETrade{
	VALUE, POWER
};

enum ESta{
	POP, BDG
};

enum EInfo{
	NO, FOUND, TOWN, RBUILD, T_DATA, BUY, PST, S_AREA
};

enum EMode{
	KEEP, CLOSE, EST, REMV, DEV, DG, BUILD, DEMO, SBUILD, CUT, BLD_P, BLD_SA, SEARCH, USE, END_USE
};

enum ETDType{
	NEW, LVUP
};

enum ERDir {
	NS, WE, NW, NE, SE, SW
};

enum EMineral {
	ROCK, GOLD, IRON
};

enum EOnly {
	T_HALL, COURT
};

struct STownData{
	STownData();

	char name[TOWNS + 1][100];
	int income[TOWNS + 1][TD_TYPES][RESOURCES];
	int cost[TOWNS + 1][TD_TYPES][RESOURCES];
	//int trade[TOWNS][TD_TYPES][TRADE];
	int goods[TOWNS + 1][TD_TYPES];
	int trade[TOWNS + 1][TD_TYPES];
	bool bufItem[TOWNS + 1][ITEMS + 1];
	int iExpL[TOWNS + 1][ITEMS + 1];
	char iExp[TOWNS + 1][ITEMS + 1][5][100];
};

struct SBuildingData{
	SBuildingData();

	char name[TOWNS + 1][BUILDINGS][100];
	int income[TOWNS + 1][BUILDINGS][RESOURCES];
	int cost[TOWNS + 1][BUILDINGS][RESOURCES];
	//int trade[TOWNS][BUILDINGS][TRADE];
	int goods[TOWNS + 1][BUILDINGS];
	int trade[TOWNS + 1][BUILDINGS];
};

struct SRBuildingData {
	SRBuildingData();

	char name[R_BUILDS][100];
	int cost[R_BUILDS][RESOURCES];
	char exp[R_BUILDS][3][100];
};

struct SSpBuildingData{
	SSpBuildingData();

	char name[SP_BUILDS][100];
	int cost[SP_BUILDS][RESOURCES];
	bool onlyOne[SP_BUILDS];
	char req[SP_BUILDS][100];
	char exp[SP_BUILDS][100];
};

struct SGoodsData{
	SGoodsData();

	double value[GOODS];
};

struct SPastureData {
	SPastureData();

	char name[3][100];
	int cost[3][RESOURCES];
	int income[3][RESOURCES];
	int goods[3];
};

struct SSpAreaData {
	SSpAreaData();

	char name[SP_AREAS + 1][100];
	bool rPType[SP_AREAS + 1][TOWNS + 1];
	int reqPop[SP_AREAS + 1];
	int cost[SP_AREAS + 1][RESOURCES];
	int reqL[SP_AREAS + 1];
	char req[SP_AREAS + 1][5][100];
	int expL[SP_AREAS + 1];
	char exp[SP_AREAS + 1][5][100];
};

struct STile{
	STile();

	int x;
	int y;
	ETerrain terrain;
	//EMineral mineral;
	ETown town;
	int townLv;
	bool built[BUILDINGS + SP_BUILDS];
	int produce[RESOURCES];
	//int trade[TRADE];
	int goods[GOODS];
	bool fac[1];
	bool connect[BLOCKS_X * BLOCKS_Y];
	double buf[RESOURCES];
	int devLim;
	int saNum;
	int saLv;
	int adjRB[R_BUILDS];
	bool adjSer;
	int hidMin; //1:gold 2:iron
	bool itemUse[ITEMS + 1];
	int itemCon[ITEMS + 1];
	STownData tData;
	SBuildingData bData;
	SPastureData pData;

	void SetProduce();
};

struct STown /*地域全体*/ {
	STown();

	int income[RESOURCES];
	int resource[RESOURCES];
	int goodsPro[GOODS]; //Production
	int goodsCon[GOODS]; //Consumption
	int itemCon[ITEMS];
	double exSum; //export
	double inSum; //inport
	double exFin;
	double inFin;
	int trade;
	int devSum;
	int towns;
	int townMax;
	bool onlyOne[ONLY];
	int tTypePop[TOWNS + 1];
	int maxMines;
	int foodMax;
	int cPas;
	int foodCon;
	bool spFarm;
	bool onlyArea[SP_AREAS + 1];
	int forests;
	int tex;

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
	static CPicture g_pasture;
	static CPicture g_shadeS;
	static CPicture g_shadeL;
	static CPicture g_resource;
	//static CPicture g_trade;
	static CPicture g_goods;
	static CPicture g_build;
	static CPicture g_demolish;
	static CPicture g_mineral;
	static CPicture g_sArea;
	STownData tData;
	SBuildingData bData;

	void DrawIB();
	void I_UpDate();
};

struct SFoundBox :public SInfoBox{
	static CPicture g_boxF;
	static CPicture g_cbut;
	static CPicture g_pasture;
	static CPicture g_saBut;
	static CPicture g_naBut;
	static CPicture g_search;
	bool cut;
	bool search;
	bool pasture[3];
	bool spArea;
	SPastureData pData;
	SSpAreaData saData;
	bool spAAble[SP_AREAS + 1];

	SFoundBox(ETerrain type, STown town, bool cFlag, bool pas_s, bool pas_c, bool pas_p, bool sFlag);
	ETown town; //Managerで使う（引数とは別物）
	int pType;
	int saType;
	ETerrain terrain;
	bool CheckTEnough(ETown type);
	bool CheckSAEnough(int type);
	bool CheckSpAAble(int num);
	void DrawSAReq(int x, int y, int type, bool ok);
	void PutSAB(int x, int y, int type);
	void PutTB(int x, int y, ETown type);
	void PutPB(int x, int y, int type);
	void DrawTData(int x, int y, ETown type);
	void DrawPData(int x, int y, int type);
	void DrawFB(int money/*, EMineral mineral*/);
	void UpDate(ETerrain type, STown town, bool cFlag, bool pas_s, bool pas_c, bool pas_p, bool sFlag);
};

struct STownBox :public SInfoBox{
	bool clickDemo;
	static CPicture g_boxT;
	static CPicture g_lvUp;
	static CPicture g_lvDn;
	static CPicture g_demoL[2];
	static CPicture g_SB;
	static CPicture g_item;
	static CPicture g_search;
	bool search;

	STownBox(STown town, STile tile, bool sFlag);
	STile tileInfo;
	SSpBuildingData sbData;
	//int devLv;
	int buildNum;
	int sBuildNum;
	int iNum;
	bool CheckDEnough();
	bool CheckBEnough(int bNum);
	bool CheckSBEnough(int bNum);
	void PutRemoveButton(int x, int y);
	void DrawDev(int x, int y/*, int lv*/);
	void PutDevButton(int x, int y/*, int lv*/);
	void PutItemB(int x, int y, int type);
	void DrawDG(int x, int y/*, int lv*/);
	void PutDGButton(int x, int y/*, int lv*/);
	void DrawBuildings(int x, int y, int build);
	void DrawSBuildings(int x, int y, int build, bool popReq, bool ex);
	void PutBuildingButton(int x, int y, int bNum, bool isBuilt);
	void PutSpBuildButton(int x, int y, int bNum, int tbNum, bool isBuilt);
	void DrawItemInfo(int x, int y, int tNum, int iNum);
	void DrawTB();
	void UpDate(STown town, STile tile);
};

struct SPastureBox :public SInfoBox {
	static CPicture g_boxP;
	static CPicture g_pasture;
	static CPicture g_demoP;
	int money;
	int num;
	STile tileInfo;
	SPastureData pData;

	SPastureBox(STile tile, int m, int n);
	void DrawPB();
};

struct SRiverBox :public SInfoBox{
	static CPicture g_boxR;
	static CPicture g_RB;
	static CPicture g_buildL;
	static CPicture g_demoL;
	SRBuildingData rbData;

	STile tileInfo;
	int buildNum;
	SRiverBox(STown town, STile tile);
	bool CheckRBEnough(int bNum);
	void DrawRB(ETown u, ETown d, ETown l, ETown r);
	void PutRBButton(int x, int y, int bNum, bool isBuilt);
	void DrawDataBox(int x, int y, int bNum);
	void UpDate(STown town, STile tile);
};

struct STradeBox :public SInfoBox {
	static CPicture g_boxG;
	static CPicture g_arrow;
	SGoodsData gData;

	STradeBox(STown town);
	void DrawTB();
	void DrawArrow(int x, int y, int n);
};

struct SBuyBox :public SInfoBox {
	static CPicture g_boxB;
	static CPicture g_brbut;
	bool bought;
	EResource rType;
	int amount;
	int cost;
	
	SBuyBox(STown town);
	void DrawBB(int money);
	void PutBuyButton(int x, int y, int trade, EResource type, int c, int a, int money);
};

struct SSpABox :public SInfoBox {
	static CPicture g_boxs;
	static CPicture g_lvUp;
	STile tileInfo;
	SSpAreaData saData;

	SSpABox(STown town, STile tile);
	bool CheckDAble(int type);
	void PutDevB(int x, int y);
	void DrawSB();
	void UpDate(STown town, STile tile);
};

class CTileManager{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_pasture;
	static CPicture g_frame;
	static CPicture g_resource;
	//static CPicture g_trade;
	static CPicture g_goods;
	static CPicture g_stats;
	static CPicture g_num;
	static CPicture g_river;
	static CPicture g_hill;
	static CPicture g_mineral;
	static CPicture g_tBut; //trade
	static CPicture g_rBut; //region
	static CPicture g_bBut; //buy
	static CPicture g_sArea;
	static CPicture g_starve;
	static CPicture g_item;

	STownData tData;
	SBuildingData bData;
	SRBuildingData rbData;
	SSpBuildingData sbData;
	SGoodsData gData;
	SPastureData pData;
	SSpAreaData saData;
	STile tile[BLOCKS_X * BLOCKS_Y];//0 1
									//2 3
	STown town;
	SFoundBox *fbox;
	STownBox *tbox;
	SPastureBox *pbox;
	SRiverBox *rbox;
	SSpABox *sbox;
	STradeBox *gbox;
	SBuyBox *bbox;
	CData *data;

	EInfo boxStatus;
	bool openInfo;
	int infoNum;
	//bool forest[BLOCKS_X * BLOCKS_Y];
	bool tflag[BLOCKS_X * BLOCKS_Y];
	bool showTrade;
	bool loaded;

	int turn;

	//void CheckConnect(int n);
	void DrawRiver(int n);
	void CheckAdjB();
	void WriteData();
	void ReadData();
	void UDData();

public:
	CTileManager();
	void Set();
	void Draw();
	void OpenInfo();
	void CloseInfo();
};