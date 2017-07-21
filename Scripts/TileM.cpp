#include"TileM.h"
#include<queue>
#include<vector>

//1.search all adjacent road tiles with <queue>
//2.change all adjacent road tiles' connect[a.a.r.t.] to true with <vector>

using namespace std;

queue<int> q;
int now, nxt;
int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
vector<int> v;

CPicture CTileManager::g_tile;
CPicture CTileManager::g_town;
CPicture CTileManager::g_frame;
CPicture CTileManager::g_resource;
CPicture CTileManager::g_trade;
CPicture CTileManager::g_stats;
CPicture CTileManager::g_num;

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
			for (int k = 0; k < TRADE; k++){
				fscanf(f, "%d", &trade[i][j][k]);
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
			for (int k = 0; k < TRADE; k++){
				fscanf(f, "%d", &trade[i][j][k]);
			}
		}
	}
	fclose(f);
}

STile::STile(){
	x = x*GRID + WINDOW_WIDTH - WINDOW_HEIGHT;
	y = y*GRID;
	terrain = PLAIN;
	town = WILD;
	townLv = 0;
	for (int i = 0; i < BUILDINGS; i++){
		built[i] = false;
	}
	fac[0] = false;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		connect[i] = false;
	}
	for (int i = 0; i < RESOURCES; i++){
		produce[i] = 0;
	}
	for (int i = 0; i < TRADE; i++){
		trade[i] = 0;
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
	for (int i = 0; i < TRADE; i++){
		trade[i] = 0;
	}

	devSum = 0;
}

void CTileManager::Set(){
	for (int i = 0; i < RESOURCES; i++){
		town.resource[i] = 100;
	}

	int c = 0, n = 0;
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		tile[i].connect[i] = true;
		forest[i] = false;
	}
	while (c < 10){
		n = GetRand(BLOCKS_X * BLOCKS_Y - 1);

		if (!forest[n]){
			forest[n] = true;
			tile[n].terrain = FOREST;
			c++;
		}
	}

	//tile[0].terrain = FOREST;
	openInfo = false;
	boxStatus = NO;
	infoNum = -1;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS, GRID, GRID, TOWNS);
	g_frame.Load("Chikuwa3/Frame.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, I_SIZE, I_SIZE, RESOURCES);
	g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, I_SIZE, I_SIZE, TRADE);
	g_stats.Load("Chikuwa3/SIcons.png", STATS, 1, I_SIZE, I_SIZE, STATS);
	g_num.Load("Chikuwa3/Numbers.png", 10, 1, 12, 20, 10);
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

		if (tile[infoNum].town == -1){
			fbox = new SFoundBox(tile[infoNum].terrain, town);
			boxStatus = FOUND;
		}
		else{
			tbox = new STownBox(town, tile[infoNum]);
			boxStatus = TOWN;
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
			tile[infoNum].fac[0] = true;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = tData.income[fbox->town][NEW][i];
				town.resource[i] -= tData.cost[fbox->town][NEW][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = tData.trade[fbox->town][NEW][i];
			}
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
			for (int i = 0; i < BUILDINGS; i++){
				if (tile[infoNum].built[i]){
					tile[infoNum].built[i] = false;
				}
			}
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = 0;
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] = 0;
			}
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
			delete tbox;
			boxStatus = NO;
			break;

		case DEV:
			//tile[infoNum].townLv = tbox->devLv;
			tile[infoNum].townLv++;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] += tData.income[tile[infoNum].town][LVUP][i];
				town.resource[i] -= tData.cost[tile[infoNum].town][LVUP][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += tData.trade[tile[infoNum].town][LVUP][i];
			}
			delete tbox;
			boxStatus = NO;
			break;
			
		case DG:
			//tile[infoNum].townLv = tbox->devLv;
			tile[infoNum].townLv--;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] += tData.income[tile[infoNum].town][LVUP][i];
				town.resource[i] -= tData.cost[tile[infoNum].town][LVUP][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += tData.trade[tile[infoNum].town][LVUP][i];
			}
			delete tbox;
			boxStatus = NO;
			break;

		case BUILD:
			tile[infoNum].built[tbox->buildNum] = true;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] += bData.income[tile[infoNum].town][tbox->buildNum][i];
				town.resource[i] -= bData.cost[tile[infoNum].town][tbox->buildNum][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] += bData.trade[tile[infoNum].town][tbox->buildNum][i];
			}
			delete tbox;
			boxStatus = NO;
			break;

		case DEMO:
			tile[infoNum].built[tbox->buildNum] = false;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] -= bData.income[tile[infoNum].town][tbox->buildNum][i];
			}
			for (int i = 0; i < TRADE; i++){
				tile[infoNum].trade[i] -= bData.trade[tile[infoNum].town][tbox->buildNum][i];
			}
			delete tbox;
			boxStatus = NO;
			break;
		}

		break;

	default:
		break;
	}
}

void CTileManager::CheckConnect(int n){
	/*for (int i = 0; i < 4; i++){
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
	}*/
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
}

void CTileManager::Draw(){
	if (boxStatus == NO){
		OpenInfo();
	}
	else{
		CloseInfo();
	}

	town.Set();

	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if (tile[i].town != WILD){
			town.devSum += tile[i].townLv + 1;
		}
	}

	for (int i = 0; i < RESOURCES; i++){
		for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
			town.income[i] += tile[j].produce[i];
		}

		g_resource.Draw(75, 50 + I_SIZE * i, i);
	}

	town.trade[POWER] = 1;
	for (int i = 0; i < TRADE; i++){
		for (int j = 0; j < BLOCKS_X * BLOCKS_Y; j++){
			town.trade[i] += tile[j].trade[i];
		}

		g_trade.Draw(75, 300 + I_SIZE * i, i);
	}

	g_stats.Draw(75, 450, 0);

	DrawFormatString(75 + I_SIZE + 5, 52, YELLOW, "%d (+%d)", town.resource[MONEY], town.income[MONEY]/* + town.trade[VALUE] * town.trade[POWER]*/);
	if (town.income[FOOD] - town.devSum >= 0){
		DrawFormatString(75 + I_SIZE + 5, 52 + I_SIZE, GREEN, "%d (+%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
	}
	else {
		DrawFormatString(75 + I_SIZE + 5, 52 + I_SIZE, GREEN, "%d (%d)", town.resource[FOOD], town.income[FOOD] - town.devSum);
	}
	DrawFormatString(75 + I_SIZE + 5, 52 + I_SIZE * 2, ORANGE, "%d (+%d)", town.resource[WOOD], town.income[WOOD]);

	DrawFormatString(75 + I_SIZE + 5, 302, WHITE, "%d", town.trade[VALUE]);

	DrawFormatString(75 + I_SIZE + 5, 302 + I_SIZE, PURPLE, "%d", town.trade[POWER]);

	DrawFormatString(75 + I_SIZE + 5, 452, WHITE, "%d", town.devSum);

	DrawString(50, 150, "Enter‚ÅŽû“ü‚ðŠl“¾", WHITE);

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			g_tile.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].town);
			}

			/////
			if (tile[i].fac[0]){
				DrawCircle(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT + 30, i / BLOCKS_X * GRID + 30, 10, RED);
			}
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

		fbox->DrawFB(town.resource[MONEY]);

		break;

	case TOWN:

		tbox->DrawTB();

	default:
		break;
	}
}