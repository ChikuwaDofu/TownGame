#include"TileM.h"
#include<queue>
#include<vector>
#include<list>

//1.search all adjacent road tiles with <queue>
//2.change all adjacent road tiles' connect[a.a.r.t.] to true with <vector>

using namespace std;

//queue<int> q;
int now, nxt;
int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
//vector<int> v;
vector<int> vec, res;

CPicture CTileManager::g_tile;
CPicture CTileManager::g_town;
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

STownData::STownData(){
	FILE* f;

	f = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 0; i < TOWNS; i++){
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
	for (int i = 0; i < TOWNS; i++){
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

STile::STile(){
	x = x*GRID + WINDOW_WIDTH - WINDOW_HEIGHT;
	y = y*GRID;
	terrain = PLAIN;
	mineral = ROCK;
	town = WILD;
	townLv = 0;
	devLim = 0;
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
		for (int i = 0; i < RESOURCES; i++) {
			produce[i] = 0;
		}
		/*for (int i = 0; i < TRADE; i++) {
			trade[i] = 0;
		}*/
		for (int i = 0; i < GOODS; i++) {
			goods[i] = 0;
		}
	}
	else {
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

		goods[tData.goods[town][0]]++;
		goods[tData.goods[town][1]] += townLv;

		for (int j = 0; j < BUILDINGS; j++) {
			if (built[j]) {
				goods[bData.goods[town][j]]++;
			}
		}

		for (int i = 0; i < GOODS; i++){
			goods[i] *= 1 + buf[RESOURCES];
		}
	}
}

STown::STown(){
	Set();

	for (int i = 0; i < RESOURCES; i++){
		resource[i] = 100;
	}
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
}

void CTileManager::Set(){
	for (int i = 0; i < RESOURCES; i++){
		town.resource[i] = 100;
	}

	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++) {
		tflag[i] = false;
	}

	int x = 0, y = 0, cnt = 5, r = 0, bf = 1, d[2] = {}, s[2] = {}; //direction, start
	switch (GetRand(1)) {
	case 0:

		switch (GetRand(1)) {
		case 0:
			x = GetRand(BLOCKS_X - 3) + 1;

			tile[x].terrain = RIVER;

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
			}

			break;

		case 1:
			y = GetRand(BLOCKS_Y - 3) + 1;

			tile[y * BLOCKS_X].terrain = RIVER;

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
					}
				}
				else {
					tile[s[UD_END] + BLOCKS_X * (BLOCKS_Y - 1)].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X].terrain = RIVER;
					tile[s[LR_END] * BLOCKS_X + 1].terrain = RIVER;
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
					}
				}
			}
			else {
				x = BLOCKS_X - 2;
				if (d[UD_END] == 0) {
					tile[s[UD_END]].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 1].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 2].terrain = RIVER;
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
					}
				}
				else {
					tile[s[UD_END] + BLOCKS_X * (BLOCKS_Y - 1)].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 1].terrain = RIVER;
					tile[(s[LR_END] + 1) * BLOCKS_X - 2].terrain = RIVER;
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
	while (c < 10){
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
		tile[n].terrain = HILL;
		c++;
		if (n%BLOCKS_X < BLOCKS_X - 1) {
			if ((tile[n + 1].terrain == PLAIN || tile[n + 1].terrain == FOREST) && !tflag[n + 1]) {
				vec.push_back(n + 1);
				tflag[n + 1] = true;
			}
		}
		if (n%BLOCKS_X > 0) {
			if ((tile[n - 1].terrain == PLAIN || tile[n - 1].terrain == FOREST) && !tflag[n - 1]) {
				vec.push_back(n - 1);
				tflag[n - 1] = true;
			}
		}
		if (n / BLOCKS_X > 0) {
			if ((tile[n - BLOCKS_X].terrain == PLAIN || tile[n - BLOCKS_X].terrain == FOREST) && !tflag[n - BLOCKS_X]) {
				vec.push_back(n - BLOCKS_X);
				tflag[n - BLOCKS_X] = true;
			}
		}
		if (n / BLOCKS_X < BLOCKS_Y - 1) {
			if ((tile[n + BLOCKS_X].terrain == PLAIN || tile[n + BLOCKS_X].terrain == FOREST) && !tflag[n + BLOCKS_X]) {
				vec.push_back(n + BLOCKS_X);
				tflag[n + BLOCKS_X] = true;
			}
		}
	}

	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].mineral = GOLD;
	res.erase(res.begin() + n);
	n = GetRand(res.size() - 1);
	tile[res.operator[](n)].mineral = IRON;
	res.erase(res.begin() + n);
	tile[res.operator[](GetRand(res.size() - 1))].mineral = IRON;

	openInfo = false;
	boxStatus = NO;
	infoNum = -1;
	showTrade = false;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS, GRID, GRID, TOWNS);
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
			rbox = new SRiverBox(tile[infoNum]);
			boxStatus = RBUILD;
		}
		else {
			if (tile[infoNum].town == -1) {
				fbox = new SFoundBox(tile[infoNum].terrain, town);
				boxStatus = FOUND;
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

	switch (boxStatus){
	case FOUND:

		switch (fbox->mode){
		case CLOSE:
			delete fbox;
			boxStatus = NO;
			break;

		case EST:
			tile[infoNum].town = fbox->town;
			tile[infoNum].devLim = 5;
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
			tile[infoNum].devLim = 0;
			/*for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = 0;
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = 0;
			}*/
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
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
			switch (tbox->sBuildNum){
			case 0:
				tile[infoNum].devLim += 5;
				break;

			default:
				break;
			}
			delete tbox;
			boxStatus = NO;
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
		for (int j = 0; j < RESOURCES + TRADE; j++) {
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
						tile[i - BLOCKS_X].buf[RESOURCES + VALUE] = 0.1;
					}
					if (tile[i].built[2] && tile[i - BLOCKS_X].town == FARM) {
						tile[i - BLOCKS_X].buf[RESOURCES + POWER] = 0.2;
					}
				}
				if (i / BLOCKS_X < BLOCKS_Y - 1) {
					if (tile[i].built[0] && tile[i + BLOCKS_X].town == FARM) {
						tile[i + BLOCKS_X].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i + BLOCKS_X].town != WILD && tile[i + BLOCKS_X].town != COMM) {
						tile[i + BLOCKS_X].buf[RESOURCES + VALUE] = 0.1;
					}
					if (tile[i].built[2] && tile[i + BLOCKS_X].town == FARM) {
						tile[i + BLOCKS_X].buf[RESOURCES + POWER] = 0.2;
					}
				}
				if (i % BLOCKS_X > 0) {
					if (tile[i].built[0] && tile[i - 1].town == FARM) {
						tile[i - 1].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i - 1].town != WILD && tile[i - 1].town != COMM) {
						tile[i - 1].buf[RESOURCES + VALUE] = 0.1;
					}
					if (tile[i].built[2] && tile[i - 1].town == FARM) {
						tile[i - 1].buf[RESOURCES + POWER] = 0.2;
					}
				}
				if (i % BLOCKS_X < BLOCKS_X - 1) {
					if (tile[i].built[0] && tile[i + 1].town == FARM) {
						tile[i + 1].buf[FOOD] = 0.2;
					}
					if (tile[i].built[1] && tile[i + 1].town != WILD && tile[i + 1].town != COMM) {
						tile[i + 1].buf[RESOURCES + VALUE] = 0.1;
					}
					if (tile[i].built[2] && tile[i + 1].town == FARM) {
						tile[i + 1].buf[RESOURCES + POWER] = 0.2;
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

	town.Set();

	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if (tile[i].town != WILD){
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

	for (int i = 2; i < GOODS; i++) {
		if (town.goodsPro[i] >= town.goodsCon[i]) {
			town.exSum += (town.goodsPro[i] - town.goodsCon[i]) * gData.value[i];
		}
		else {
			town.inSum += (town.goodsCon[i] - town.goodsPro[i]) * gData.value[i];
		}
	}

	//town.income[MONEY] += town.trade[VALUE] * double (town.trade[POWER] / 5);
	bool flag = false;
	if (showTrade){
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

		DrawFormatString(75, 450, GREEN, "óAèoäzÅF%.2lf", town.exSum);
		DrawFormatString(75, 470, RED, "óAì¸äzÅF%.2lf", town.inSum);

		g_rBut.Draw(75, 520);
		if (Event.LMouse.GetClick(74, 519, 183, 670)){
			showTrade = false;
			flag = true;
		}
	}
	else {
		for (int i = 0; i < RESOURCES; i++){
			g_resource.Draw(75, 100 + I_SIZE * i, i);
		}

		for (int i = 0; i < STATS; i++){
			g_stats.Draw(75, 300 + i * I_SIZE, i); 
		}

		DrawFormatString(75 + I_SIZE + 5, 102, YELLOW, "%d (+%d)", town.resource[MONEY], town.income[MONEY]/* + town.trade[VALUE] * town.trade[POWER]*/);
		if (town.income[FOOD] - town.devSum >= 0){
			DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE, GREEN, "%d (+%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
		}
		else {
			DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE, GREEN, "%d (%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
		}
		DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE * 2, ORANGE, "%d (+%d)", town.resource[WOOD], town.income[WOOD]);

		DrawFormatString(75 + I_SIZE + 5, 102 + I_SIZE * 3, GRAY, "%d (+%d)", town.resource[STONE], town.income[STONE]);

		//DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "%d", town.trade[VALUE]);

		//DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, PURPLE, "%d", town.trade[POWER]);

		//for (int i = 1; i < GOODS; i++){
		//	DrawFormatString(75 + I_SIZE * ((i - 1) % 8) + 5, 302 + I_SIZE * ((i - 1) / 8), WHITE, "%d", town.goodsPro[i] - town.goodsCon[i]);
		//} 

		DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "%d", town.devSum);

		DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, WHITE, "%d/%d", town.towns, town.townMax);

		g_tBut.Draw(75, 520);
		if (Event.LMouse.GetClick(74, 519, 183, 670) && !flag){
			showTrade = true;
		}
	}

	DrawString(50, 10, "EnterÇ≈é˚ì¸Çälìæ", WHITE);

	ETown u = WILD, d = WILD, l = WILD, r = WILD;

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			if (tile[i].terrain != RIVER && tile[i].terrain != HILL) {
				g_tile.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);
			}
			else if (tile[i].terrain == RIVER){
				DrawRiver(i);
			}
			else {
				g_hill.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].mineral);
			}

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].town);
			}

			/////
			/*if (tile[i].fac[0]){
				DrawCircle(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT + 30, i / BLOCKS_X * GRID + 30, 10, RED);
			}*/
			/////
		}

		if (Event.RMouse.GetOn(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
			g_frame.Draw(Event.RMouse.GetX() - (Event.RMouse.GetX()) % GRID, Event.RMouse.GetY() - (Event.RMouse.GetY()) % GRID);
		}

		if (Event.key.GetDown(Event.key.RETURN)){
			for (int i = 0; i < RESOURCES; i++){
				town.resource[i] += town.income[i];
			}
			//town.resource[MONEY] += town.trade[VALUE] * town.trade[POWER];
			town.resource[FOOD] -= town.devSum;
		}

		break;

	case FOUND:

		fbox->DrawFB(town.resource[MONEY], tile[infoNum].mineral);

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

	default:
		break;
	}
}