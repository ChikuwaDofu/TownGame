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
	close = false;
	select = false;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
	g_town.Load("Chikuwa3/Towns.png", TOWNS, TLVS, GRID, GRID, TOWNS * TLVS);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, 1, 40, 40, BUILDINGS);
	g_box.Load("Chikuwa3/Box.png");
	g_shade.Load("Chikuwa3/Shade.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	g_build.Load("Chikuwa3/Build.png");
	g_demolish.Load("Chikuwa3/Demolish.png");

	f_tData = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 0; i < TOWNS; i++){
		for (int j = 0; j < TLVS; j++){
			fscanf(f_tData, "%d %d", &tData.income_m[i][j], &tData.income_f[i][j]);
			fscanf(f_tData, "%d %d", &tData.cost_m[i][j], &tData.cost_f[i][j]);
		}
	}
	fclose(f_tData);
}

void SInfoBox::DrawIB(){
	g_box.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);

	if (Event.LMouse.GetClick(WINDOW_WIDTH - CROSS_L - 1, -1, WINDOW_WIDTH, CROSS_L) && !open){
		close = true;
	}
}

/////

SFoundBox::SFoundBox(ETerrain type){
	terrain = type;
}

void SFoundBox::PutButton(int x, int y, ETown type){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		town = type;
		select = true;
	}
}

void SFoundBox::DrawFB(int money){
	DrawIB();

	switch (terrain){
	case 0:
		g_town.Draw(540, 270, FARM);

		if (money < tData.cost_m[0][0]){
			DrawFormatString(550, 340, RED, "%d", tData.cost_m[0][0]);
		}
		else{
			if (!open){
				PutButton(540, 270, FARM);
			}

			DrawFormatString(550, 340, BLACK, "%d", tData.cost_m[0][0]);
		}

		g_resource.Draw(605, 270, MONEY);
		DrawFormatString(630, 270, BLACK, "%d", tData.income_m[0][0]);
		g_resource.Draw(605, 295, FOOD);
		DrawFormatString(630, 295, BLACK, "%d", tData.income_f[0][0]);

		break;

	/*case 1:
		g_town.Draw(570, 270, 1);

		if (money < tData.cost_m[0][0]){
			DrawFormatString(580, 340, RED, "%d", tData.cost_m[0][0]);
		}
		else{
			if (!open){
				PutButton(570, 270, PORT);
			}

			DrawFormatString(580, 340, BLACK, "%d", tData.cost_m[1][0]);
		}

		break;*/

	default:
		break;
	}

	open = false;
}

/////

STownBox::STownBox(ETerrain type){
	remove = false;
	develop = false;
	build = false;
	demolish = false;
	devLv = 0;
	terrain = type;
}

void STownBox::PutRemoveButton(int x, int y){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		remove = true;
	}
}

void STownBox::PutDevButton(int x, int y, int lv){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		develop = true;
		devLv = lv;
	}
}

void STownBox::PutBuildButton(int x, int y, int bNum){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		build = true;
		buildNum = bNum;
	}
}

void STownBox::PutDemolishButton(int x, int y, int bNum){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		demolish = true;
		buildNum = bNum;
	}
}

void STownBox::DrawTB(STile town, int money){
	DrawIB();

	g_resource.Draw(425, 485, MONEY);
	DrawFormatString(450, 485, BLACK, "%d", tData.income_m[town.town][town.townLv]);
	g_resource.Draw(425, 505, FOOD);
	DrawFormatString(450, 505, BLACK, "%d", tData.income_f[town.town][town.townLv]);

	g_building.Draw(430, 60, 0);

	if (town.built[0]){
		if (Event.RMouse.GetOn(430, 60, 470, 100)){
			g_demolish.Draw(430, 60);
		}

		PutDemolishButton(430, 60, 0);
	}
	else{
		g_shade.Draw(430, 60);

		if (Event.RMouse.GetOn(430, 60, 470, 100)){
			g_build.Draw(430, 60);
		}

		PutBuildButton(430, 60, 0);
	}

	if (town.townLv == 1){
		g_building.Draw(730, 60, 1);

		if (town.built[1]){
			if (Event.RMouse.GetOn(730, 60, 770, 100)){
				g_demolish.Draw(730, 60);
			}

			PutDemolishButton(730, 60, 1);
		}
		else{
			g_shade.Draw(730, 60);
			if (Event.RMouse.GetOn(730, 60, 770, 100)){
				g_build.Draw(730, 60);
			}

			PutBuildButton(730, 60, 1);
		}
	}

	if (town.townLv == 2){
		g_building.Draw(730, 60, 2);
		if (Event.RMouse.GetOn(730, 60, 770, 100)){
			
		}

		if (town.built[2]){
			if (Event.RMouse.GetOn(730, 60, 770, 100)){
				g_demolish.Draw(730, 60);
			}

			PutDemolishButton(730, 60, 2);
		}
		else{
			g_shade.Draw(730, 60);
			if (Event.RMouse.GetOn(730, 60, 770, 100)){
				g_build.Draw(730, 60);
			}

			PutBuildButton(730, 60, 2);
		}
	}

	if (town.townLv == 0){
		g_town.Draw(440, 270, town.town + TOWNS * 1);

		if (money < tData.cost_m[town.town][1]){
			DrawFormatString(450, 340, RED, "%d", tData.cost_m[town.town][1]);
		}
		else{
			DrawFormatString(450, 340, BLACK, "%d", tData.cost_m[town.town][1]);

			if (!open){
				PutDevButton(440, 270, 1);
			}
		}

		g_resource.Draw(505, 270, MONEY);
		DrawFormatString(530, 270, BLACK, "+%d", tData.income_m[town.town][1] - tData.income_m[town.town][0]);
		g_resource.Draw(505, 295, FOOD);
		DrawFormatString(530, 295, BLACK, "+%d", tData.income_f[town.town][1] - tData.income_f[town.town][0]);

		g_town.Draw(640, 270, town.town + TOWNS * 2);

		if (money < tData.cost_m[town.town][2]){
			DrawFormatString(650, 340, RED, "%d", tData.cost_m[town.town][2]);
		}
		else{
			DrawFormatString(650, 340, BLACK, "%d", tData.cost_m[town.town][2]);

			if (!open){
				PutDevButton(640, 270, 2);
			}
		}

		g_resource.Draw(705, 270, MONEY);
		DrawFormatString(730, 270, BLACK, "+%d", tData.income_m[town.town][2] - tData.income_m[town.town][0]);
		g_resource.Draw(705, 295, FOOD);
		DrawFormatString(730, 295, BLACK, "+%d", tData.income_f[town.town][2] - tData.income_f[town.town][0]);
	}

	g_tile.Draw(720, 470, terrain);

	if (!open){
		PutRemoveButton(720, 470);
	}

	open = false;

	/*case 0:
	case 2:
	case 4:

		g_town.Draw(540, 270, town + (lv + 1) * TOWNS);

		if (money < tData.cost_m[town][lv + 1]){
			DrawFormatString(550, 340, RED, "%d", tData.cost_m[town][lv + 1]);
		}
		else{
			DrawFormatString(550, 340, BLACK, "%d", tData.cost_m[town][lv + 1]);

			if (!open){
				PutDevButton(540, 270, lv + 1);
			}
		}

		g_resource.Draw(605, 270, MONEY);
		DrawFormatString(630, 270, BLACK, "+%d", tData.income_m[town][lv + 1] - tData.income_m[town][lv]);
		g_resource.Draw(605, 295, FOOD);
		DrawFormatString(630, 295, BLACK, "+%d", tData.income_f[town][lv + 1] - tData.income_f[town][lv]);

		break;

	default:

		break;
	}

	g_tile.Draw(570, 470, terrain);

	if (!open){
		PutRemoveButton(570, 470);
	}

	open = false;*/
}
