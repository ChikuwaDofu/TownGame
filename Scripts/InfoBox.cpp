#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
//CPicture SInfoBox::g_box;
CPicture SInfoBox::g_resource;
//CPicture SInfoBox::g_trade;
CPicture SInfoBox::g_goods;
CPicture SInfoBox::g_building;
CPicture SInfoBox::g_shadeS;
CPicture SInfoBox::g_shadeL;
CPicture SInfoBox::g_build;
CPicture SInfoBox::g_demolish;
CPicture SInfoBox::g_mineral;

SInfoBox::SInfoBox(){
	mode = KEEP;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS, GRID, GRID, TOWNS);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, TOWNS, 40, 40, BUILDINGS * TOWNS);
	//g_box.Load("Chikuwa3/Box.png");
	g_shadeS.Load("Chikuwa3/Shade40.png");
	g_shadeL.Load("Chikuwa3/Shade50.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	//g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, 20, 20, TRADE);
	g_goods.Load("Chikuwa3/Goods.png", GOODS, 1, G_SIZE, G_SIZE, GOODS);
	g_build.Load("Chikuwa3/Build.png");
	g_demolish.Load("Chikuwa3/Demolish.png");
	g_mineral.Load("Chikuwa3/Mineral.png", MNR_TYPE, 1, R_SIZE, R_SIZE, MNR_TYPE);
}

void SInfoBox::DrawIB(){

	if ((Event.LMouse.GetClick(WINDOW_WIDTH - CROSS_L - 1, -1, WINDOW_WIDTH, CROSS_L) || Event.RMouse.GetClick()) && !open){
		mode = CLOSE;
	}
}

/////

CPicture SFoundBox::g_boxF;

SFoundBox::SFoundBox(ETerrain type, STown town){
	terrain = type;
	townInfo = town;

	g_boxF.Load("Chikuwa3/Box_F.png");
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
	g_town.Draw(x, y, type);
	
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	DrawString(x + GRID + 5, y - 20, "産出", BLACK);
	DrawData(x + GRID + 5, y, type);

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
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
	}*/
	g_goods.Draw(x, y + RESOURCES * I_SIZE, tData.goods[type][0]);
	//DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
}

void SFoundBox::DrawFB(int money, EMineral mineral){
	g_boxF.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	if (terrain == HILL){
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, mineral);
		switch (mineral) {
		case GOLD:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 10, 450 + R_SIZE / 2 + 5, "金", BLACK);
			break;

		case IRON:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 - 5, 450 + R_SIZE / 2 + 5, "鉄鉱石", BLACK);
			break;

		default:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 3, 450 + R_SIZE / 2 + 5, "石材", BLACK);
			break;
		}
	}

	if (townInfo.towns < townInfo.townMax){
		switch (terrain){
		case PLAIN:
			PutButton(470, 120, FARM, money);
			PutButton(670, 120, COMM, money);
			break;

		case FOREST:
			PutButton(540, 120, F_VIL, money);
			break;

		case HILL:
			switch (mineral) {
			case GOLD:
				PutButton(540, 120, MINE_G, money);
				break;

			case IRON:
				PutButton(540, 120, MINE_I, money);
				break;

			default:
				PutButton(540, 120, MINE_S, money);
				break;
			}
			break;

		default:
			break;
		}
	}
	else {
		DrawString(500, 120, "これ以上都市を造れません！", BLACK);
	}
	open = false;
}

/////

CPicture STownBox::g_boxT;
CPicture STownBox::g_lvUp;
CPicture STownBox::g_lvDn;
CPicture STownBox::g_demoL[2];
CPicture STownBox::g_SB;

STownBox::STownBox(STown town, STile tile){
	//devLv = 0;
	townInfo = town;
	tileInfo = tile;
	clickDemo = false;

	g_boxT.Load("Chikuwa3/Box_T.png");
	g_lvUp.Load("Chikuwa3/Dev.png");
	g_lvDn.Load("Chikuwa3/DG.png");
	g_demoL[0].Load("Chikuwa3/Demo_L.png");
	g_demoL[1].Load("Chikuwa3/AskDL.png");
	g_SB.Load("Chikuwa3/SpBuildings.png", SP_BUILDS, 1, B_SIZE, B_SIZE, SP_BUILDS);
}

void STownBox::PutRemoveButton(int x, int y){
	DrawString(x + GRID + 15, y - 30, "コスト", BLACK);
	g_resource.Draw(x + GRID + 15, y + I_SIZE - 30, MONEY);

	int cost = (tData.cost[tileInfo.town][NEW][MONEY] + tData.cost[tileInfo.town][LVUP][MONEY] * tileInfo.townLv) / 2;
	if (townInfo.resource[MONEY] >= cost){
		DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE - 28, BLACK, "%d", cost);

		if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			if (clickDemo){
				mode = REMV;
			}
			else {
				clickDemo = true;
			}
		}

		if (!Event.RMouse.GetOn(x - 1, y - 1, x + GRID, y + GRID) && clickDemo){
			clickDemo = false;
		}
	}
	else {
		DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE - 28, RED, "%d", cost);
	}

	DrawString(x + GRID + 15, y + I_SIZE * 2 - 25, "回収", BLACK);
	g_resource.Draw(x + GRID + 15, y + I_SIZE * 3 - 25, WOOD);
	DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE * 3 - 21, BLACK, "%d", (tData.cost[tileInfo.town][NEW][WOOD] + tData.cost[tileInfo.town][LVUP][WOOD] * tileInfo.townLv) / 4);
	g_resource.Draw(x + GRID + 15, y + I_SIZE * 4 - 25, STONE);
	DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE * 4 - 21, BLACK, "%d", (tData.cost[tileInfo.town][NEW][STONE] + tData.cost[tileInfo.town][LVUP][STONE] * tileInfo.townLv) / 4);

}

void STownBox::DrawDev(int x, int y/*, int lv*/){
	DrawString(x, y - 20, "上昇値", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "+%d", tData.income[tileInfo.town][LVUP][i]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "+%d", tData.trade[tileInfo.town][LVUP][i]);
	}*/
}

void STownBox::DrawDG(int x, int y/*, int lv*/){
	DrawString(x, y - 20, "下降値", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "-%d", tData.income[tileInfo.town][LVUP][i]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "-%d", tData.trade[tileInfo.town][LVUP][i]);
	}*/
}

bool STownBox::CheckDEnough(){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutDevButton(int x, int y/*, int lv*/){
	g_lvUp.Draw(x, y);

	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "%d", tData.cost[tileInfo.town][LVUP][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "%d", tData.cost[tileInfo.town][LVUP][i]);
		}
	}

	if (CheckDEnough()){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + 50, y + 50)){
			mode = DEV;
			//devLv = lv;
		}
	}
}

void STownBox::PutDGButton(int x, int y/*, int lv*/){
	g_lvDn.Draw(x, y);

	DrawString(x - 65 - I_SIZE, y - 20, "コスト・回収", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
	}

	if (CheckDEnough()){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + 50, y + 50)){
			mode = DG;
			//devLv = lv;
		}
	}
}

void STownBox::DrawBuildings(int x, int y, int build){
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 221;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 205, my + 160, LIGHTYELLOW, true);

			DrawFormatString(mx + 5, my + 5, BLACK, "%s", bData.name[tileInfo.town][build]);
			DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]){
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][i]);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][i]);
				}
			}
		
			DrawString(mx + 5, my + 10 + I_SIZE * 3, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + 10 + I_SIZE * 4, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 4, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			/*for (int i = 0; i < TRADE; i++){
				g_trade.Draw(mx + 5 + i * 55, my + 15 + I_SIZE * 5, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 5, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}*/
			g_goods.Draw(mx + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
		}
		else {
			DrawBox(lim, my, lim + 205, my + 160, LIGHTYELLOW, true);
			
			DrawFormatString(lim + 5, my + 5, BLACK, "%s", bData.name[tileInfo.town][build]);
			DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]){
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][i]);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][i]);
				}
			}
		
			DrawString(lim + 5, my + 10 + I_SIZE * 3, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + 10 + I_SIZE * 4, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 4, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			/*for (int i = 0; i < TRADE; i++){
				g_trade.Draw(lim + 5 + i * 55, my + 15 + I_SIZE * 5, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 5, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}*/
			g_goods.Draw(lim + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
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
		/*if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
			g_demolish.Draw(x, y);
		}

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = DEMO;
			buildNum = bNum;
		}*/
	}
	else{
		g_shadeS.Draw(x, y);

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

void STownBox::DrawSBuildings(int x, int y, int build, bool popReq){
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 231;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 215, my + 160, LIGHTYELLOW, true);

			DrawFormatString(mx + 5, my + 5, BLACK, "%s", sbData.name[build]);
			DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < sbData.cost[build][i]){
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", sbData.cost[build][i]);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", sbData.cost[build][i]);
				}
			}
		
			if (popReq){
				DrawString(mx + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
				DrawFormatString(mx + 5, my + 10 + I_SIZE * 4, BLACK, "%s", sbData.req[build]);
			}else{
				DrawString(mx + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
				DrawFormatString(mx + 5, my + 10 + I_SIZE * 4, RED, "%s", sbData.req[build]);
			}

			DrawString(mx + 5, my + 15 + I_SIZE * 5, "効果", BLACK);
			DrawFormatString(mx + 5, my + 15 + I_SIZE * 6, BLACK, "%s", sbData.exp[build]);
		}
		else {
			DrawBox(lim, my, lim + 215, my + 160, LIGHTYELLOW, true);
			
			DrawFormatString(lim + 5, my + 5, BLACK, "%s", sbData.name[build]);
			DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < sbData.cost[build][i]){
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", sbData.cost[build][i]);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", sbData.cost[build][i]);
				}
			}
		
			if (popReq){
				DrawString(lim + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
				DrawFormatString(lim + 5, my + 10 + I_SIZE * 4, BLACK, "%s", sbData.req[build]);
			}else{
				DrawString(lim + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
				DrawFormatString(lim + 5, my + 10 + I_SIZE * 4, RED, "%s", sbData.req[build]);
			}

			DrawString(lim + 5, my + 15 + I_SIZE * 5, "効果", BLACK);
			DrawFormatString(lim + 5, my + 15 + I_SIZE * 6, BLACK, "%s", sbData.exp[build]);
		}
	}
}

bool STownBox::CheckSBEnough(int bNum){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < sbData.cost[bNum][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutSpBuildButton(int x, int y, int bNum, int tbNum, bool isBuilt){
	if (isBuilt){
		/*if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
			g_demolish.Draw(x, y);
		}

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = DEMO;
			buildNum = bNum;
		}*/
	}
	else{
		if (CheckSBEnough(bNum)){
			if (Event.RMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				g_build.Draw(x, y);
			}

			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				mode = SBUILD;
				buildNum = tbNum;
				sBuildNum = bNum;
			}
		}
	}
}

void STownBox::DrawTB(){
	g_boxT.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	DrawFormatString(425, 230, BLACK, "レベル:%d/%d", tileInfo.townLv + 1, tileInfo.devLim);
	DrawString(425, 255, "産出", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(425, 275 + i * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 275 + i * I_SIZE + 2, BLACK, "%d", tileInfo.produce[i]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(425, 275 + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 275 + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tileInfo.trade[i]);
	}*/
	int count = 0, g[GOODS] = {};
	for (int i = 1; i < GOODS; i++){
		if (tileInfo.goods[i] > 0){
			g[count] = i;
			count++;
		}
	}

	for (int i = 0; i < count; i++){
		g_goods.Draw(316 + 282 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
		DrawFormatString(321 + 282 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, BLACK, "%d", tileInfo.goods[g[i]]);
	}

	for (int i = 0; i < BUILDINGS; i++){
		g_building.Draw(450 - B_SIZE / 2 + i * 150, 60, i + tileInfo.town * BUILDINGS);
	}

	PutBuildingButton(430, 60, 0, tileInfo.built[0]);
	PutBuildingButton(580, 60, 1, tileInfo.built[1]);
	PutBuildingButton(730, 60, 2, tileInfo.built[2]);

	switch (tileInfo.town){
	case FARM:
		g_SB.Draw(430, 120, 0);
		if (!tileInfo.built[3]){
			g_shadeS.Draw(430, 120);
		}
		break;

	case COMM:
		if (!townInfo.onlyOne[T_HALL] || tileInfo.built[3]) {
			g_SB.Draw(430, 120, 1);
		}
		if (!tileInfo.built[3] && !townInfo.onlyOne[T_HALL]){
			g_shadeS.Draw(430, 120);
		}
		break;

	default:
		break;
	}
	
	DrawBuildings(430, 60, 0);
	DrawBuildings(580, 60, 1);
	DrawBuildings(730, 60, 2);
	
	if (tileInfo.townLv + 1 < tileInfo.devLim){ 
		DrawDev(710 + GRID + 5, 270/*, tileInfo.townLv + 1*/);
		PutDevButton(710, 270/*, tileInfo.townLv + 1*/);
	}
	else {
		DrawString(660, 300, "これ以上開発できません", BLACK);
	}

	bool flag = false;
	switch (tileInfo.town){
	case FARM:
		if (/*townInfo.devSum >= 10 &&*/ tileInfo.townLv >= 4){
			PutSpBuildButton(430, 120, 0, 3, tileInfo.built[3]);
			DrawSBuildings(430, 120, 0, true);
		}else{
			DrawSBuildings(430, 120, 0, false);
		}
		break;

	case COMM:
		if (!townInfo.onlyOne[T_HALL]){
			if (townInfo.towns >= 5 && tileInfo.townLv >= 4){
				PutSpBuildButton(430, 120, 1, 3, tileInfo.built[3]);
				DrawSBuildings(430, 120, 1, true);
			}else{
				DrawSBuildings(430, 120, 1, false);
			}
		}
		break;

	default:
		break;
	}

	if (tileInfo.townLv >= 1){
		DrawDG(460 + GRID + 5, 470/*, tileInfo.townLv + 1*/);
		PutDGButton(460, 470/*, tileInfo.townLv + 1*/);
	}
	
	DrawString(635, 465, "元の地形", BLACK);
	g_tile.Draw(640, 490, tileInfo.terrain);
	DrawString(715, 465, "▼壊す", BLACK);
	g_demoL[0].Draw(720, 490);
	if (clickDemo){
		g_demoL[1].Draw(720, 490);
	}

	if (!open){
		PutRemoveButton(720, 490);
	}

	open = false;
}

CPicture SRiverBox::g_boxR;
CPicture SRiverBox::g_RB;

SRiverBox::SRiverBox(STile tile) {
	g_boxR.Load("Chikuwa3/Box_R.png");
	g_RB.Load("Chikuwa3/RBuilding.png", R_BUILDS, 1, RB_SIZE, RB_SIZE, R_BUILDS);

	tileInfo = tile;
	buildNum = 0;
}

void SRiverBox::PutRBButton(int x, int y, int bNum, bool isBuilt) {
	g_RB.Draw(x, y, bNum);

	if(isBuilt){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
			mode = DEMO;
			buildNum = bNum;
		}
	}
	else {
		g_shadeL.Draw(x, y);

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
			mode = BUILD;
			buildNum = bNum;
		}
	}
}

void SRiverBox::DrawDataBox(int x, int y, int bNum) {
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 246;
	if (Event.RMouse.GetOn(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
		if (mx < lim) {
			DrawBox(mx, my, mx + 230, my + 140, LIGHTYELLOW, true);

			DrawFormatString(mx + 5, my + 5, BLACK, "%s", rbData.name[bNum]);
			DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++) {
				g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < rbData.cost[bNum][i]) {
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][i]);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][i]);
				}
			}

			DrawString(mx + 5, my + I_SIZE * 3 + 10, "効果", BLACK);
			for (int i = 0; i < 2; i++) {
				DrawFormatString(mx + 5, my + I_SIZE * 4 + i * I_SIZE + 10, BLACK, "%s", rbData.exp[bNum][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 230, my + 140, LIGHTYELLOW, true);

			DrawFormatString(lim + 5, my + 5, BLACK, "%s", rbData.name[bNum]);
			DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++) {
				g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
				if (townInfo.resource[i] < rbData.cost[bNum][i]) {
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][i]);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][i]);
				}
			}

			DrawString(lim + 5, my + I_SIZE * 3 + 10, "効果", BLACK);
			for (int i = 0; i < 2; i++) {
				DrawFormatString(lim + 5, my + I_SIZE * 4 + i * I_SIZE + 10, BLACK, "%s", rbData.exp[bNum][i]);
			}
		}
	}
}

void SRiverBox::DrawRB(ETown u, ETown d, ETown l, ETown r) {
	g_boxR.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	if (u == FARM || d == FARM || l == FARM || r == FARM) {
		PutRBButton(430, 65, 0, tileInfo.built[0]);
	}
	if ((u != WILD && u != COMM) || (d != WILD && d != COMM) || (l != WILD && l != COMM) || (r != WILD && r != COMM)) {
		PutRBButton(580, 65, 1, tileInfo.built[1]);
	}
	if (u == COMM || d == COMM || l == COMM || r == COMM) {
		PutRBButton(730, 65, 2, tileInfo.built[2]);
	}

	if (u == FARM || d == FARM || l == FARM || r == FARM) {
		DrawDataBox(430, 65, 0);
	}
	if ((u != WILD && u != COMM) || (d != WILD && d != COMM) || (l != WILD && l != COMM) || (r != WILD && r != COMM)) {
		DrawDataBox(580, 65, 1);
	}
	if (u == COMM || d == COMM || l == COMM || r == COMM) {
		DrawDataBox(730, 65, 2);
	}

	open = false;
}