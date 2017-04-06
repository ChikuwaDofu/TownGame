#include"TileM.h"

CPicture CTileManager::g_tile;
CPicture CTileManager::g_town;
CPicture CTileManager::g_frame;

void CTileManager::Set(){
	for(int y=0; y<BLOCKS_Y; y++){
		for(int x=0; x<BLOCKS_X; x++){
			tile[x + y*BLOCKS_X].x = x*GRID + 300;
			tile[x + y*BLOCKS_X].y = y*GRID;
			tile[x + y*BLOCKS_X].terrain = PLAIN;
			tile[x + y*BLOCKS_X].town = WILD;
			tile[x + y*BLOCKS_X].townLv = 0;
		}
	}

	town.money = 100;

	openInfo = false;
	boxStatus = NO;
	infoNum = -1;

	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
	g_town.Load("Chikuwa3/Towns.png", TOWNS, TLVS, GRID, GRID, TOWNS * TLVS);
	g_frame.Load("Chikuwa3/Frame.png");

	f_tData = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 0; i < TOWNS; i++){
		for (int j = 0; j < TLVS; j++){
			fscanf(f_tData, "%d %d", &tData.income_m[i][j], &tData.cost_m[i][j]);
		}
	}
	fclose(f_tData);
}

void CTileManager::OpenInfo(){
	infoNum = -1;
	bool info = false;

	if (Event.LMouse.GetClick(299, -1, 900, 600)){
		info = true;
		infoNum = (Event.RMouse.GetX() - 300) / GRID + (Event.RMouse.GetY()) / GRID * BLOCKS_X;
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
		if (fbox->close){
			delete fbox;
			boxStatus = NO;
		}
		else if (fbox->select){
			tile[infoNum].town = fbox->town;
			town.money -= tData.cost_m[tile[infoNum].town][tile[infoNum].townLv];
			delete fbox;
			boxStatus = NO;
		}

		break;

	case TOWN:
		if (tbox->close){
			delete tbox;
			boxStatus = NO;
		}
		else if(tbox->remove){
			tile[infoNum].town = WILD;
			tile[infoNum].townLv = 0;
			delete tbox;
			boxStatus = NO;
		}
		else if (tbox->develop){
			tile[infoNum].townLv = tbox->devLv;
			town.money -= tData.cost_m[tile[infoNum].town][tile[infoNum].townLv];
			delete tbox;
			boxStatus = NO;
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

	DrawFormatString(100, 50, YELLOW, "%d", town.money);

	DrawString(50, 150, "Enter‚ÅŽû“ü‚ðŠl“¾", WHITE);

	switch (boxStatus){
	case NO:
		for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
			g_tile.Draw(i % BLOCKS_X * GRID + 300, i / BLOCKS_X * GRID, tile[i].terrain);

			if (tile[i].town != WILD){
				g_town.Draw(i % BLOCKS_X * GRID + 300, i / BLOCKS_X * GRID, tile[i].town + tile[i].townLv * TOWNS);
			}
		}

		if (Event.RMouse.GetOn(299, -1, 900, 600)){
			g_frame.Draw(Event.RMouse.GetX() - (Event.RMouse.GetX()) % GRID, Event.RMouse.GetY() - (Event.RMouse.GetY()) % GRID);
		}

		if (Event.key.GetDown(Event.key.RETURN)){
			for (int i = 0; i < BLOCKS_X*BLOCKS_Y; i++){
				if (tile[i].town != WILD){
					town.money += tData.income_m[tile[i].town][tile[i].townLv];
				}
			}
		}

		break;

	case FOUND:

		fbox->DrawFB(town.money);

		break;

	case TOWN :

		tbox->DrawTB(tile[infoNum].town, tile[infoNum].townLv, town.money);

	default:
		break;
	}
}