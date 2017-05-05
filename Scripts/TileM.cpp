#include"TileM.h"

CPicture CTileManager::g_tile;
CPicture CTileManager::g_town;
CPicture CTileManager::g_frame;
CPicture CTileManager::g_resource;
CPicture CTileManager::g_num;

STownData::STownData(){
	FILE* f;

	f = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 0; i < TOWNS; i++){
		for (int j = 0; j < TLVS; j++){
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &income[i][j][k]);
			}
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &cost[i][j][k]);
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
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &income[i][j][k]);
			}
			for (int k = 0; k < RESOURCES; k++){
				fscanf(f, "%d", &cost[i][j][k]);
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
	for (int i = 0; i < RESOURCES; i++){
		produce[i] = 0;
	}
}

void CTileManager::Set(){
	town.money = 100;
	town.food = 100;

	openInfo = false;
	boxStatus = NO;
	infoNum = -1;

	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
	g_town.Load("Chikuwa3/Towns.png", TLVS, TOWNS, GRID, GRID, TLVS*TOWNS);
	g_frame.Load("Chikuwa3/Frame.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	g_num.Load("Chikuwa3/Numbers.png", 10, 1, 12, 20, 10);
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

		if (tile[infoNum].town == -1){
			fbox = new SFoundBox(tile[infoNum].terrain);
			boxStatus = FOUND;
		}
		else{
			tbox = new STownBox(tile[infoNum].terrain);
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
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = tData.income[fbox->town][0][i];
			}
			town.money -= tData.cost[fbox->town][0][MONEY];
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
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
			delete tbox;
			boxStatus = NO;
			break;

		case DEV:
			tile[infoNum].townLv = tbox->devLv;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = tData.income[tile[infoNum].town][tbox->devLv][i];
			}
			for (int i = 0; i < RESOURCES; i++){
				for (int j = 0; j < BUILDINGS; j++){
					if (tile[infoNum].built[j]){
						tile[infoNum].produce[i] += bData.income[tile[infoNum].town][j][i];
					}
				}
			}
			town.money -= tData.cost[tile[infoNum].town][tile[infoNum].townLv][MONEY];
			delete tbox;
			boxStatus = NO;
			break;

		case BUILD:
			tile[infoNum].built[tbox->buildNum] = true;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] += bData.income[tile[infoNum].town][tbox->buildNum][i];
			}
			town.money -= bData.cost[tile[infoNum].town][tbox->buildNum][MONEY];
			delete tbox;
			boxStatus = NO;
			break;

		case DEMO:
			tile[infoNum].built[tbox->buildNum] = false;
			delete tbox;
			boxStatus = NO;
			break;
		}

		break;

	default:
		break;
	}
}

void CTileManager::Draw(){
	if (boxStatus == NO){
		OpenInfo();
	}
	else{
		CloseInfo();
	}

	int income[RESOURCES] = {};
	for (int i = 0; i < BLOCKS_X * BLOCKS_Y; i++){
		if(tile[i].town != WILD){
			for (int j = 0; j < RESOURCES; j++){
				income[j] += tile[i].produce[j];
			}
		}
	}
	g_resource.Draw(75, 50, MONEY);
	DrawFormatString(100, 50, YELLOW, "%d (+%d)", town.money, income[MONEY]);
	g_resource.Draw(75, 70, FOOD);
	DrawFormatString(100, 70, GREEN, "%d (+%d)", town.food, income[FOOD]);

	DrawString(50, 150, "Enter‚ÅŽû“ü‚ðŠl“¾", WHITE);

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			g_tile.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].townLv + tile[i].town*TOWNS);
			}
		}

		if (Event.RMouse.GetOn(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
			g_frame.Draw(Event.RMouse.GetX() - (Event.RMouse.GetX()) % GRID, Event.RMouse.GetY() - (Event.RMouse.GetY()) % GRID);
		}

		if (Event.key.GetDown(Event.key.RETURN)){
			town.money += income[MONEY];
			town.food += income[FOOD];
		}

		break;

	case FOUND:

		fbox->DrawFB(town.money);

		break;

	case TOWN :

		tbox->DrawTB(tile[infoNum], town.money);

	default:
		break;
	}
}