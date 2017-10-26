#include"TileM.h"
#include<queue>
#include<vector>
#include<list>
#include<fstream>
#include<algorithm>

//1.search all adjacent road tiles with <queue>
//2.change all adjacent road tiles' connect[a.a.r.t.] to true with <vector>

using namespace std;
typedef std::pair<int, int> P;

//queue<int> q;
int now, nxt;
int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
//vector<int> v;
vector<int> vec, res, frs;
vector<P> stv;
P none;

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
CPicture CTileManager::g_hBut;
CPicture CTileManager::g_back;

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
		int buf = 0;
		fscanf(f, "%d", &buf);
		for (int j = 0; j <= ITEMS; j++) {
			bufItem[i][j] = false;
		}
		switch (buf) {
		case 1:
			bufItem[i][1] = true;
			fscanf(f, "%d", &iExpL[i][1]);
			for (int j = 0; j < iExpL[i][1]; j++) {
				fscanf(f, "%s", &iExp[i][1][j]);
			}
			iExpL[i][2] = 0;
			break;

		case 2:
			bufItem[i][2] = true;
			iExpL[i][1] = 0;
			fscanf(f, "%d", &iExpL[i][2]);
			for (int j = 0; j < iExpL[i][2]; j++) {
				fscanf(f, "%s", &iExp[i][2][j]);
			}
			break;

		case 3:
			bufItem[i][1] = true;
			bufItem[i][2] = true;
			fscanf(f, "%d", &iExpL[i][1]);
			for (int j = 0; j < iExpL[i][1]; j++) {
				fscanf(f, "%s", &iExp[i][1][j]);
			}
			fscanf(f, "%d", &iExpL[i][2]);
			for (int j = 0; j < iExpL[i][2]; j++) {
				fscanf(f, "%s", &iExp[i][2][j]);
			}
			break;

		default:
			break;
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
		fscanf(f, "%d", &expL[i]);
		for (int j = 0; j < expL[i]; j++) {
			fscanf(f, "%s", &exp[i][j]);
		}
	}
	fclose(f);
}

STile::STile(){
	terrain = PLAIN;
	//mineral = ROCK;
	town = WILD;
	townLv = 0;
	devLim = 0;
	saNum = 0;
	saLv = 0;
	hidMin = 0;
	for (int i = 1; i <= ITEMS; i++) {
		itemUse[i] = false;
	}
	for (int i = 0; i < BUILDINGS + SP_BUILDS; i++){
		built[i] = false;
	}
	for (int i = 0; i < RESOURCES; i++) {
		buf[i] = 0;
	}
	//fac[0] = false;
	for (int i = 0; i < RESOURCES; i++){
		produce[i] = 0;
	}
	/*for (int i = 0; i < TRADE; i++){
		trade[i] = 0;
	}*/
	for (int i = 0; i < GOODS; i++){
		goods[i] = 0;
	}
	for (int i = 0; i < ITEMS; i++) {
		itemCon[i] = 0;
	}
}

void STile::Set() {
	terrain = PLAIN;
	//mineral = ROCK;
	town = WILD;
	townLv = 0;
	devLim = 0;
	saNum = 0;
	saLv = 0;
	hidMin = 0;
	for (int i = 1; i <= ITEMS; i++) {
		itemUse[i] = false;
	}
	for (int i = 0; i < BUILDINGS + SP_BUILDS; i++) {
		built[i] = false;
	}
	for (int i = 0; i < RESOURCES; i++) {
		buf[i] = 0;
	}
	for (int i = 0; i < RESOURCES; i++) {
		produce[i] = 0;
	}
	for (int i = 0; i < GOODS; i++) {
		goods[i] = 0;
	}
	for (int i = 0; i < ITEMS; i++) {
		itemCon[i] = 0;
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
		for (int i = 0; i <= ITEMS; i++) {
			itemCon[i] = 0;
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

			for (int j = 0; j <= ITEMS; j++) {
				itemCon[j] = 0;
			}

			if (itemUse[1]) {
				switch (town) {
				case FARM:
					produce[i] *= 1.2;
					break;

				case F_VIL:
					produce[i] *= 1.34;
					break;

				case MINE_S:
					produce[i] *= 1.5;
					break;

				case MINE_G:
					produce[i] *= 1.5;
					break;

				case MINE_I:
					produce[i] *= 1.5;
					break;

				default:
					break;
				}
				itemCon[1] = 1;
			}
			for (int j = 0; j < BUILDINGS; j++) {
				if (built[j]) {
					produce[i] += bData.income[town][j][i];
				}
			}
			if (itemUse[2]) {
				switch (town) {
				case MINE_G:
					if (built[0]) {
						produce[i] += bData.income[town][0][i];
					}
					break;

				case MINE_I:
					if (built[0]) {
						produce[i] += bData.income[town][0][i];
					}
					if (built[2]) {
						produce[i] += bData.income[town][2][i];
					}
					break;

				default:
					break;
				}
				itemCon[2] = 1;
			}
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

		if (itemUse[1] && built[1]) {
			switch (town) {
			case MINE_G:
				goods[12] += 2;
				break;

			case MINE_I:
				goods[14] += 2;
				break;

			default:
				break;
			}
		}

		if (itemUse[2] && built[2] && town == MINE_I) {
			goods[16]++;
		}

		if (town == MINE_G && built[4]) {
			goods[13] += goods[12] / 2 - 1;
			if (itemUse[2]) {
				goods[13]++;
			}
		}
		if (town == MINE_I && built[4]) {
			goods[15] += goods[14] / 2 - 1;
			if (itemUse[2]) {
				goods[15]++;
			}
		}


		for (int i = 0; i < RESOURCES; i++) {
			buf[i] = 0;
		}

		if (adjRB[0] > 0) {
			if (built[2]) {
				goods[3] += adjRB[0];
			}
			buf[FOOD] += 0.25 * adjRB[0];
		}

		for (int i = 0; i < RESOURCES; i++) {
			produce[i] = (1.0 + buf[i]) * ((double)produce[i]);
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

		goods[1] *= 1.0 + buf[RESOURCES];
	}
}

STown::STown(){
	Set();

	for (int i = 0; i < RESOURCES; i++){
		resource[i] = 100;
	}
	resource[FOOD] = 0;
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
	for (int i = 0; i < ITEMS; i++) {
		itemCon[i] = 0;
	}

	devSum = 0;
	towns = 0;
	townMax = 5;
	exSum = 0;
	inSum = 0;
	exFin = 0;
	inFin = 0;
	trade = 0;
	maxMines = 0;
	foodMax = 0;
	cPas = 0;
	foodCon = 0;
	spFarm = false;

	for (int i = 0; i <= SP_AREAS; i++) {
		onlyArea[i] = false;
	}
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
	while (c < 7 && !vec.empty()) {
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
	/*res.erase(res.begin() + n);
	tile[res.operator[](GetRand(res.size() - 1))].terrain = HILL_I;*/

	res.resize(0, 0);
	vec.resize(0, 0);
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		tflag[i] = false;
	}
	c = 0;
	n = GetRand(BLOCKS_X * BLOCKS_Y - 1);
	while (tile[n].terrain == RIVER || tile[n].terrain == HILL_S && tile[n].terrain == HILL_G && tile[n].terrain == HILL_I) {
		n = GetRand(BLOCKS_X * BLOCKS_Y - 1);
	}
	tflag[n] = true;
	vec.push_back(n);
	while (c < 6 && !vec.empty()) {
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
	tile[res.operator[](n)].terrain = HILL_I;

	res.resize(0, 0);
	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		if (tile[i].terrain == HILL_S) {
			res.push_back(i);
		}
	}
	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].hidMin = 1;
	res.erase(res.begin() + n);
	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].hidMin = 2;
	res.erase(res.begin() + n);
	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].hidMin = 2;

	res.resize(0, 0);
	vec.resize(0, 0);

	openInfo = false;
	boxStatus = NO;
	infoNum = -1;
	showTrade = false;
	loaded = false;
	turn = 1;
	howTo = false;
	back = false;
	end = false;

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
	g_hBut.Load("Chikuwa3/HButton.png");
	g_back.Load("Chikuwa3/Back.png");
}

void CTileManager::OpenInfo(){
	infoNum = -1;
	bool info = false;

	if (Event.LMouse.GetClick(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
		info = true;
		infoNum = (Event.RMouse.GetX() + WINDOW_HEIGHT - WINDOW_WIDTH) / GRID + (Event.RMouse.GetY()) / GRID * BLOCKS_X;
	}

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
					bool flag[5] = {};
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

						fbox = new SFoundBox(tile[infoNum].terrain, town, flag[0], flag[1], flag[2], flag[3], tile[infoNum].adjSer);
						boxStatus = FOUND;

						break;

					default:

						pbox = new SPastureBox(tile[infoNum], town.resource[MONEY], num - 1);
						boxStatus = PST;

						break;
					}
				}
				else {
					sbox = new SSpABox(town, tile[infoNum]);
					boxStatus = S_AREA;
				}
			}
			else {
				bool flag = false;
				if (tile[infoNum].adjSer || (tile[infoNum].town == MINE_S && tile[infoNum].built[3] && tile[infoNum].hidMin != 3)) {
					flag = true;
				}

				tbox = new STownBox(town, tile[infoNum], flag);
				boxStatus = TOWN;
			}
		}
	}
}

void CTileManager::CloseInfo(){
	openInfo = false;

	bool pFlag = false;
	bool rFlag[3] ={};
	bool cFlag = false;
	bool pFlags[3] = {};
	switch (boxStatus){
	case FOUND:

		if (tile[infoNum].terrain == FOREST) {
			for (int i = 0; i < 4; i++) {
				if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
					if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town != WILD) {
						cFlag = true;
					}
				}
			}
		}

		switch (fbox->mode){
		case CLOSE:
			delete fbox;
			boxStatus = NO;
			break;

		case EST:
			tile[infoNum].town = fbox->town;
			for (int i = 0; i < RESOURCES; i++){
				//tile[infoNum].produce[i] = tData.income[fbox->town][NEW][i];
				town.resource[i] -= tData.cost[fbox->town][NEW][i];
			}
			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = tData.trade[fbox->town][NEW][i];
			}*/
			music->AddSound(sFound);
			delete fbox;
			boxStatus = NO;
			break;

		case CUT:
			tile[infoNum].terrain = PLAIN;
			town.resource[MONEY] -= 50;
			for (int i = 0; i < 4; i++) {
				if (infoNum % BLOCKS_X + dx[i] >= 0 && infoNum % BLOCKS_X + dx[i] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] >= 0 && infoNum / BLOCKS_X + dy[i] < BLOCKS_Y) {
					if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == PAS_C && tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
						pFlags[1] = true;
					}
				}
			}
			UDData();
			fbox->UpDate(tile[infoNum].terrain, town, false, pFlags[0], pFlags[1], pFlags[2], tile[infoNum].adjSer);
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

		case SEARCH:
			town.resource[MONEY] -= 80;
			switch (tile[infoNum].hidMin) {
			case 1:
				tile[infoNum].terrain = HILL_G;
				break;

			case 2:
				tile[infoNum].terrain = HILL_I;
				break;

			default:
				tile[infoNum].hidMin = 3;
				break;
			}
			UDData();
			fbox->UpDate(tile[infoNum].terrain, town, cFlag, pFlags[0], pFlags[1], pFlags[2], tile[infoNum].adjSer);
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
			for (int i = 1; i <= ITEMS; i++) {
				tile[infoNum].itemUse[i] = false;
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
					if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].town == WILD) {
						if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].terrain == RIVER){
							if (tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0]) {
								for (int k = 0; k < 4; k++) {
									if (infoNum%BLOCKS_X + dx[i] + dx[k] >= 0 && infoNum%BLOCKS_X + dx[i] + dx[k] < BLOCKS_X && infoNum / BLOCKS_X + dy[i] + dy[k] >= 0 && infoNum / BLOCKS_X + dy[i] + dy[k] < BLOCKS_Y) {
										if (tile[infoNum + dx[i] + dx[k] + (dy[i] + dy[k]) * BLOCKS_X].town == FARM) {
											rFlag[0] = true;
										}
									}
								}
							}
							if (!rFlag[0]) {
								tile[infoNum + dx[i] + dy[i] * BLOCKS_X].built[0] = false;
							}
						}
						else {
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
			/*delete tbox;*/
			UDData();
			tbox->UpDate(town, tile[infoNum]);
			music->AddSound(sDev);
			break;
			
		case DG:
			//tile[infoNum].townLv = tbox->devLv;
			tile[infoNum].townLv--;
			//tile[infoNum].produce[i] += tData.income[tile[infoNum].town][LVUP][i];
			town.resource[MONEY] -= tData.cost[tile[infoNum].town][LVUP][MONEY] / 2;
			town.resource[WOOD] += tData.cost[tile[infoNum].town][LVUP][WOOD] / 4;
			town.resource[STONE] += tData.cost[tile[infoNum].town][LVUP][STONE] / 4;

			/*for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += tData.trade[tile[infoNum].town][LVUP][i];
			}*/
			UDData();
			tbox->UpDate(town, tile[infoNum]);
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
			UDData();
			tbox->UpDate(town, tile[infoNum]);
			music->AddSound(sBuild);
			break;

		case DEMO:
			tile[infoNum].built[tbox->buildNum] = false;
			town.resource[MONEY] -= bData.cost[tile[infoNum].town][tbox->buildNum][MONEY] / 2;
			/*for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] -= bData.income[tile[infoNum].town][tbox->buildNum][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] -= bData.trade[tile[infoNum].town][tbox->buildNum][i];
			}*/
			UDData();
			tbox->UpDate(town, tile[infoNum]);
			break;

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
			UDData();
			tbox->UpDate(town, tile[infoNum]);
			break;

		case USE:
			tile[infoNum].itemUse[tbox->iNum] = true;
			UDData();
			town.itemCon[tbox->iNum - 1]++;
			tbox->UpDate(town, tile[infoNum]);
			break;

		case END_USE:
			tile[infoNum].itemUse[tbox->iNum] = false;
			UDData();
			town.itemCon[tbox->iNum - 1]--;
			tbox->UpDate(town, tile[infoNum]);
			break;

		case SEARCH:
			town.resource[MONEY] -= 80;
			switch (tile[infoNum].hidMin) {
			case 1:
				tile[infoNum].terrain = HILL_G;
				tile[infoNum].town = MINE_G;
				tile[infoNum].built[0] = false;
				tile[infoNum].built[2] = false;
				break;

			case 2:
				tile[infoNum].terrain = HILL_I;
				tile[infoNum].town = MINE_I;
				tile[infoNum].built[0] = false;
				tile[infoNum].built[2] = false;
				break;

			default:
				tile[infoNum].hidMin = 3;
				break;
			}
			UDData();
			tbox->UpDate(town, tile[infoNum]);
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
			UDData();
			rbox->UpDate(town, tile[infoNum]);
			break;

		case DEMO:
			tile[infoNum].built[rbox->buildNum] = false;
			town.resource[MONEY] -= rbData.cost[rbox->buildNum][MONEY] / 2;
			UDData();
			rbox->UpDate(town, tile[infoNum]);
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

	case S_AREA:

		switch (sbox->mode) {
		case CLOSE:
			delete sbox;
			boxStatus = NO;
			break;

		case DEV:
			tile[infoNum].saLv++;
			for (int i = 0; i < RESOURCES; i++) {
				town.resource[i] -= saData.cost[tile[infoNum].saNum][i] / 2;
			}
			UDData();
			sbox->UpDate(town, tile[infoNum]);
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

void CTileManager::CheckAdjB() {
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		for (int j = 0; j < R_BUILDS; j++) {
			tile[i].adjRB[j] = 0;
		}
		tile[i].adjSer = false;
	}

	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if (tile[i].town == FARM){
			for (int j = 0; j < 4; j++){
				if (i % BLOCKS_X + dx[j] >= 0 && i % BLOCKS_X + dx[j] < BLOCKS_X && i / BLOCKS_X + dy[j] >= 0 && i / BLOCKS_X + dy[j] < BLOCKS_Y){
					if (tile[i + dx[j] + dy[j] * BLOCKS_X].terrain == RIVER && tile[i + dx[j] + dy[j] * BLOCKS_X].built[0]){
						tile[i].adjRB[0]++;
					}
				}
			}
			if (tile[i].adjRB[0] > 2) {
				tile[i].adjRB[0] = 2;
			}
		}

		if ((tile[i].town == MINE_S || tile[i].town == MINE_G || tile[i].town == MINE_I) && tile[i].built[3]) {
			for (int j = 0; j < 4; j++) {
				if (i % BLOCKS_X + dx[j] >= 0 && i % BLOCKS_X + dx[j] < BLOCKS_X && i / BLOCKS_X + dy[j] >= 0 && i / BLOCKS_X + dy[j] < BLOCKS_Y) {
					if (tile[i + dx[j] + dy[j] * BLOCKS_X].terrain == HILL_S /*&& tile[i + dx[j] + dy[j] * BLOCKS_X].town == 0*/ && tile[i + dx[j] + dy[j] * BLOCKS_X].hidMin != 3) {
						tile[i + dx[j] + dy[j] * BLOCKS_X].adjSer = true;
					}
				}
			}
		}
	}
	/*for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
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
	}*/
}

void CTileManager::UDData() {
	CheckAdjB();
	town.forests = 0;
	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		if (tile[i].terrain == FOREST && tile[i].town == WILD && tile[i].saNum == 0 && !tile[i].built[0] && !tile[i].built[1] && !tile[i].built[2]) {
			town.forests++;
		}
	}
	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		tile[i].SetProduce();
	}
}

void CTileManager::Draw(){
	if (boxStatus == NO){
		OpenInfo();
	}
	else{
		CloseInfo();
		UDData();
	}

	if (loaded){
		CloseInfo();
		UDData();
	}

	town.Set();
	int cNum = 0;

	for (int i = 0; i <= TOWNS; i++) {
		town.tTypePop[i] = 0;
	}
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if (tile[i].town != WILD){
			town.tTypePop[tile[i].town] += tile[i].townLv + 1;
			town.devSum += tile[i].townLv + 1;
			town.foodCon += tile[i].townLv + 1;
			town.towns++;
		}

		if (tile[i].town == COMM){
			if (tile[i].built[3]) {
				town.onlyOne[T_HALL] = true;
				town.townMax += 5;
			}

			if (tile[i].built[4]) {
				town.onlyOne[COURT] = true;
				town.townMax += 10;
			}
		}

		if ((tile[i].town == MINE_S || tile[i].town == MINE_G || tile[i].town == MINE_I) && tile[i].townLv >= 4){
			town.maxMines++;
		}

		if (tile[i].town == FARM && tile[i].built[4]) {
			town.foodMax += 5;
		}
		
		if (tile[i].town == WILD && tile[i].terrain != RIVER && tile[i].built[1]) {
			town.cPas++;
		}

		if (tile[i].saNum == 6) {
			town.foodCon += 10 * (1 + tile[i].saLv);
			town.spFarm = true;
			town.goodsPro[21] += 1 + tile[i].saLv;
		}

		if (tile[i].saNum == 7) {
			town.goodsPro[20] += min(1 + tile[i].saLv, town.forests);
			cNum = tile[i].saLv + 1;
		}

		for (int j = 0; j < ITEMS; j++) {
			town.itemCon[j] += tile[i].itemCon[j + 1];
		}

		if (tile[i].saNum != 0) {
			town.onlyArea[tile[i].saNum] = true;
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

	for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
		if (tile[i].saNum == 8) {
			town.goodsPro[18] += min(1 + tile[i].saLv, town.goodsPro[20] - town.itemCon[1]) * 2;
			town.tex = min(1 + tile[i].saLv, town.goodsPro[20] - town.itemCon[1]) * 2;
			town.itemCon[1] += min(1 + tile[i].saLv, town.goodsPro[20] - town.itemCon[1]);
		}
	}

	town.goodsCon[2] = town.goodsPro[3] * 3;
	town.goodsCon[7] = town.goodsPro[8];
	town.goodsCon[8] = town.goodsPro[9] * 3;
	town.goodsCon[10] = town.goodsPro[11] * 2;
	town.goodsCon[12] = town.goodsPro[13] * 2;
	town.goodsCon[14] = town.goodsPro[15] * 2;
	town.goodsCon[15] = town.goodsPro[16];
	town.goodsCon[17] = town.goodsPro[18] * 2;
	town.goodsCon[16] = town.itemCon[0];
	town.goodsCon[20] += town.itemCon[1];

	if (town.goodsCon[16] > town.goodsPro[16]) {
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
			if (tile[i].itemUse[1]) {
				tile[i].itemUse[1] = false;
				town.goodsCon[16]--;
				town.itemCon[0]--;
			}
			if (town.goodsCon[16] == town.goodsPro[16]) {
				break;
			}
		}
	}
	if (town.goodsCon[20] > town.goodsPro[20]) {
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
			if (tile[i].itemUse[2]) {
				tile[i].itemUse[2] = false;
				town.goodsCon[20]--;
				town.itemCon[1]--;
			}
			if (town.goodsCon[20] == town.goodsPro[20]) {
				break;
			}
		}
	}

	for (int i = 2; i < GOODS; i++) {
		if (town.goodsPro[i] >= town.goodsCon[i]) {
			town.exSum += ((double)town.goodsPro[i] - town.goodsCon[i]) * gData.value[i];
		}
		else {
			town.inSum += ((double)town.goodsCon[i] - town.goodsPro[i]) * gData.value[i];
		}
	}

	town.exFin = town.exSum * (1.0 + double(town.goodsPro[1]) / 100);
	town.inFin = town.inSum * (1.0 - double(town.goodsPro[1]) / 100) * 2.5;

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
			g_resource.Draw(50, 100 + I_SIZE * i, i);
		}

		for (int i = 0; i < STATS; i++){
			g_stats.Draw(50, 300 + i * I_SIZE, i);
		}

		DrawFormatString(50 + I_SIZE + 5, 102, YELLOW, "%d (+%d)(åà’:%d)", town.resource[MONEY], town.income[MONEY], town.trade);
		if (town.income[FOOD] - town.foodCon >= 0){
			DrawFormatString(50 + I_SIZE + 5, 102 + I_SIZE, GREEN, "%d/%d (ê∂éY+%d,è¡îÔ-%d)", town.resource[FOOD], town.foodMax, town.income[FOOD], town.foodCon);
		}
		else {
			DrawFormatString(50 + I_SIZE + 5, 102 + I_SIZE, RED, "%d/%d (ê∂éY+%d,è¡îÔ-%d)", town.resource[FOOD], town.foodMax, town.income[FOOD], town.foodCon);
			if (town.resource[FOOD] + town.income[FOOD] - town.foodCon < 0) {
				g_starve.Draw(50, 120);
			}
		}
		DrawFormatString(50 + I_SIZE + 5, 102 + I_SIZE * 2, ORANGE, "%d (+%d)", town.resource[WOOD], town.income[WOOD]);

		DrawFormatString(50 + I_SIZE + 5, 102 + I_SIZE * 3, GRAY, "%d (+%d)", town.resource[STONE], town.income[STONE]);

		//DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "%d", town.trade[VALUE]);

		//DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, PURPLE, "%d", town.trade[POWER]);

		//for (int i = 1; i < GOODS; i++){
		//	DrawFormatString(75 + I_SIZE * ((i - 1) % 8) + 5, 302 + I_SIZE * ((i - 1) / 8), WHITE, "%d", town.goodsPro[i] - town.goodsCon[i]);
		//} 

		DrawFormatString(50 + I_SIZE + 5, 302, WHITE, "ëSëÃêlå˚:%d", town.devSum);

		DrawFormatString(50 + I_SIZE + 5, 302 + I_SIZE, WHITE, "ìsésêî:%d/%d", town.towns, town.townMax);

	//}

	DrawString(50, 10, "EnterÇ≈é˚ì¸Çälìæ", WHITE);
	DrawString(50, 30, "[s]Ç≈ÉZÅ[Éu", WHITE);
	DrawString(50, 50, "[l]Ç≈ÉçÅ[Éh", WHITE);

	DrawFormatString(50,70,WHITE,"É^Å[ÉìÅF%d/100",turn);

	ETown u = WILD, d = WILD, l = WILD, r = WILD;

	loaded = false;
	int buf = 0;

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
			if (tile[i].hidMin == 1) {
				g_goods.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, 13);
			}
			if (tile[i].hidMin == 2) {
				g_goods.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, 15);
			}
			/////
		}

		g_tBut.Draw(75, 350);
		if (Event.LMouse.GetClick(74, 349, 183, 400)/* && !flag*/) {
			//showTrade = true;

			gbox = new STradeBox(town);
			boxStatus = T_DATA;
		}

		g_bBut.Draw(75, 410);
		if (Event.LMouse.GetClick(74, 409, 175, 460)/* && !flag*/) {
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
			town.resource[FOOD] -= town.foodCon;
			turn++;
			if (town.resource[FOOD] < 0) {
				P box;
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
				if (stv.operator[](stv.size() - 1).first <= 0) {
					int num = stv.operator[](stv.size() - 1).second;
					tile[num].town = WILD;
					for (int i = 0; i < BUILDINGS + SP_BUILDS; i++) {
						tile[num].built[i] = false;
					}
					bool pFlag = false;
					for (int i = 0; i < 4; i++) {
						if (num%BLOCKS_X + dx[i] >= 0 && num%BLOCKS_X + dx[i] < BLOCKS_X && num / BLOCKS_X + dy[i] >= 0 && num / BLOCKS_X + dy[i] < BLOCKS_Y) {
							if (tile[num + dx[i] + dy[i] * BLOCKS_X].town == WILD && tile[num + dx[i] + dy[i] * BLOCKS_X].terrain != RIVER) {
								for (int j = 0; j < 3; j++) {
									if (tile[num + dx[i] + dy[i] * BLOCKS_X].built[j]) {
										for (int k = 0; k < 4; k++) {
											if (num%BLOCKS_X + dx[i] + dx[k] >= 0 && num%BLOCKS_X + dx[i] + dx[k] < BLOCKS_X && num / BLOCKS_X + dy[i] + dy[k] >= 0 && num / BLOCKS_X + dy[i] + dy[k] < BLOCKS_Y) {
												if (tile[num + dx[i] + dx[k] + (dy[i] + dy[k]) * BLOCKS_X].town == PAS_S + j && tile[num + dx[i] + dx[k] + (dy[i] + dy[k]) * BLOCKS_X].built[0]) {
													pFlag = true;
												}
											}
										}

										if (!pFlag) {
											tile[num + dx[i] + dy[i] * BLOCKS_X].built[j] = false;
										}
									}
								}
							}
						}
					}
					stv.erase(stv.begin());
				}
				else {
					tile[stv.operator[](stv.size() - 1).second].townLv--;
					stv.operator[](stv.size() - 1).first--;
				}
				std::sort(stv.begin(), stv.end());
				town.resource[FOOD]++;

				CloseInfo();
				CheckAdjB();
				for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
					tile[i].SetProduce();
				}
			}


			if (cNum > 0) {
				frs.assign(0, 0);
				for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
					if (tile[i].terrain == FOREST && tile[i].town == WILD && tile[i].saNum == 0 && !tile[i].built[0] && !tile[i].built[1] && !tile[i].built[2]) {
						frs.push_back(i);
					}
				}
				while (cNum > 0 && !frs.empty()) {
					buf = GetRand(frs.size() - 1);
					tile[frs.operator[](buf)].terrain = PLAIN;
					frs.erase(frs.begin() + buf);
					cNum--;
				}
				frs.resize(0, 0);
			}

			town.resource[FOOD] = min(town.resource[FOOD], town.foodMax);
			town.resource[FOOD] = max(town.resource[FOOD], 0);
			UDData();

			if (turn > 100) {
				end = true;
				int ps[TOWNS] = {};
				for (int i = 0; i < S_AREA; i++) {
					endD.spA[i] = false;
				}
				for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
					if (tile[i].town != WILD) {
						ps[tile[i].town - 1] = tile[i].townLv + 1;
					}
					else if (tile[i].saNum != 0) {
						endD.spA[tile[i].saNum - 1] = true;
						endD.spLv[tile[i].saNum - 1] = tile[i].saLv + 1;
					}
				}
				for (int i = 0; i < TOWNS; i++) {
					endD.townPop[i] = ps[i];
				}
				for (int i = 0; i < RESOURCES; i++) {
					endD.resPro[i] = town.income[i];
				}
				endD.resPro[MONEY] += town.trade;
				endD.resPro[FOOD] -= town.foodCon;
				for (int i = 0; i < GOODS; i++) {
					endD.goods[i] = town.goodsPro[i] - town.goodsCon[i];
				}
				endD.tIncome = town.trade;

				town.Set();
				for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
					tile[i].Set();
				}
				none.first = 0;
				none.second = 0;
				stv.resize(0, none);
				for (int i = 0; i < RESOURCES; i++) {
					town.resource[i] = 100;
				}
				town.resource[FOOD] = 0;
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
			music->AddSound(sBuy);
		}

		break;

	case PST:

		pbox->DrawPB();

		break;

	case S_AREA:

		sbox->DrawSB();

		break;

	default:
		break;
	}

	g_hBut.Draw(75, 470);
	if (Event.LMouse.GetClick(74, 469, 225, 520)) {
		howTo = true;
	}

	g_back.Draw(75, 530);
	if (Event.LMouse.GetClick(74, 529, 225, 580)) {
		back = true;
		town.Set();
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++) {
			tile[i].Set();
		}
		none.first = 0;
		none.second = 0;
		stv.resize(0, none);
		for (int i = 0; i < RESOURCES; i++) {
			town.resource[i] = 100;
		}
		town.resource[FOOD] = 0;
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
		data->Set(i * 100 + 4, tile[i].hidMin);
		data->Set(i * 100 + 5, tile[i].saLv);
		for (int j = 0; j < ITEMS; j++) {
			if (tile[i].itemUse[j + 1]) {
				data->Set(i * 100 + 6 + j, 1);
			}
			else {
				data->Set(i * 100 + 6 + j, 0);
			}
		}
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
	data->Set(10000 + RESOURCES, turn);
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
		tile[i].hidMin = data->GetInt(i * 100 + 4);
		tile[i].saLv = data->GetInt(i * 100 + 5);
		for (int j = 0; j < ITEMS; j++) {
			if (data->GetInt(i * 100 + 6 + j) == 0) {
				tile[i].itemUse[j + 1] = false;
			}
			else {
				tile[i].itemUse[j + 1] = true;
			}
		}
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
	turn = data->GetInt(10000 + RESOURCES);
}

void CTileManager::Load() {
	ReadData();
	loaded = true;
}