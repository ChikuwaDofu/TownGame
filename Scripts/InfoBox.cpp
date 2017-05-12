#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
CPicture SInfoBox::g_box;
CPicture SInfoBox::g_resource;
CPicture SInfoBox::g_building;
CPicture SInfoBox::g_shade;
CPicture SInfoBox::g_build;
CPicture SInfoBox::g_demolish;

SInfoBox::SInfoBox(){
	mode = KEEP;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", TLVS, TOWNS, GRID, GRID, TLVS*TOWNS);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, 1, 40, 40, BUILDINGS);
	g_box.Load("Chikuwa3/Box.png");
	g_shade.Load("Chikuwa3/Shade.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
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
		if (townInfo.resource[i] < tData.cost[type][0][i]){
			return false;
		}
	}

	return true;
}

void SFoundBox::PutButton(int x, int y, ETown type, int money){
	g_town.Draw(540, 270, type);

	DrawData(540 + GRID + 5, 270, type);

	if (CheckEnough(type)){
		DrawFormatString(x + 10, y + GRID + 10, BLACK, "%d", tData.cost[type][0][MONEY]);

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			town = type;
			mode = EST;
		}
	}
	else{
		DrawFormatString(x + 10, y + GRID + 10, RED, "%d", tData.cost[type][0][MONEY]);
	}
}

void SFoundBox::DrawData(int x, int y, ETown type){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * (I_SIZE + 5), i);
		DrawFormatString(x + I_SIZE + 5, y + i * 25, BLACK, "%d", tData.income[type][0][i]);
	}
}

void SFoundBox::DrawFB(int money){
	DrawIB();

	switch (terrain){
	case 0:
		PutButton(540, 270, FARM, money);
		break;

	default:
		break;
	}

	open = false;
}

/////

STownBox::STownBox(STown town, STile tile){
	devLv = 0;
	townInfo = town;
	tileInfo = tile;
}

void STownBox::PutRemoveButton(int x, int y){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = REMV;
	}
}

void STownBox::DrawDev(int x, int y, int lv){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * (I_SIZE + 5), i);
		DrawFormatString(x + I_SIZE + 5, y + i * (I_SIZE + 5), BLACK, "+%d", tData.income[tileInfo.town][lv][i] - tData.income[tileInfo.town][0][i]);
	}
}

bool STownBox::CheckDEnough(int lv){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[tileInfo.town][lv][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutDevButton(int x, int y, int lv){
	if (CheckDEnough(lv)){
		DrawFormatString(x + 10, y + GRID + 10, BLACK, "%d", tData.cost[tileInfo.town][lv][MONEY]);

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = DEV;
			devLv = lv;
		}
	}
	else{
		DrawFormatString(x + 10, y + GRID + 10, RED, "%d", tData.cost[tileInfo.town][lv][MONEY]);
	}
}

void STownBox::DrawBuildings(int x, int y, int build){
	g_building.Draw(x, y, build);

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x + B_SIZE + 5, y - 5 + i * I_SIZE, i);
		DrawFormatString(x + B_SIZE + I_SIZE + 10, y - 5 + i * I_SIZE, BLACK, "%d", bData.income[tileInfo.town][build][i]);
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

void STownBox::PutBuildButton(int x, int y, int bNum){
	g_shade.Draw(x, y);

	if (CheckBEnough(bNum)){
		if (Event.RMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
			g_build.Draw(x, y);
		}

		if (Event.LMouse.GetClick(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
			mode = BUILD;
			buildNum = bNum;
		}

		DrawFormatString(x, y + 45, BLACK, "%d", bData.cost[tileInfo.town][bNum][MONEY]);
	}
	else{
		DrawFormatString(x, y + 45, RED, "%d", bData.cost[tileInfo.town][bNum][MONEY]);
	}
}

void STownBox::PutDemolishButton(int x, int y, int bNum){
	if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
		g_demolish.Draw(x, y);
	}

	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = DEMO;
		buildNum = bNum;
	}
}

void STownBox::DrawTB(){
	DrawIB();

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(425, 485 + i * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 485 + i * I_SIZE, BLACK, "%d", tileInfo.produce[i]);
	}

	DrawBuildings(430, 60, 0);

	if (tileInfo.built[0]){
		PutDemolishButton(430, 60, 0);
	}
	else{
		PutBuildButton(430, 60, 0);
	}

	switch (tileInfo.townLv){
	case 0:
		g_town.Draw(440, 270, 1 + tileInfo.town*TOWNS);
		PutDevButton(440, 270, 1);
		DrawDev(440 + GRID + 5, 270, 1);

		g_town.Draw(640, 270, 2 + tileInfo.town*TOWNS);
		PutDevButton(640, 270, 2);
		DrawDev(640 + GRID + 5, 270, 2);
		break;

	case 1:
		DrawBuildings(730, 60, 1);

		if (tileInfo.built[1]){
			PutDemolishButton(730, 60, 1);
		}
		else{
			PutBuildButton(730, 60, 1);
		}
		break;

	case 2:
		DrawBuildings(730, 60, 2);

		if (tileInfo.built[2]){
			PutDemolishButton(730, 60, 2);
		}
		else{
			PutBuildButton(730, 60, 2);
		}
		break;

	default:
		break;
	}

	g_tile.Draw(720, 470, tileInfo.terrain);

	if (!open){
		PutRemoveButton(720, 470);
	}

	open = false;
}
