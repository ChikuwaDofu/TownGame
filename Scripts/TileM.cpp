#include"TileM.h"
#include<queue>
#include<vector>
#include<list>
#include<fstream>

//1.search all adjacent road tiles with <queue>
//2.change all adjacent road tiles' connect[a.a.r.t.] to true with <vector>

using namespace std;
typedef std::pair<int, int> P;

//queue<int> q;
int now, nxt;
int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
//vector<int> v;
vector<int> vec, res;
vector<P> stv;

CPicture CTileManager::g_tile;
CPicture CTileManager::g_town;
CPicture CTileManager::g_pasture;
CPicture CTileManager::g_frame;
CPicture CTileManager::g_resource;
//CPicture CTileManager::g_trade;
CPicture CTileManager::g_goods;
CPicture CTileManager::g_stats;
CPicture CTileManager::g_num;
CPicture CTileManager::g_river;
CPicture CTileManager::g_hill;
CPicture CTileManager::g_mineral;
CPicture CTileManager::g_rBut;
CPicture CTileManager::g_tBut;
CPicture CTileManager::g_bBut;
CPicture CTileManager::g_sArea;
CPicture CTileManager::g_starve;

STownData::STownData(){
	FILE* f;

	f = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 1; i <= TOWNS; i++){
		fscanf(f, "%s", &name[i]);
		for (int j = 0; j < TD_TYPES; j++){
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &income[i][j][k]);
			}
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &cost[i][j][k]);
			}
			/*for (int k = 0; k < TRADE; k++){
				fscanf(f, "%d", &trade[i][j][k]);
			}*/
			fscanf(f, "%d", &goods[i][j]);
			trade[i][j] = 0;
			if (goods[i][j] == 1){
				fscanf(f, "%d", &trade[i][j]);
			}
		}
	}
	fclose(f);
}

SBuildingData::SBuildingData(){
	FILE* f;

	f = fopen("Chikuwa3/BuildingData.txt", "r");
	for (int i = 1; i <= TOWNS; i++){
		for (int j = 0; j < BUILDINGS; j++){
			fscanf(f, "%s", &name[i][j]);
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &income[i][j][k]);
			}
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &cost[i][j][k]);
			}
			/*for (int k = 0; k < TRADE; k++){
				fscanf(f, "%d", &trade[i][j][k]);
			}*/
			fscanf(f, "%d", &goods[i][j]);
			trade[i][j] = 0;
			if (goods[i][j] == 1){
				fscanf(f, "%d", &trade[i][j]);
			}
		}
	}
	fclose(f);
}

SRBuildingData::SRBuildingData() {
	FILE* f;

	f = fopen("Chikuwa3/RBData.txt", "r");
	for (int i = 0; i < R_BUILDS; i++) {
		fscanf(f, "%s", &name[i]);
		for (int j = 0; j < RESOURCES; j++) {
			fscanf(f, "%d", &cost[i][j]);
		}
		for (int j = 0; j < 2; j++) {
			fscanf(f, "%s", &exp[i][j]);
		}
	}
	fclose(f);
}

SSpBuildingData::SSpBuildingData(){
	FILE* f;

	f = fopen("Chikuwa3/SpBData.txt", "r");
	for (int i = 0; i < SP_BUILDS; i++){
		fscanf(f, "%s", &name[i]);
		for (int j = 0; j < RESOURCES; j++){
			fscanf(f, "%d", &cost[i][j]);
		}
		int buf = 0;
		fscanf(f, "%d", &buf);
		if (buf == 0){
			onlyOne[i] = false;
		}else{
			onlyOne[i] = true;
		}
		fscanf(f, "%s", &req[i]);
		fscanf(f, "%s", &exp[i]);
	}
	fclose(f);
}

SGoodsData::SGoodsData(){
	FILE* f;

	f = fopen("Chikuwa3/GoodsValue.txt", "r");
	/////
	for (int i = 0; i < GOODS; i++){
		fscanf(f, "%lf", &value[i]);
	}
	/////
	fclose(f);
}

SPastureData::SPastureData() {
	FILE* f;

	f = fopen("Chikuwa3/PData.txt", "r");
	for (int i = 0; i < 3; i++) {
		fscanf(f, "%s", &name[i]);
		for (int j = 0; j < RESOURCES; j++) {
			fscanf(f, "%d", &cost[i][j]);
		}
		for (int j = 0; j < RESOURCES; j++) {
			fscanf(f, "%d", &income[i][j]);
		}
		fscanf(f, "%d", &goods[i]);
	}
	fclose(f);
}

SSpAreaData::SSpAreaData() {
	FILE *f;

	f = fopen("Chikuwa3/SpAData.txt", "r");
	for (int i = 1; i <= SP_AREAS; i++) {
		fscanf(f, "%s", &name[i]);
		for (int j = 0; j <= TOWNS; j++) {
			rPType[i][j] = false;
		}
		int num = 0, town = 0;
		fscanf(f, "%d", &num);
		for (int j = 0; j < num; j++) {
			fscanf(f, "%d", &town);
			rPType[i][town] = true;
		}
		fscanf(f, "%d", &reqPop[i]);
		for (int j = 0; j < RESOURCES; j++) {
			fscanf(f, "%d", &cost[i][j]);
		}
		fscanf(f, "%d", &reqL[i]);
		for (int j = 0; j < reqL[i]; j++) {
			fscanf(f, "%s", &req[i][j]);
		}
	}
	fclose(f);
}

STile::STile(){
	x = x*GRID + WINDOW_WIDTH - WINDOW_HEIGHT;
	y = y*GRID;
	terrain = PLAIN;
	//mineral = ROCK;
	town = WILD;
	townLv = 0;
	devLim = 0;
	saNum = 0;
	for (int i = 0; i < BUILDINGS + SP_BUILDS; i++){
		built[i] = false;
	}
	for (int i = 0; i < RESOURCES + TRADE; i++) {
		buf[i] = 0;
	}
	//fac[0] = false;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		connect[i] = false;
	}
	for (int i = 0; i < RESOURCES; i++){
		produce[i] = 0;
	}
	/*for (int i = 0; i < TRADE; i++){
		trade[i] = 0;
	}*/
	for (int i = 0; i < GOODS; i++){
		goods[i] = 0;
	}
}

void STile::SetProduce() {
	if (town == WILD) {
		devLim = 0;
		for (int i = 0; i < RESOURCES; i++) {
			produce[i] = 0;
		}
		/*for (int i = 0; i < TRADE; i++) {
			trade[i] = 0;
		}*/
		for (int i = 0; i < GOODS; i++) {
			goods[i] = 0;
		}

		if (terrain != RIVER) {
			if (built[0]) {
				for (int i = 0; i < RESOURCES; i++) {
					produce[i] += pData.income[0][i];
				}
				goods[pData.goods[0]]++;
			}
			if (built[1]) {
				for (int i = 0; i < RESOURCES; i++) {
					produce[i] += pData.income[1][i];
				}
				goods[pData.goods[1]]++;
			}
			if (built[2]) {
				for (int i = 0; i < RESOURCES; i++) {
					produce[i] += pData.income[2][i];
				}
				goods[pData.goods[2]]++;
			}
		}
	}
	else {
		devLim = 5;
		for (int i = 0; i < RESOURCES; i++) {
			produce[i] = tData.income[town][0][i] + tData.income[town][1][i] * townLv;

			for (int j = 0; j < BUILDINGS; j++) {
				if (built[j]) {
					produce[i] += bData.income[town][j][i];
				}
			}

			produce[i] *= 1 + buf[i];
		}
		/*for (int i = 0; i < TRADE; i++) {
			trade[i] = tData.trade[town][0][i] + tData.trade[town][1][i] * townLv;

			for (int j = 0; j < BUILDINGS; j++) {
				if (built[j]) {
					trade[i] += bData.trade[town][j][i];
				}
			}

			trade[i] *= 1 + buf[RESOURCES + i];
		}*/

		for (int i = 0; i < GOODS; i++){
			goods[i] = 0;
		}

		if (tData.goods[town][0] == 1) {
			goods[1] += tData.trade[town][0];
		}
		else {
			goods[tData.goods[town][0]]++;
		}
		if (tData.goods[town][1] == 1) {
			goods[1] += tData.trade[town][1] * townLv;
		}
		else {
			goods[tData.goods[town][1]] += townLv;
		}

		for (int j = 0; j < BUILDINGS; j++) {
			if (built[j]) {
				if (bData.goods[town][j] == 1) {
					goods[bData.goods[town][j]] += bData.trade[town][j];
				}
				else {
					goods[bData.goods[town][j]]++;
				}
			}
		}

		switch (town){
		case FARM:
			if (built[3]) {
				devLim += 5;
			}
			break;

		default:
			break;
		}

		goods[1] *= 1 + buf[RESOURCES];
	}
}

STown::STown(){
	Set();

	for (int i = 0; i < RESOURCES; i++){
		resource[i] = 100;
	}
	resource[FOOD] = 10;
}

void STown::Set(){
	for (int i = 0; i < RESOURCES; i++){
		income[i] = 0;
	}
	/*for (int i = 0; i < TRADE; i++){
		trade[i] = 0;
	}*/
	for (int i = 0; i < GOODS; i++){
		goodsPro[i] = 0;
		goodsCon[i] = 0;
	}
	for (int i = 0; i < ONLY; i++){
		onlyOne[i] = false;
	}

	devSum = 0;
	towns = 0;
	townMax = 5;
	exSum = 0;
	inSum = 0;
	exFin = 0;
	inFin = 0;
	trade = 0;
}

CTileManager::CTileManager() {
	data = new CData("Chikuwa3/SaveData.txt");
}

void CTileManager::Set(){
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		tflag[i] = false;
	}

	int x = 0, y = 0, cnt = 5, r = 0, bf = 1, d[2] = {}, s[2] = {} , left = BLOCKS_X * BLOCKS_Y; //direction, start
	switch (GetRand(1)) {
	case 0:

		switch (GetRand(1)) {
		case 0:
			x = GetRand(BLOCKS_X - 3) + 1;

			tile[x].terrain = RIVER;
			left--;

			while (y != BLOCKS_Y - 1) {
				if (cnt > 0 && bf == 0) {
					r = GetRand(3);
					switch (r) {
					case 2:

						if (tile[x - 1 + y*BLOCKS_X].terrain == PLAIN) {
							if (x > 1) {
								x--;
							}
							else {
								x++;
							}
							cnt--;
							bf = 2;
						}
						else {
							y++;
							if (bf > 0) {
								bf--;
							}
						}

						break;

					case 3:
						if (tile[x + 1 + y*BLOCKS_X].terrain == PLAIN) {
							if (x < BLOCKS_X - 2) {
								x++;
							}
							else {
								x--;
							}
							cnt--;
							bf = 2;
						}
						else {
							y++;
							if (bf > 0) {
								bf--;
							}
						}

						break;

					default:

						y++;
						if (bf > 0) {
							bf--;
						}

						break;
					}
				}
				else {
					y++;
					if (bf > 0) {
						bf--;
					}
				}

				tile[x + y*BLOCKS_X].terrain = RIVER;
				left--;
			}

			break;

		case 1:
			y = GetRand(BLOCKS_Y - 3) + 1;

			tile[y * BLOCKS_X].terrain = RIVER;
			left--;

			while (x != BLOCKS_X - 1) {
				if (cnt > 0 && bf == 0) {
					r = GetRand(3);
					switch (r) {
					case 2:

						if (tile[x + (y - 1)*BLOCKS_X].terrain == PLAIN) {
							if (y > 1) {
								y--;
							}
							else {
								y++;
							}
							cnt--;
							bf = 2;
						}
						else {
							x++;
							if (bf > 0) {
								bf--;
							}
						}

						break;

					case 3:

						if (tile[x + 1 + (y + 1)*BLOCKS_X].terrain == PLAIN) {
							if (y < BLOCKS_Y - 2) {
								y++;
							}
							else {
								y--;
							}
							cnt--;
							bf = 2;
						}
						else {
							x++;
							if (bf > 0) {
								bf--;
							}
						}

						break;

					default:

						x++;
						if (bf > 0) {
							bf--;
						}

						break;
					}
				}
				else {
					x++;
					if (bf > 0) {
						bf--;
					}
				}

				tile[x + y*BLOCKS_X].terrain = RIVER;
				left--;
			}

			break;
		}

		break;

		case 1:

			d[UD_END] = GetRand(1);
			d[LR_END] = GetRand(1);
			s[UD_END] = GetRand(BLOCKS_X - 5) + 2;
			s[LR_END] = GetRand(BLOCKS_Y - 5) + 2;
			if (d[LR_END] == 2) {
				x = 1;
				if (d[UD_END] == 0) {
					tile[s[UD_END]].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X + 1].terrain = RIVER;
					left -= 3;
					y = s[LR_END];

					while (x != s[UD_END] || y != 1) {
						if (x != s[UD_END]) {
							if (y != 1) {
								if (GetRand(1) == 0) {
									x++;
								}
								else {
									y--;
								}
							}
							else {
								x++;
							}
						}
						else {
							y--;
						}
						tile[x + y*BLOCKS_X].terrain = RIVER;
						left--;
					}
				}
				else {
					tile[s[UD_END] + BLOCKS_X * (BLOCKS_Y - 1)].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X + 1].terrain = RIVER;
					left -= 3;
					y = s[LR_END];

					while (x != s[UD_END] || y != BLOCKS_Y - 2) {
						if (x != s[UD_END]) {
							if (y != BLOCKS_Y - 2) {
								if (GetRand(1) == 0) {
									x++;
								}
								else {
									y++;
								}
							}
							else {
								x++;
							}
						}
						else {
							y++;
						}
						tile[x + y*BLOCKS_X].terrain = RIVER;
						left--;
					}
				}
			}
			else {
				x = BLOCKS_X - 2;
				if (d[UD_END] == 0) {
					tile[s[UD_END]].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 1].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 2].terrain = RIVER;
					left -= 3;
					y = s[LR_END];

					while (x != s[UD_END] || y != 1) {
						if (x != s[UD_END]) {
							if (y != 1) {
								if (GetRand(1) == 0) {
									x--;
								}
								else {
									y--;
								}
							}
							else {
								x--;
							}
						}
						else {
							y--;
						}
						tile[x + y*BLOCKS_X].terrain = RIVER;
						left--;
					}
				}
				else {
					tile[s[UD_END] + BLOCKS_X * (BLOCKS_Y - 1)].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 1].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 2].terrain = RIVER;
					left -= 3;
					y = s[LR_END];

					while (x != s[UD_END] || y != BLOCKS_Y - 2) {
						if (x != s[UD_END]) {
							if (y != BLOCKS_Y - 2) {
								if (GetRand(1) == 0) {
									x--;
								}
								else {
									y++;
								}
							}
							else {
								x--;
							}
						}
						else {
							y++;
						}
						tile[x + y*BLOCKS_X].terrain = RIVER;
						left--;
					}
				}
			}

			break;
	}

	int c = 0, n = 0;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		tile[i].connect[i] = true;
		//forest[i] = false;
	}
	while (c < left - 5){
		n = GetRand(BLOCKS_X * BLOCKS_Y - 1);

		if (tile[n].terrain == PLAIN /*!forest[n]*/){
			//forest[n] = true;
			tile[n].terrain = FOREST;
			c++;
		}
	}
	c = 0;
	n = GetRand(BLOCKS_X * BLOCKS_Y - 1);
	while (tile[n].terrain == RIVER) {
		n = GetRand(BLOCKS_X * BLOCKS_Y - 1);
	}
	tflag[n] = true;
	vec.push_back(n);
	while (c < 8 && !vec.empty()) {
		r = GetRand(vec.size() - 1);
		n = vec.operator[](r);
		res.push_back(n);
		vec.erase(vec.begin() + r);
		tile[n].terrain = HILL_S;
		c++;
		if (n%BLOCKS_X < BLOCKS_X - 1) {
			if ((/*tile[n + 1].terrain == PLAIN || */tile[n + 1].terrain == FOREST) && !tflag[n + 1]) {
				vec.push_back(n + 1);
				tflag[n + 1] = true;
			}
		}
		if (n%BLOCKS_X > 0) {
			if ((/*tile[n - 1].terrain == PLAIN || */tile[n - 1].terrain == FOREST) && !tflag[n - 1]) {
				vec.push_back(n - 1);
				tflag[n - 1] = true;
			}
		}
		if (n / BLOCKS_X > 0) {
			if ((/*tile[n - BLOCKS_X].terrain == PLAIN || */tile[n - BLOCKS_X].terrain == FOREST) && !tflag[n - BLOCKS_X]) {
				vec.push_back(n - BLOCKS_X);
				tflag[n - BLOCKS_X] = true;
			}
		}
		if (n / BLOCKS_X < BLOCKS_Y - 1) {
			if ((/*tile[n + BLOCKS_X].terrain == PLAIN || */tile[n + BLOCKS_X].terrain == FOREST) && !tflag[n + BLOCKS_X]) {
				vec.push_back(n + BLOCKS_X);
				tflag[n + BLOCKS_X] = true;
			}
		}
	}

	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].terrain = HILL_G;
	res.erase(res.begin() + n);
	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].terrain = HILL_I;
	res.erase(res.begin() + n);
	tile[res.operator[](GetRand(res.size() - 1))].terrain = HILL_I;

	openInfo = false;
	boxStatus = NO;
	infoNum = -1;
	showTrade = false;
	loaded = false;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS + 1, GRID, GRID, TOWNS + 1);
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);
	g_frame.Load("Chikuwa3/Frame.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, I_SIZE, I_SIZE, RESOURCES);
	//g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, I_SIZE, I_SIZE, TRADE);
	g_goods.Load("Chikuwa3/Goods.png", GOODS, 1, G_SIZE, G_SIZE, GOODS);
	g_stats.Load("Chikuwa3/SIcons.png", STATS, 1, I_SIZE, I_SIZE, STATS);
	g_num.Load("Chikuwa3/Numbers.png", 10, 1, 12, 20, 10);
	g_river.Load("Chikuwa3/River.png", R_DIR, 1, GRID, GRID, R_DIR);
	g_hill.Load("Chikuwa3/Hill.png", MNR_TYPE, 1, GRID, GRID, MNR_TYPE);
	g_mineral.Load("Chikuwa3/Mineral.png", MNR_TYPE, 1, R_SIZE, R_SIZE, MNR_TYPE);
	g_tBut.Load("Chikuwa3/TButton.png");
	g_rBut.Load("Chikuwa3/RButton.png");
	g_bBut.Load("Chikuwa3/BButton.png");
	g_sArea.Load("Chikuwa3/SpArea.png", 1, SP_AREAS + 1, GRID, GRID, SP_AREAS + 1);
	g_starve.Load("Chikuwa3/Starve.png");

	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
//		tile[i].terrain = FOREST;
	}
}

void CTileManager::OpenInfo(){
	infoNum = -1;
	bool info = false;

	if (Event.LMouse.GetClick(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
		info = true;
		infoNum = (Event.RMouse.GetX() + WINDOW_HEIGHT - WINDOW_WIDTH) / GRID + (Event.RMouse.GetY()) / GRID * BLOCKS_X;
	}

	/////

	if (Event.RMouse.GetClick(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
		tile[(Event.RMouse.GetX() + WINDOW_HEIGHT - WINDOW_WIDTH) / GRID + (Event.RMouse.GetY()) / GRID * BLOCKS_X].fac[0] = true;
	}

	/////

	if (info){
		openInfo = true;

		if (tile[infoNum].terrain == RIVER) {
			rbox = new SRiverBox(town, tile[infoNum]);
			boxStatus = RBUILD;
		}
		else {
			if (tile[infoNum].town == WILD) {
				if (tile[infoNum].saNum == 0) {
					int num = 0;
					for (int i = 0; i < BUILDINGS; i++) {
						if (tile[infoNum].built[i]) {
							num = i + 1;
						}
					}
					bool flag[4] = {};
					switch (num) {
					case 0:
						if (tile[infoNum].terrain == FOREST) {
							for (int i = 0; i < 4; i++) {
								if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
									if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town != WILD) {
										flag[0] = true;
									}
								}
							}
						}

						switch (tile[infoNum].terrain) {
						case PLAIN:
							for (int i = 0; i < 4; i++) {
								if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
									if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == PAS_S && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
										flag[1] = true;
									}
									if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == PAS_C && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
										flag[2] = true;
									}
								}
							}
							break;

						case FOREST:
							for (int i = 0; i < 4; i++) {
								if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
									if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == PAS_P && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
										flag[3] = true;
									}
								}
							}
							break;

						case HILL_S:
						case HILL_G:
						case HILL_I:
							for (int i = 0; i < 4; i++) {
								if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
									if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == PAS_S && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
										flag[1] = true;
									}
								}
							}
							break;

						default:
							break;
						}

						fbox = new SFoundBox(tile[infoNum].terrain, town, flag[0], flag[1], flag[2], flag[3]);
						boxStatus = FOUND;

						break;

					default:

						pbox = new SPastureBox(tile[infoNum], town.resource[MONEY], num - 1);
						boxStatus = PST;

						break;
					}
				}
				/*else {

				}*/
			}
			else {
				tbox = new STownBox(town, tile[infoNum]);
				boxStatus = TOWN;
			}
		}
	}
}

void CTileManager::CloseInfo(){
	openInfo = false;

	bool pFlag = false;
	switch (boxStatus){
	case FOUND:

		switch (fbox->mode){
		case CLOSE:
			delete fbox;
			boxStatus = NO;
			break;

		case EST:
			tile[infoNum].town = fbox->town;
			tile[infoNum].fac[0] = true;
			for (int i = 0; i < RESOURCES; i++){
				//tile[infoNum].produce[i] = tData.income[fbox->town][NEW][i];
				town.resource[i] -= tData.cost[fbox->town][NEW][i];
			}
			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = tData.trade[fbox->town][NEW][i];
			}*/
			delete fbox;
			boxStatus = NO;
			break;

		case CUT:
			tile[infoNum].terrain = PLAIN;
			town.resource[MONEY] -= 50;
			delete fbox;
			boxStatus = NO;
			break;

		case BLD_P:
			tile[infoNum].built[fbox->pType] = true;
			for (int i = 0; i < RESOURCES; i++) {
				town.resource[i] -= pData.cost[fbox->pType][i];
			}
			delete fbox;
			boxStatus = NO;
			break;

		case BLD_SA:
			tile[infoNum].saNum = fbox->saType;
			for (int i = 0; i < RESOURCES; i++) {
				town.resource[i] -= saData.cost[fbox->saType][i];
			}
			delete fbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	case TOWN:
		switch (tbox->mode){
		case CLOSE:
			delete tbox;
			boxStatus = NO;
			break;

		case REMV:
			town.resource[MONEY] -= (tData.cost[tile[infoNum].town][NEW][MONEY] + tData.cost[tile[infoNum].town][LVUP][MONEY] * tile[infoNum].townLv) / 2;
			town.resource[WOOD] += (tData.cost[tile[infoNum].town][NEW][WOOD] + tData.cost[tile[infoNum].town][LVUP][WOOD] * tile[infoNum].townLv) / 4;
			town.resource[STONE] += (tData.cost[tile[infoNum].town][NEW][STONE] + tData.cost[tile[infoNum].town][LVUP][STONE] * tile[infoNum].townLv) / 4;
			for (int i = 0; i < BUILDINGS + SP_BUILDS; i++){
				if (tile[infoNum].built[i]){
					tile[infoNum].built[i] = false;
				}
			}
			/*for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = 0;
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = 0;
			}*/
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
			for (int i = 0; i < 4; i++) {
				if (infoNum%BLOCKS_X + dx[i] >= 0 && infoNum%BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
					if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == WILD && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].terrain != RIVER) {
						for (int j = 0; j < 3; j++) {
							if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[j]) {
								for (int k = 0; k < 4; k++) {
									if (infoNum%BLOCKS_X + dx[i] + dx[k] >= 0 && infoNum%BLOCKS_X + dx[i] + dx[k] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] + dy[k] >= 0 && infoNum / BLOCKS_X + dy[i] + dy[k] < BLOCKS_Y) {
										if (tile[infoNum + dx[i] + dx[k] + (dy[i] + dy[k]) * BLOCKS_X].town == PAS_S + j && tile[infoNum + dx[i] + dx[k] + (dy[i] + dy[k]) * BLOCKS_X].built[0]) {
											pFlag = true;
										}
									}
								}

								if (!pFlag) {
									tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[j] = false;
								}
							}
						}
					}
				}
			}
			delete tbox;
			boxStatus = NO;
			break;

		case DEV:
			//tile[infoNum].townLv = tbox->devLv;
			tile[infoNum].townLv++;
			for (int i = 0; i < RESOURCES; i++){
				//tile[infoNum].produce[i] += tData.income[tile[infoNum].town][LVUP][i];
				town.resource[i] -= tData.cost[tile[infoNum].town][LVUP][i];
			}
			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += tData.trade[tile[infoNum].town][LVUP][i];
			}*/
			delete tbox;
			boxStatus = NO;
			break;
			
		case DG:
			//tile[infoNum].townLv = tbox->devLv;
			tile[infoNum].townLv--;
			for (int i = 0; i < RESOURCES; i++){
				//tile[infoNum].produce[i] += tData.income[tile[infoNum].town][LVUP][i];
				town.resource[i] -= tData.cost[tile[infoNum].town][LVUP][i];
			}
			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += tData.trade[tile[infoNum].town][LVUP][i];
			}*/
			delete tbox;
			boxStatus = NO;
			break;

		case BUILD:
			tile[infoNum].built[tbox->buildNum] = true;
			for (int i = 0; i < RESOURCES; i++){
				//tile[infoNum].produce[i] += bData.income[tile[infoNum].town][tbox->buildNum][i];
				town.resource[i] -= bData.cost[tile[infoNum].town][tbox->buildNum][i];
			}
			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += bData.trade[tile[infoNum].town][tbox->buildNum][i];
			}*/
			delete tbox;
			boxStatus = NO;
			break;

		//case DEMO:
		//	tile[infoNum].built[tbox->buildNum] = false;
		//	/*for (int i = 0; i < RESOURCES; i++){
		//		tile[infoNum].produce[i] -= bData.income[tile[infoNum].town][tbox->buildNum][i];
		//	}
		//	for (int i = 0; i < TRADE; i++){
		//		tile[infoNum].trade[i] -= bData.trade[tile[infoNum].town][tbox->buildNum][i];
		//	}*/
		//	delete tbox;
		//	boxStatus = NO;
		//	break;

		case SBUILD:
			tile[infoNum].built[tbox->buildNum] = true;
			for (int i = 0; i < RESOURCES; i++){
				town.resource[i] -= sbData.cost[tbox->sBuildNum][i];
			}
			/*switch (tbox->sBuildNum){
			case 0:
				tile[infoNum].devLim += 5;
				break;

			default:
				break;
			}*/
			delete tbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	case RBUILD:

		switch (rbox->mode) {
		case CLOSE:
			delete rbox;
			boxStatus = NO;
			break;

		case BUILD:
			tile[infoNum].built[rbox->buildNum] = true;
			for (int i = 0; i < RESOURCES; i++) {
				town.resource[i] -= rbData.cost[rbox->buildNum][i];
			}
			delete rbox;
			boxStatus = NO;
			break;

		case DEMO:
			tile[infoNum].built[rbox->buildNum] = false;
			delete rbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	case T_DATA:

		switch (gbox->mode) {
		case CLOSE:
			delete gbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	case BUY:

		switch (bbox->mode) {
		case CLOSE:
			delete bbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	case PST:

		switch (pbox->mode) {
		case CLOSE:
			delete pbox;
			boxStatus = NO;
			break;

		case DEMO:
			tile[infoNum].built[pbox->num] = false;
			town.resource[MONEY] -= 30;
			delete pbox;
			boxStatus = NO;
			break;

		default:
			break;
		}

		break;

	default:
		break;
	}
}

/*void CTileManager::CheckConnect(int n){
	for (int i = 0; i < 4; i++){
		if (n % BLOCKS_X + dx[i] >= 0 && n % BLOCKS_X + dx[i] < BLOCKS_X && n / BLOCKS_X + dy[i] >= 0 && n / BLOCKS_X + dy[i] < BLOCKS_Y){
			for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
				if (tile[n + dx[i] + dy[i] * 10].connect[j]){
					tile[n].connect[j] = true;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++){
		if (n % BLOCKS_X + dx[i] >= 0 && n % BLOCKS_X + dx[i] < BLOCKS_X && n / BLOCKS_X + dy[i] >= 0 && n / BLOCKS_X + dy[i] < BLOCKS_Y){
			for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
				if (!tile[n + dx[i] + dy[i] * 10].connect[j] && tile[n].connect[j]){
					tile[n + dx[i] + dy[i] * 10].connect[j] = true;
				}
			}
		}
	}
	bool search[BLOCKS_X * BLOCKS_Y] = {};
	search[n] = true;
	q.push(n);
	while (!q.empty()){
		now = q.front();
		v.push_back(now);
		q.pop();
		for (int i = 0; i < 4; i++){
			nxt = now + dx[i] + dy[i] * BLOCKS_X;
			if (tile[nxt].fac){
				q.push(nxt);
			}
		}
	}

	for (int i = 0; i < v.size(); i++){
		for (int j = 0; j < v.size(); j++){
			tile[i].connect[j] = true;
		}
	}
}*/

void CTileManager::DrawRiver(int n) {
	if (n % BLOCKS_X == 0 || (n % BLOCKS_X != 0 && tile[n - 1].terrain == RIVER)) {
		if (n / BLOCKS_X == 0 || (n / BLOCKS_X != 0 && tile[n - BLOCKS_X].terrain == RIVER)) {
			g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, NW);
		}
		else if (n % BLOCKS_X == BLOCKS_X - 1 || (n % BLOCKS_X != BLOCKS_X - 1 && tile[n + 1].terrain == RIVER)){
			g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, WE);
		}
		else {
			g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, SW);
		}
	}
	else if (n % BLOCKS_X == BLOCKS_X - 1 || (n % BLOCKS_X != BLOCKS_X - 1 && tile[n + 1].terrain == RIVER)) {
		if (n / BLOCKS_X == 0 || (n / BLOCKS_X != 0 && tile[n - BLOCKS_X].terrain == RIVER)) {
			g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, NE);
		}
		else {
			g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, SE);
		}
	}
	else {
		g_river.Draw(n%BLOCKS_X*GRID + WINDOW_WIDTH - WINDOW_HEIGHT, n / BLOCKS_X*GRID, NS);
	}
}

void CTileManager::CheckAdjRB() {
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		for (int j = 0; j < RESOURCES + 1; j++) {
			tile[i].buf[j] = 0;
		}
	}

	bool flag = false;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		if (tile[i].terrain == RIVER) {
			flag = false;
			for (int j = 0; j < 4; j++) {
				if ((i + dx[j]) % BLOCKS_X > 0 && (i + dx[j]) % BLOCKS_X < BLOCKS_X - 1 && (i + dy[j]) / BLOCKS_X > 0 && (i + dy[j]) / BLOCKS_X < BLOCKS_Y) {
					if (tile[i + dx[j] + dy[j] * BLOCKS_X].town != WILD) {
						flag = true;
					}
				}
			}

			if (!flag) {
				for (int j = 0; j < R_BUILDS; j++) {
					tile[i].built[j] = false;
				}
			}
		}
	}

	if (flag) {
		for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
			if (tile[i].terrain == RIVER) {
				if (i / BLOCKS_X > 0) {
					if (tile[i].built[0] && tile[i - BLOCKS_X].town == FARM) {
						tile[i - BLOCKS_X].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i - BLOCKS_X].town != WILD && tile[i - BLOCKS_X].town != COMM) {
						tile[i - BLOCKS_X].buf[RESOURCES] = 0.1;
					}
					if (tile[i].built[2] && tile[i - BLOCKS_X].town == FARM) {
						tile[i - BLOCKS_X].buf[RESOURCES] = 0.2;
					}
				}
				if (i / BLOCKS_X < BLOCKS_Y - 1) {
					if (tile[i].built[0] && tile[i + BLOCKS_X].town == FARM) {
						tile[i + BLOCKS_X].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i + BLOCKS_X].town != WILD && tile[i + BLOCKS_X].town != COMM) {
						tile[i + BLOCKS_X].buf[RESOURCES] = 0.1;
					}
					if (tile[i].built[2] && tile[i + BLOCKS_X].town == FARM) {
						tile[i + BLOCKS_X].buf[RESOURCES] = 0.2;
					}
				}
				if (i % BLOCKS_X > 0) {
					if (tile[i].built[0] && tile[i - 1].town == FARM) {
						tile[i - 1].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i - 1].town != WILD && tile[i - 1].town != COMM) {
						tile[i - 1].buf[RESOURCES] = 0.1;
					}
					if (tile[i].built[2] && tile[i - 1].town == FARM) {
						tile[i - 1].buf[RESOURCES] = 0.2;
					}
				}
				if (i % BLOCKS_X < BLOCKS_X - 1) {
					if (tile[i].built[0] && tile[i + 1].town == FARM) {
						tile[i + 1].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i + 1].town != WILD && tile[i + 1].town != COMM) {
						tile[i + 1].buf[RESOURCES] = 0.1;
					}
					if (tile[i].built[2] && tile[i + 1].town == FARM) {
						tile[i + 1].buf[RESOURCES] = 0.2;
					}
				}
			}
		}
	}
}

void CTileManager::Draw(){
	if (boxStatus == NO){
		OpenInfo();
	}
	else{
		CloseInfo();
		CheckAdjRB();
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
			tile[i].SetProduce();
		}
	}

	if (loaded){
		CloseInfo();
		CheckAdjRB();
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
			tile[i].SetProduce();
		}
	}

	town.Set();

	for (int i = 0; i <= TOWNS; i++) {
		town.tTypePop[i] = 0;
	}
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if (tile[i].town != WILD){
			town.tTypePop[tile[i].town] += tile[i].devLim;
			town.devSum += tile[i].townLv + 1;
			town.towns++;
		}

		if (tile[i].town == COMM && tile[i].built[3]){
			town.onlyOne[T_HALL] = true;
			town.townMax += 5;
		}
	}

	for (int i = 0; i < RESOURCES; i++){
		for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
			town.income[i] += tile[j].produce[i];
		}
	}

	//town.trade[POWER] = 1;
	/*for (int i = 0; i < TRADE; i++){
		for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
			town.trade[i] += tile[j].trade[i];
		}

		g_trade.Draw(75, 300 + I_SIZE * i, i);
	}*/
	for (int i = 1; i < GOODS; i++){
		for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
			town.goodsPro[i] += tile[j].goods[i];
		}

		//g_goods.Draw(75, 250 + I_SIZE * i, i);
	}

	town.goodsCon[2] = town.goodsPro[3] * 3;
	town.goodsCon[7] = town.goodsPro[8];
	town.goodsCon[8] = town.goodsPro[9] * 4;
	town.goodsCon[10] = town.goodsPro[11] * 2;
	town.goodsCon[12] = town.goodsPro[13] * 3;
	town.goodsCon[14] = town.goodsPro[15] * 3;
	town.goodsCon[15] = town.goodsPro[16];
	town.goodsCon[17] = town.goodsPro[18] * 2;

	for (int i = 2; i < GOODS; i++) {
		if (town.goodsPro[i] >= town.goodsCon[i]) {
			town.exSum += (town.goodsPro[i] - town.goodsCon[i]) * gData.value[i];
		}
		else {
			town.inSum += (town.goodsCon[i] - town.goodsPro[i]) * gData.value[i];
			town.inSum *= 2.5;
		}
	}

	town.exFin = town.exSum * (1 + double(town.goodsPro[1] / 10));
	town.inFin = town.inSum * (1 - double(town.goodsPro[1] / 100));

	if (town.exFin - town.inFin - int(town.exFin - town.inFin) >= 0.5) {
		town.trade += town.exFin - town.inFin + 1;
	}
	else if (town.inFin - town.exFin - int(town.inFin - town.exFin) >= 0.5){
		town.trade += town.exFin - town.inFin - 1;
	}
	else {
		town.trade += town.exFin - town.inFin;
	}

	//town.income[MONEY] += town.trade[VALUE] * double (town.trade[POWER] / 5);
	/*if (showTrade){
		DrawString(10 + G_SIZE + 15, 50, "âø", YELLOW);
		DrawString(10 + G_SIZE + 52, 50, "ê∂", GREEN);
		DrawString(10 + G_SIZE + 82, 50, "è¡", RED);
		DrawString(10 + G_SIZE + 15, 70, "íl", YELLOW);
		DrawString(10 + G_SIZE + 51, 70, "éY", GREEN);
		DrawString(10 + G_SIZE + 82, 70, "îÔ", RED);

		DrawString(160 + G_SIZE + 15, 50, "âø", YELLOW);
		DrawString(160 + G_SIZE + 52, 50, "ê∂", GREEN);
		DrawString(160 + G_SIZE + 82, 50, "è¡", RED);
		DrawString(160 + G_SIZE + 15, 70, "íl", YELLOW);
		DrawString(160 + G_SIZE + 51, 70, "éY", GREEN);
		DrawString(160 + G_SIZE + 82, 70, "îÔ", RED);

		for (int i = 2; i < GOODS; i++){
			if (i < GOODS / 2 + 1){
				g_goods.Draw(10, 100 + (i - 2) * G_SIZE, i);
				DrawFormatString(10 + G_SIZE + 5, 100 + (i - 2) * G_SIZE, YELLOW, "%.2lf", gData.value[i]);
				DrawFormatString(10 + G_SIZE + 60, 100 + (i - 2) * G_SIZE, GREEN, "%d", town.goodsPro[i]);
				DrawFormatString(10 + G_SIZE + 90, 100 + (i - 2) * G_SIZE, RED, "%d", town.goodsCon[i]);
			}
			else {
				g_goods.Draw(160, 100 + (i - 1 - GOODS / 2) * G_SIZE, i);
				DrawFormatString(160 + G_SIZE + 5, 100 + (i - 1 - GOODS / 2) * G_SIZE, YELLOW, "%.2lf", gData.value[i]);
				DrawFormatString(160 + G_SIZE + 60, 100 + (i - 1 - GOODS / 2) * G_SIZE, GREEN, "%d", town.goodsPro[i]);
				DrawFormatString(160 + G_SIZE + 90, 100 + (i - 1 - GOODS / 2) * G_SIZE, RED, "%d", town.goodsCon[i]);
			}
		}

		g_goods.Draw(110, 400, 1);
		DrawFormatString(155, 412, CYAN, "%d", town.goodsPro[1]);

		DrawFormatString(75, 450, GREEN, "óAèoäzÅF%.2lf", town.exSum);
		DrawFormatString(75, 470, RED, "óAì¸äzÅF%.2lf", town.inSum);

		DrawFormatString(75, 490, YELLOW, "é˚éx:%d", town.trade);

		g_rBut.Draw(75, 520);
		if (Event.LMouse.GetClick(74, 519, 183, 670)){
			showTrade = false;
			flag = true;
		}
	}
	else {*/
		for (int i = 0; i < RESOURCES; i++){
			g_resource.Draw(75, 100 + I_SIZE * i, i);
		}

		for (int i = 0; i < STATS; i++){
			g_stats.Draw(75, 300 + i * I_SIZE, i); 
		}

		DrawFormatString(75 + I_SIZE + 5, 102, YELLOW, "%d (+%d)(åà’:%d)", town.resource[MONEY], town.income[MONEY], town.trade);
		if (town.income[FOOD] - town.devSum >= 0){
			DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE, GREEN, "%d (+%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
		}
		else {
			DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE, GREEN, "%d (%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
			if (town.resource[FOOD] + town.income[FOOD] - town.devSum < 0) {
				g_starve.Draw(75, 120);
			}
		}
		DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE * 2, ORANGE, "%d (+%d)", town.resource[WOOD], town.income[WOOD]);

		DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE * 3, GRAY, "%d (+%d)", town.resource[STONE], town.income[STONE]);

		//DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "%d", town.trade[VALUE]);

		//DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, PURPLE, "%d", town.trade[POWER]);

		//for (int i = 1; i < GOODS; i++){
		//	DrawFormatString(75 + I_SIZE * ((i - 1) % 8) + 5, 302 + I_SIZE * ((i - 1) / 8), WHITE, "%d", town.goodsPro[i] - town.goodsCon[i]);
		//} 

		DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "ëSëÃêlå˚:%d", town.devSum);

		DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, WHITE, "ìsésêî:%d/%d", town.towns, town.townMax);

	//}

	DrawString(50, 10, "EnterÇ≈é˚ì¸Çälìæ", WHITE);
	DrawString(50, 30, "[s]Ç≈ÉZÅ[Éu", WHITE);
	DrawString(50, 50, "[l]Ç≈ÉçÅ[Éh", WHITE);

	ETown u = WILD, d = WILD, l = WILD, r = WILD;

	loaded = false;

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			if (tile[i].terrain != RIVER/* && tile[i].terrain != HILL*/) {
				g_tile.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);
			}
			else {
				DrawRiver(i);
			}
			/*else {
				g_hill.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].mineral);
			}*/

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].town);
			}
			else if (tile[i].terrain != RIVER){
				if (tile[i].saNum != 0) {
					g_sArea.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].saNum);
				}
				if (tile[i].built[0]) {
					g_pasture.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);
				}
				if (tile[i].built[1]) {
					g_pasture.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain + TERRAINS);
				}
				if (tile[i].built[2]) {
					g_pasture.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain + TERRAINS * 2);
				}
			}

			/////
			/*if (tile[i].fac[0]){
				DrawCircle(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT + 30, i / BLOCKS_X * GRID + 30, 10, RED);
			}*/
			/////
		}

		g_tBut.Draw(75, 520);
		if (Event.LMouse.GetClick(74, 519, 183, 570)/* && !flag*/) {
			//showTrade = true;

			gbox = new STradeBox(town);
			boxStatus = T_DATA;
		}

		g_bBut.Draw(75, 430);
		if (Event.LMouse.GetClick(74, 429, 175, 480)/* && !flag*/) {
			bbox = new SBuyBox(town);
			boxStatus = BUY;
		}

		if (Event.RMouse.GetOn(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
			g_frame.Draw(Event.RMouse.GetX() - (Event.RMouse.GetX()) % GRID, Event.RMouse.GetY() - (Event.RMouse.GetY()) % GRID);
		}

		if (Event.key.GetDown(Event.key.RETURN)){
			for (int i = 0; i < RESOURCES; i++){
				town.resource[i] += town.income[i];
			}
			//town.resource[MONEY] += town.trade[VALUE] * town.trade[POWER];
			town.resource[MONEY] += town.trade;
			town.resource[FOOD] -= town.devSum;
			if (town.resource[FOOD] < 0) {
				P none, box;
				none.first = 0;
				none.second = 0;
				stv.assign(0, none);
				for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
					if (tile[i].town != WILD) {
						box.first = tile[i].townLv;
						box.second = i;
						stv.push_back(box);
					}
				}
				std::sort(stv.begin(), stv.end());
			}
			while (town.resource[FOOD] < 0 && !stv.empty()) {
				if (stv.operator[](0).first <= 0) {
					tile[stv.operator[](0).second].town = WILD;
					stv.erase(stv.begin());
				}
				else {
					tile[stv.operator[](0).second].townLv--;
					stv.operator[](0).first--;
				}
				std::sort(stv.begin(), stv.end());
				town.resource[FOOD]++;
			}
		}

		if (Event.key.GetDown(Event.key.S)){
			WriteData();
		}

		if (Event.key.GetDown(Event.key.L)){
			ReadData();
			loaded = true;
		}

		break;

	case FOUND:

		fbox->DrawFB(town.resource[MONEY]/*, tile[infoNum].mineral*/);

		break;

	case TOWN:

		tbox->DrawTB();

		break;

	case RBUILD:

		if (infoNum / BLOCKS_X != 0) {
			u = tile[infoNum - BLOCKS_X].town;
		}
		if (infoNum / BLOCKS_X != BLOCKS_Y - 1) {
			d = tile[infoNum + BLOCKS_X].town;
		}
		if (infoNum % BLOCKS_X != 0) {
			l = tile[infoNum - 1].town;
		}
		if (infoNum % BLOCKS_X != BLOCKS_X - 1) {
			r = tile[infoNum + 1].town;
		}
		rbox->DrawRB(u, d, l, r);

		break;

	case T_DATA:

		gbox->DrawTB();

		break;

	case BUY:

		bbox->DrawBB(town.resource[MONEY]);
		if (bbox->bought) {
			town.resource[MONEY] -= bbox->cost;
			town.resource[bbox->rType] += bbox->amount;
		}

		break;

	case PST:

		pbox->DrawPB();

		break;

	default:
		break;
	}
}

void CTileManager::WriteData(){
	/*std::ofstream of;
	of.open("SaveData.txt", ios::out |ios::binary | ios::trunc);
	unsigned char buf;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		buf = tile[i].terrain;
		of << buf;
		buf = tile[i].town;
		of << buf;
		buf = tile[i].townLv;
		of << buf;
		buf = 0;
		for (int j = 0; j < BUILDINGS + SP_BUILDS; j++){
			if (tile[i].built[j]){
				buf |= (1 << j);
			}
		}
		of << buf;
	}*/

	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		data->Set(i * 100, tile[i].terrain);
		data->Set(i * 100 + 1, tile[i].town);
		data->Set(i * 100 + 2, tile[i].townLv);
		data->Set(i * 100 + 3, tile[i].saNum);
		for (int j = 0; j < BUILDINGS + SP_BUILDS; j++) {
			if (tile[i].built[j]) {
				data->Set(i * 100 + 10 + j, 1);
			}
			else {
				data->Set(i * 100 + 10 + j, 0);
			}
		}
	}
	for (int i = 0; i < RESOURCES; i++) {
		data->Set(10000 + i, town.resource[i]);
	}
	data->WriteAll();
}

void CTileManager::ReadData(){
	/*std::ifstream ifs;
	ifs.open("SaveData.txt", ios::in |ios::binary);
	unsigned char buf;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		ifs >> buf;
		tile[i].terrain = (ETerrain)buf;
		ifs >> buf;
		tile[i].town = (ETown)(buf);
		ifs >> buf;
		tile[i].townLv = buf;
		ifs >> buf;
		for (int j = 0; j < BUILDINGS + SP_BUILDS; j++){
			if ((buf & (1 << j)) != 0){
				tile[i].built[j] = true;
			}
		}
	}*/

	data->ReadAll();
	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		tile[i].terrain = (ETerrain)(data->GetInt(i * 100));
		tile[i].town = (ETown)(data->GetInt(i * 100 + 1));
		tile[i].townLv = data->GetInt(i * 100 + 2);
		tile[i].saNum = data->GetInt(i * 100 + 3);
		for (int j = 0; j < BUILDINGS + SP_BUILDS; j++) {
			if (data->GetInt(i * 100 + 10 + j) == 0) {
				tile[i].built[j] = false;
			}
			else {
				tile[i].built[j] = true;
			}
		}
	}
	for (int i = 0; i < RESOURCES; i++) {
		town.resource[i] = data->GetInt(10000 + i);
	}
}