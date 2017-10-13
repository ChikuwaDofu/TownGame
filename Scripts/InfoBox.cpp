#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
CPicture SInfoBox::g_pasture;
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
CPicture SInfoBox::g_sArea;

SInfoBox::SInfoBox(){
	mode = KEEP;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS + 1, GRID, GRID, TOWNS + 1);
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, TOWNS + 1, 40, 40, BUILDINGS * (TOWNS + 1));
	//g_box.Load("Chikuwa3/Box.png");
	g_shadeS.Load("Chikuwa3/Shade40.png");
	g_shadeL.Load("Chikuwa3/Shade50.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	//g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, 20, 20, TRADE);
	g_goods.Load("Chikuwa3/Goods.png", GOODS, 1, G_SIZE, G_SIZE, GOODS);
	g_build.Load("Chikuwa3/Build.png");
	g_demolish.Load("Chikuwa3/Demolish.png");
	g_mineral.Load("Chikuwa3/Mineral.png", MNR_TYPE, 1, R_SIZE, R_SIZE, MNR_TYPE);
	g_sArea.Load("Chikuwa3/SpArea.png", 1, SP_AREAS + 1, GRID, GRID, SP_AREAS + 1);
}

void SInfoBox::DrawIB(){

	if ((Event.LMouse.GetClick(WINDOW_WIDTH - CROSS_L - 1, -1, WINDOW_WIDTH, CROSS_L) || Event.RMouse.GetClick()) && !open){
		mode = CLOSE;
	}
}

/////

CPicture SFoundBox::g_boxF;
CPicture SFoundBox::g_cbut;
CPicture SFoundBox::g_pasture;
CPicture SFoundBox::g_saBut;
CPicture SFoundBox::g_naBut;
CPicture SFoundBox::g_search;

SFoundBox::SFoundBox(ETerrain type, STown town, bool cFlag, bool pas_s, bool pas_c, bool pas_p, bool sFlag){
	terrain = type;
	townInfo = town;
	pType = 0;
	saType = 0;
	cut = cFlag;
	search = sFlag;
	pasture[0] = pas_s;
	pasture[1] = pas_c;
	pasture[2] = pas_p;
	spArea = false;
	spAAble[0] = false;
	for (int i = 1; i <= SP_AREAS; i++) {
		spAAble[i] = CheckSpAAble(i);
	}

	g_boxF.Load("Chikuwa3/Box_F.png");
	g_cbut.Load("Chikuwa3/CButton.png");
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);
	g_saBut.Load("Chikuwa3/SaButton.png");
	g_naBut.Load("Chikuwa3/NaButton.png");
	g_search.Load("Chikuwa3/Search.png");
}

bool SFoundBox::CheckSpAAble(int n) {
	int sum = 0;
	for (int i = 1; i <= TOWNS; i++) {
		if (saData.rPType[n][i]) {
			sum += townInfo.tTypePop[i];
		}
	}
	if (sum >= saData.reqPop[n]) {
		return true;
	}
	else {
		return false;
	}
}

bool SFoundBox::CheckSAEnough(int type) {
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < saData.cost[type][i]) {
			return false;
		}
	}

	return true;
}

void SFoundBox::PutSAB(int x, int y, int type) {
	g_sArea.Draw(x, y, type);
	
	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", saData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);

	bool flag = true;
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x - (I_SIZE + 45), y + I_SIZE * i, i);
		if (townInfo.resource[i] < saData.cost[type][i]) {
			flag = false;
			DrawFormatString(x - 40, y + I_SIZE * i + 2, RED, "-%d", saData.cost[type][i]);
		}
		else {
			DrawFormatString(x - 40, y + I_SIZE * i + 2, BLACK, "-%d", saData.cost[type][i]);
		}
	}
	if (flag) {
		if (!open && Event.LMouse.GetClick(x, y, x + GRID, y + GRID) && spAAble[type]) {
			saType = type;
			mode = BLD_SA;
		}
	}
}

void SFoundBox::DrawSAReq(int x, int y, int type, bool ok) {
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 171;
	int col = 0;
	if (ok) {
		col = BLACK;
	}
	else {
		col = RED;
	}
	if (Event.LMouse.GetOn(x - 1, y - 1, x + GRID, y + GRID)) {
		if (mx < lim) {
			DrawBox(mx, my, mx + 155, my + saData.reqL[type] * 25 + 5, LIGHTYELLOW, true);
			for (int i = 0; i < saData.reqL[type]; i++) {
				DrawFormatString(mx + 5, my + 5 + i * 25, col, "%s", saData.req[type][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 155, my + saData.reqL[type] * 25 + 5, LIGHTYELLOW, true);
			for (int i = 0; i < saData.reqL[type]; i++) {
				DrawFormatString(lim + 5, my + 5 + i * 25, col, "%s", saData.req[type][i]);
			}
		}
	}
}


bool SFoundBox::CheckTEnough(ETown type){
	for(int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			return false;
		}
	}

	return true;
}

void SFoundBox::PutPB(int x, int y, int type) {
	g_pasture.Draw(x, y, terrain + type*TERRAINS);

	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", pData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	DrawString(x + GRID + 5, y - 20, "産出", BLACK);
	DrawPData(x, y, type);

	bool flag = true;
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < pData.cost[type][i]) {
			flag = false;
		}
	}

	if (flag) {
		if (!open && Event.LMouse.GetClick(x, y, x + GRID, y + GRID)) {
			pType = type;
			mode = BLD_P;
		}
	}
}

void SFoundBox::DrawPData(int x, int y, int type) {
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < pData.cost[type][i]) {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", pData.cost[type][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", pData.cost[type][i]);
		}
	}
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x + GRID + 5, y + i * I_SIZE, i);
		DrawFormatString(x + GRID + 5 + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "%d", pData.income[type][i]);
	}
	if (pData.goods[type] != 0) {
		g_goods.Draw(x + GRID + 5, y + RESOURCES * I_SIZE, pData.goods[type]);
	}
}

void SFoundBox::PutTB(int x, int y, ETown type){
	g_town.Draw(x, y, type);
	
	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", tData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	DrawString(x + GRID + 5, y - 20, "産出", BLACK);
	DrawTData(x + GRID + 5, y, type);

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[type][NEW][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[type][NEW][i]);
		}
	}

	if (CheckTEnough(type)){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			town = type;
			mode = EST;
		}
	}
}

void SFoundBox::DrawTData(int x, int y, ETown type){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "%d", tData.income[type][NEW][i]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
	}*/
	if (tData.goods[type][0] != 0) {
		g_goods.Draw(x, y + RESOURCES * I_SIZE, tData.goods[type][0]);
	}
	if (tData.goods[type][0] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES * I_SIZE + 10, CYAN, "%d", tData.trade[type][0]);
	}
	//DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
}

void SFoundBox::DrawFB(int money/*, EMineral mineral*/) {
	g_boxF.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	/*if (terrain == HILL){
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
	}*/

	switch (terrain) {
	case HILL_S:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, ROCK);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 3, 450 + R_SIZE / 2 + 5, "石材", BLACK);
		break;

	case HILL_G:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, GOLD);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 10, 450 + R_SIZE / 2 + 5, "金", BLACK);
		break;

	case HILL_I:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, IRON);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 - 5, 450 + R_SIZE / 2 + 5, "鉄鉱石", BLACK);
		break;

	default:
		break;
	}

	if (townInfo.towns < townInfo.townMax) {
		switch (terrain) {
		case PLAIN:

			if (spArea) {
				g_naBut.Draw(340, 87);
				if (!open && Event.LMouse.GetClick(340, 87, 340 + 105, 87 + 105)) {
					spArea = false;
				}
				DrawString(400, 30, "絵にカーソルを合わせると条件を見られます", BLACK);
				for (int i = 1; i <= SP_AREAS; i++) {
					PutSAB(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), i);
				}
				for (int i = 1; i <= SP_AREAS; i++) {
					DrawSAReq(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), i, spAAble[i]);
				}
			}
			else {
				g_saBut.Draw(340, 87);
				if (!open && Event.LMouse.GetClick(340, 87, 340 + 105, 87 + 105)) {
					spArea = true;
				}
				PutTB(570, 100, FARM);
				PutTB(750, 100, COMM);
				PutTB(390, 270, PAS_S);
				PutTB(570, 270, PAS_C);
				PutTB(750, 270, PAS_P);
			}

			break;

		case FOREST:
			PutTB(540, 120, F_VIL);
			break;

			/*case HILL:
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
			break;*/

		case HILL_S:
			PutTB(540, 120, MINE_S);

			if (search) {
				g_search.Draw(450, 465);
				DrawString(445, 522, "コスト", BLACK);
				g_resource.Draw(505, 520, MONEY);
				if (money >= 80) {
					DrawString(530, 522, "80", BLACK);
					if (Event.LMouse.GetClick(449, 464, 550, 515) && !open) {
						mode = SEARCH;
					}
				}
				else {
					DrawString(530, 522, "80", RED);
				}
			}
			break;

		case HILL_G:
			PutTB(540, 120, MINE_G);
			break;

		case HILL_I:
			PutTB(540, 120, MINE_I);
			break;

		default:
			break;
		}
	}
	else {
		DrawString(500, 120, "これ以上都市を造れません！", BLACK);
	}

	if (cut) {
		g_cbut.Draw(450, 465);
		DrawString(445, 522, "コスト", BLACK);
		g_resource.Draw(505, 520, MONEY);

		if (money >= 50) {
			DrawString(530, 522, "50", BLACK);
			if (Event.LMouse.GetClick(449, 464, 550, 515) && !open) {
				mode = CUT;
			}
		}
		else {
			DrawString(530, 522, "50", RED);
		}
	}

	if (pasture[0]) {
		PutPB(470, 460, 0);
	}
	if (pasture[1]) {
		PutPB(670, 460, 1);
	}
	if (pasture[2]) {
		PutPB(730, 460, 2);
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
	if (tData.goods[tileInfo.town][LVUP] != 0) {
		g_goods.Draw(x, y + RESOURCES*I_SIZE, tData.goods[tileInfo.town][LVUP]);
	}
	if (tData.goods[tileInfo.town][LVUP] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES*I_SIZE + 10, CYAN, "%d", tData.trade[tileInfo.town][LVUP]);
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
	g_goods.Draw(x, y + RESOURCES*I_SIZE, tData.goods[tileInfo.town][LVUP]);
	if (tData.goods[tileInfo.town][LVUP] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES*I_SIZE + 10, CYAN, "%d", tData.trade[tileInfo.town][LVUP]);
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
	int lim = WINDOW_WIDTH - 241;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 225, my + 160, LIGHTYELLOW, true);

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
			if (bData.goods[tileInfo.town][build] != 0) {
				g_goods.Draw(mx + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
			}
			if (bData.goods[tileInfo.town][build] == 1) {
				DrawFormatString(mx + G_SIZE + 10, my + 25 + I_SIZE * 5, CYAN, "%d", bData.trade[tileInfo.town][build]);
			}
			if ((tileInfo.town == PAS_S || tileInfo.town == PAS_C || tileInfo.town == PAS_P) && build == 0) {
				DrawString(mx + 5, my + 20 + I_SIZE * 5, "周りに放牧地が", BLACK);
				DrawString(mx + 5, my + 20 + I_SIZE * 6, "造れるようになる", BLACK);
			}
		}
		else {
			DrawBox(lim, my, lim + 225, my + 160, LIGHTYELLOW, true);
			
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
			if (bData.goods[tileInfo.town][build] != 0) {
				g_goods.Draw(lim + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
			}
			if (bData.goods[tileInfo.town][build] == 1) {
				DrawFormatString(lim + G_SIZE + 10, my + 25 + I_SIZE * 5, CYAN, "%d", bData.trade[tileInfo.town][build]);
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
	int lim = WINDOW_WIDTH - 261;
	int l = 0;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 245, my + 180, LIGHTYELLOW, true);

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

			if (sbData.onlyOne[build]){
				DrawString(mx + 5, my + 10 + I_SIZE * 5, "地域に一つだけ建てられる", BLACK);
				l = 1;
			}

			DrawString(mx + 5, my + 15 + I_SIZE * (5 + l), "効果", BLACK);
			DrawFormatString(mx + 5, my + 15 + I_SIZE * (6 + l), BLACK, "%s", sbData.exp[build]);
		}
		else {
			DrawBox(lim, my, lim + 245, my + 180, LIGHTYELLOW, true);
			
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

			if (sbData.onlyOne[build]){
				DrawString(lim + 5, my + 10 + I_SIZE * 5, "地域に１つだけ建てられる", BLACK);
				l = 1;
			}

			DrawString(lim + 5, my + 15 + I_SIZE * (5 + l), "効果", BLACK);
			DrawFormatString(lim + 5, my + 15 + I_SIZE * (6 + l), BLACK, "%s", sbData.exp[build]);
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

	DrawFormatString(415, 210, BLACK, "%s", tData.name[tileInfo.town]);
	DrawFormatString(415, 235, BLACK, "都市人口:%d/%d", tileInfo.townLv + 1, tileInfo.devLim);
	DrawString(440, 255, "産出", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(450, 275 + i * I_SIZE, i);
		DrawFormatString(450 + I_SIZE + 5, 275 + i * I_SIZE + 2, BLACK, "%d", tileInfo.produce[i]);
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
		if (g[i] == 1) {
			g_goods.Draw(316 + 282 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
			DrawFormatString(321 + 282 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, CYAN, "%d", tileInfo.goods[g[i]]);
		}
		else {
			g_goods.Draw(316 + 282 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
			DrawFormatString(321 + 282 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, BLACK, "%d", tileInfo.goods[g[i]]);
		}
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

	case MINE_G:
	case MINE_I:
		g_SB.Draw(580, 120, 2);
		if (!tileInfo.built[4]) {
			g_shadeS.Draw(580, 120);
		}

	case MINE_S:
		g_SB.Draw(430, 120, 3);
		if (!tileInfo.built[3]){
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

	case MINE_G:
	case MINE_I:
		if (tileInfo.townLv >= 4) {
			PutSpBuildButton(580, 120, 2, 4, tileInfo.built[4]);
			DrawSBuildings(580, 120, 2, true);
		}
		else {
			DrawSBuildings(580, 120, 2, false);
		}

	case MINE_S:
		if (townInfo.maxMines >= 5){
			PutSpBuildButton(430, 120, 3, 3, tileInfo.built[3]);
			DrawSBuildings(430, 120, 3, true);
		}
		else{
			DrawSBuildings(430, 120, 3, false);
		}
		break;

	default:
		break;
	}

	if (tileInfo.townLv >= 1){
		DrawDG(460 + GRID + 5, 450/*, tileInfo.townLv + 1*/);
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

/////

CPicture SPastureBox::g_boxP;
CPicture SPastureBox::g_pasture;
CPicture SPastureBox::g_demoP;

SPastureBox::SPastureBox(STile tile, int m, int n) {
	g_boxP.Load("Chikuwa3/Box_P.png");
	//g_pasture.Load();
	g_demoP.Load("Chikuwa3/Demo_P.png");
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);

	money = m;
	num = n;
	tileInfo = tile;
}

void SPastureBox::DrawPB() {
	g_boxP.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	DrawFormatString(470, 125, BLACK, "%s", pData.name[num]);
	g_pasture.Draw(470, 150, tileInfo.terrain + num * TERRAINS);

	DrawString(670, 100, "産出", BLACK);
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(670, 125 + I_SIZE * i, i);
		DrawFormatString(670 + I_SIZE + 5, 127 + I_SIZE * i, BLACK, "%d", pData.income[num][i]);
	}
	g_goods.Draw(670, 125 + I_SIZE * RESOURCES, pData.goods[num]);

	g_demoP.Draw(570, 400);
	DrawString(575, 380, "▼壊す", BLACK);
	DrawString(570, 465, "コスト", BLACK);
	g_resource.Draw(570, 490, MONEY);

	if (money >= 30) {
		DrawString(600, 492, "30", BLACK);

		if (Event.LMouse.GetClick(569, 399, 630, 460) && !open) {
			mode = DEMO;
		}
	}
	else {
		DrawString(600, 492, "30", RED);
	}

	open = false;
}

/////

CPicture SRiverBox::g_boxR;
CPicture SRiverBox::g_RB;

SRiverBox::SRiverBox(STown town, STile tile) {
	g_boxR.Load("Chikuwa3/Box_R.png");
	g_RB.Load("Chikuwa3/RBuilding.png", R_BUILDS, 1, RB_SIZE, RB_SIZE, R_BUILDS);

	townInfo = town;
	tileInfo = tile;
	buildNum = 0;
}

bool SRiverBox::CheckRBEnough(int bNum) {
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < rbData.cost[bNum][i]) {
			return false;
		}
	}

	return true;
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
		if (CheckRBEnough(bNum)) {
			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
				mode = BUILD;
				buildNum = bNum;
			}
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

/////

CPicture STradeBox::g_boxG;
CPicture STradeBox::g_arrow;

STradeBox::STradeBox(STown town) {
	townInfo = town;

	g_boxG.Load("Chikuwa3/Box_G.png");
	g_arrow.Load("Chikuwa3/Arrow.png");
}

void STradeBox::DrawArrow(int x, int y, int n) {
	g_arrow.Draw(x, y);
	DrawString(x + 11, y + 25, "消", BLACK);
	DrawString(x + 11, y + 42, "費", BLACK);
	DrawVString(x + 10, y + 56, "：", BLACK);
	DrawFormatString(x + 15, y + 69, BLACK, "%d", n);
}

void STradeBox::DrawTB() {
	g_boxG.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	for (int i = 0; i < GOODS / 5; i++) {
		DrawString(330, 75 + i * 120, "価値", ORANGE);
		DrawString(330, 100 + i * 120, "生産", DARKGREEN);
		DrawString(330, 120 + i * 120, "消費", RED);
	}
	for (int i = 2; i < GOODS; i++) {
		int show = i - 2;
		g_goods.Draw(390 + show % 5 * 100, 30 + show / 5 * 120, i);
		DrawFormatString(392 + show % 5 * 100, 75 + show / 5 * 120, ORANGE, "%.2lf", gData.value[i]);
		DrawCenterString(409 + show % 5 * 100, 100 + show / 5 * 120, DARKGREEN, "%d", townInfo.goodsPro[i]);
		DrawCenterString(409 + show % 5 * 100, 120 + show / 5 * 120, RED, "%d", townInfo.goodsCon[i]);
	}

	DrawArrow(440, 40, 3);
	DrawArrow(440, 160, 1);
	DrawArrow(540, 160, 3);
	DrawArrow(740, 160, 2);
	DrawArrow(440, 280, 2);
	DrawArrow(640, 280, 2);
	DrawArrow(740, 280, 1);
	DrawArrow(440, 400, 2);

	g_goods.Draw(400, 510, 1);
	DrawFormatString(450, 522, CYAN, "%d", townInfo.goodsPro[1]);
	double a = 1.0 - ((double)townInfo.goodsPro[1]) / 100;
	DrawFormatString(500, 512, DARKGREEN, "交易収入:%.2lf × %.2lf = %.2lf", townInfo.exSum, 1 + ((double)townInfo.goodsPro[1]) / 100, townInfo.exFin);
	DrawFormatString(500, 537, RED, "交易支出:%.2lf × %.2lf = %.2lf", townInfo.inSum, a * 2.5, townInfo.inFin);

	open = false;
}

/////

CPicture SBuyBox::g_boxB;
CPicture SBuyBox::g_brbut;

int mod[RESOURCES] = { 0, 4, 5, 5 };

SBuyBox::SBuyBox(STown town) {
	townInfo = town;
	bought = false;
	rType = MONEY;
	amount = 0;
	cost = 0;

	g_boxB.Load("Chikuwa3/Box_B.png");
	g_brbut.Load("Chikuwa3/BuyRButton.png", RESOURCES, 1, BRB_SIZE, BRB_SIZE, RESOURCES);
}

void SBuyBox::DrawBB(int money) {
	g_boxB.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	bought = false;
	rType = MONEY;
	amount = 0;
	cost = 0;

	DrawFormatString(300 + 1 * 150 - BRB_SIZE / 2, 170, ORANGE, "コスト:100");
	for (int i = 1; i < RESOURCES; i++) {
		PutBuyButton(300 + i * 150 - BRB_SIZE / 2, 200, 0, (EResource)i, 100, 100 / mod[i], money);
	}

	open = false;
}

void SBuyBox::PutBuyButton(int x, int y, int trade, EResource type, int c, int a, int money) {
	DrawFormatString(x, y + BRB_SIZE + 5, BLACK, "買い入れ量:%d", a);

	if (money >= c) {
		if (Event.LMouse.GetClick(x - 1, y - 1, x + BRB_SIZE, y + BRB_SIZE)) {
			bought = true;
			rType = type;
			amount = a;
			cost = c;
		}

		g_brbut.Draw(x, y, type);
	}
	else {
		g_brbut.Draw(x, y, type);

		DrawString(x, y + BRB_SIZE + 30, "お金が足りません", RED);
	}
}