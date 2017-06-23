#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
CPicture SInfoBox::g_box;
CPicture SInfoBox::g_resource;
CPicture SInfoBox::g_trade;
CPicture SInfoBox::g_building;
CPicture SInfoBox::g_shade;
CPicture SInfoBox::g_build;
CPicture SInfoBox::g_demolish;

SInfoBox::SInfoBox(){
	mode = KEEP;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS, GRID, GRID, TOWNS);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, TOWNS, 40, 40, BUILDINGS * TOWNS);
	g_box.Load("Chikuwa3/Box.png");
	g_shade.Load("Chikuwa3/Shade.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, 20, 20, TRADE);
	g_build.Load("Chikuwa3/Build.png");
	g_demolish.Load("Chikuwa3/Demolish.png");
}

void SInfoBox::DrawIB(){
	g_box.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);

	if (Event.LMouse.GetClick(WINDOW_WIDTH - CROSS_L - 1, -1, WINDOW_WIDTH, CROSS_L) && !open){
		mode = CLOSE;
	}
}

/////

SFoundBox::SFoundBox(ETerrain type, STown town){
	terrain = type;
	townInfo = town;
}

bool SFoundBox::CheckEnough(ETown type){
	for(int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			return false;
		}
	}

	return true;
}

void SFoundBox::PutButton(int x, int y, ETown type, int money){
	g_town.Draw(540, 270, type);

	DrawData(540 + GRID + 5, 270, type);

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[type][NEW][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[type][NEW][i]);
		}
	}

	if (CheckEnough(type)){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			town = type;
			mode = EST;
		}
	}
}

void SFoundBox::DrawData(int x, int y, ETown type){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "%d", tData.income[type][NEW][i]);
	}
	for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
	}
}

void SFoundBox::DrawFB(int money){
	DrawIB();

	switch (terrain){
	case PLAIN:
		PutButton(540, 270, FARM, money);
		break;

	case FOREST:
		PutButton(540, 270, F_VIL, money);
		break;

	default:
		break;
	}

	open = false;
}

/////

CPicture STownBox::g_lvUp;

STownBox::STownBox(STown town, STile tile){
	devLv = 0;
	townInfo = town;
	tileInfo = tile;

	g_lvUp.Load("Chikuwa3/Dev.png");
}

void STownBox::PutRemoveButton(int x, int y){
	g_resource.Draw(x + GRID + 5, y, MONEY);
	g_resource.Draw(x + GRID + 5, y + I_SIZE + 5, WOOD);
	DrawFormatString(x + GRID + I_SIZE + 5, y + I_SIZE + 7, BLACK, "+%d", (tData.cost[tileInfo.town][NEW][WOOD] + tData.cost[tileInfo.town][LVUP][WOOD] * tileInfo.townLv) / 4);

	int cost = (tData.cost[tileInfo.town][NEW][MONEY] + tData.cost[tileInfo.town][LVUP][MONEY] * tileInfo.townLv) / 2;
	if (townInfo.resource[MONEY] >= cost){
		DrawFormatString(x + GRID + I_SIZE + 5, y + 2, BLACK, "-%d", cost);

		if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = REMV;
		}
	}
	else {
		DrawFormatString(x + GRID + I_SIZE + 5, y + 2, RED, "-%d", cost);
	}
}

void STownBox::DrawDev(int x, int y, int lv){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "+%d", tData.income[tileInfo.town][LVUP][i]);
	}
	for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "+%d", tData.trade[tileInfo.town][LVUP][i]);
	}
}

bool STownBox::CheckDEnough(int lv){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutDevButton(int x, int y, int lv){
	g_lvUp.Draw(x, y);

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
	}

	if (CheckDEnough(lv)){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + 50, y + 50)){
			mode = DEV;
			devLv = lv;
		}
	}
}

void STownBox::DrawBuildings(int x, int y, int build){
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 191;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 175, my + 130, LIGHTYELLOW, true);

			DrawString(mx + 5, my + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + I_SIZE + 5, i);
				if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]){
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE + 7, RED, "-%d", bData.cost[tileInfo.town][build][i]);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE + 7, BLACK, "-%d", bData.cost[tileInfo.town][build][i]);
				}
			}
		
			DrawString(mx + 5, my + 10 + I_SIZE * 2, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + 10 + I_SIZE * 3, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 3, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			for (int i = 0; i < TRADE; i++){
				g_trade.Draw(mx + 5 + i * 55, my + 15 + I_SIZE * 4, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 4, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 175, my + 130, LIGHTYELLOW, true);

			DrawString(lim + 5, my + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + I_SIZE + 5, i);
				if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]){
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE + 7, RED, "-%d", bData.cost[tileInfo.town][build][i]);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE + 7, BLACK, "-%d", bData.cost[tileInfo.town][build][i]);
				}
			}
		
			DrawString(lim + 5, my + 10 + I_SIZE * 2, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + 10 + I_SIZE * 3, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 3, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			for (int i = 0; i < TRADE; i++){
				g_trade.Draw(lim + 5 + i * 55, my + 15 + I_SIZE * 4, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 4, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}
		}
	}
}

bool STownBox::CheckBEnough(int bNum){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < bData.cost[tileInfo.town][bNum][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutBuildingButton(int x, int y, int bNum, bool isBuilt){
	if (isBuilt){
		if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
			g_demolish.Draw(x, y);
		}

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = DEMO;
			buildNum = bNum;
		}
	}
	else{
		g_shade.Draw(x, y);

		if (CheckBEnough(bNum)){
			if (Event.RMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				g_build.Draw(x, y);
			}

			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				mode = BUILD;
				buildNum = bNum;
			}
		}
	}

	
}

/*void STownBox::PutDemolishButton(int x, int y, int bNum){
	if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
		g_demolish.Draw(x, y);
	}

	if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = DEMO;
		buildNum = bNum;
	}
}*/

void STownBox::DrawTB(){
	DrawIB();

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(425, 465 + i * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 465 + i * I_SIZE + 2, BLACK, "%d", tileInfo.produce[i]);
	}
	for (int i = 0; i < TRADE; i++){
		g_trade.Draw(425, 465 + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 465 + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tileInfo.trade[i]);
	}

	for (int i = 0; i < BUILDINGS; i++){
		g_building.Draw(450 - B_SIZE / 2 + i * 150, 60, i + tileInfo.town * BUILDINGS);
	}

	PutBuildingButton(430, 60, 0, tileInfo.built[0]);
	PutBuildingButton(580, 60, 1, tileInfo.built[1]);
	PutBuildingButton(730, 60, 2, tileInfo.built[2]);
	
	DrawBuildings(430, 60, 0);
	DrawBuildings(580, 60, 1);
	DrawBuildings(730, 60, 2);

	DrawDev(440 + GRID + 5, 270, tileInfo.townLv + 1);
	PutDevButton(440, 270, tileInfo.townLv + 1);

	g_tile.Draw(720, 470, tileInfo.terrain);

	if (!open){
		PutRemoveButton(720, 470);
	}

	open = false;
}
