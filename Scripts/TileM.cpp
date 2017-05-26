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
	for (int i = 0; i < RESOURCES; i++){
		town.resource[i] = 100;
	}

	tile[0].terrain = FOREST;
	openInfo = false;
	boxStatus = NO;
	infoNum = -1;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
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
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = tData.income[fbox->town][0][i];
				town.resource[i] -= tData.cost[fbox->town][0][i];
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
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
			delete tbox;
			boxStatus = NO;
			break;

		case DEV:
			tile[infoNum].townLv = tbox->devLv;
			for (int i = 0; i < RESOURCES; i++){
				tile[infoNum].produce[i] = tData.income[tile[infoNum].town][tbox->devLv][i];
				town.resource[i] -= tData.cost[tile[infoNum].town][tile[infoNum].townLv][i];
			}
			for (int i = 0; i < RESOURCES; i++){
				for (int j = 0; j < BUILDINGS; j++){
					if (tile[infoNum].built[j]){
						tile[infoNum].produce[i] += bData.income[tile[infoNum].town][j][i];
					}
				}
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
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(75, 50 + I_SIZE * i, i);
	}
	DrawFormatString(75 + I_SIZE + 5, 50, YELLOW, "%d (+%d)", town.resource[MONEY], income[MONEY]);
	DrawFormatString(75 + I_SIZE + 5, 50 + I_SIZE, GREEN, "%d (+%d)", town.resource[FOOD], income[FOOD]);
	DrawFormatString(75 + I_SIZE + 5, 50 + I_SIZE * 2, ORANGE, "%d (+%d)", town.resource[WOOD], income[WOOD]);

	DrawString(50, 150, "Enter‚ÅŽû“ü‚ðŠl“¾", WHITE);

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			g_tile.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].terrain);

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + WINDOW_WIDTH - WINDOW_HEIGHT, i / BLOCKS_X * GRID, tile[i].townLv + tile[i].town * TLVS);
			}
		}

		if (Event.RMouse.GetOn(WINDOW_WIDTH - WINDOW_HEIGHT - 1, -1, WINDOW_WIDTH, WINDOW_HEIGHT)){
			g_frame.Draw(Event.RMouse.GetX() - (Event.RMouse.GetX()) % GRID, Event.RMouse.GetY() - (Event.RMouse.GetY()) % GRID);
		}

		if (Event.key.GetDown(Event.key.RETURN)){
			for (int i = 0; i < RESOURCES; i++){
				town.resource[i] += income[i];
			}
		}

		break;

	case FOUND:

		fbox->DrawFB(town.resource[MONEY]);

		break;

	case TOWN :

		tbox->DrawTB();

	default:
		break;
	}
}