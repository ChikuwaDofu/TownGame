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

	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
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

SFoundBox::SFoundBox(ETerrain type){
	terrain = type;
}

void SFoundBox::PutButton(int x, int y, ETown type){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		town = type;
		mode = EST;
	}
}

void SFoundBox::DrawFB(int money){
	DrawIB();

	switch (terrain){
	case 0:
		g_town.Draw(540, 270, FARM);

		if (money < tData.cost[0][0][MONEY]){
			DrawFormatString(550, 340, RED, "%d", tData.cost[0][0][MONEY]);
		}
		else{
			if (!open){
				PutButton(540, 270, FARM);
			}

			DrawFormatString(550, 340, BLACK, "%d", tData.cost[0][0][MONEY]);
		}

		g_resource.Draw(605, 270, MONEY);
		DrawFormatString(630, 270, BLACK, "%d", tData.income[0][0][MONEY]);
		g_resource.Draw(605, 295, FOOD);
		DrawFormatString(630, 295, BLACK, "%d", tData.income[0][0][FOOD]);

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
	devLv = 0;
	terrain = type;
}

void STownBox::PutRemoveButton(int x, int y){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = REMV;
	}
}

void STownBox::PutDevButton(int x, int y, int lv){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = DEV;
		devLv = lv;
	}
}

void STownBox::PutBuildButton(int x, int y, int bNum){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = BUILD;
		buildNum = bNum;
	}
}

void STownBox::PutDemolishButton(int x, int y, int bNum){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
		mode = DEMO;
		buildNum = bNum;
	}
}

void STownBox::DrawTB(STile town, int money){
	DrawIB();

	g_resource.Draw(425, 485, MONEY);
	DrawFormatString(450, 485, BLACK, "%d", town.produce[MONEY]);
	g_resource.Draw(425, 505, FOOD);
	DrawFormatString(450, 505, BLACK, "%d", town.produce[FOOD]);

	g_building.Draw(430, 60, 0);
	g_resource.Draw(475, 55, MONEY);
	DrawFormatString(500, 55, BLACK, "%d", bData.income[town.town][0][MONEY]);
	g_resource.Draw(475, 80, FOOD);
	DrawFormatString(500, 80, BLACK, "%d", bData.income[town.town][0][FOOD]);

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
		g_resource.Draw(775, 55, MONEY);
		DrawFormatString(800, 55, BLACK, "%d", bData.income[town.town][1][MONEY]);
		g_resource.Draw(775, 80, FOOD);
		DrawFormatString(800, 80, BLACK, "%d", bData.income[town.town][1][FOOD]);
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
		g_resource.Draw(775, 55, MONEY);
		DrawFormatString(800, 55, BLACK, "%d", bData.income[town.town][1][MONEY]);
		g_resource.Draw(775, 80, FOOD);
		DrawFormatString(800, 80, BLACK, "%d", bData.income[town.town][1][FOOD]);
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
		g_town.Draw(440, 270, 1 + town.town*TOWNS);

		if (money < tData.cost[town.town][1][MONEY]){
			DrawFormatString(450, 340, RED, "%d", tData.cost[town.town][1][MONEY]);
		}
		else{
			DrawFormatString(450, 340, BLACK, "%d", tData.cost[town.town][1][MONEY]);

			if (!open){
				PutDevButton(440, 270, 1);
			}
		}

		g_resource.Draw(505, 270, MONEY);
		DrawFormatString(530, 270, BLACK, "+%d", tData.income[town.town][1][MONEY] - tData.income[town.town][0][MONEY]);
		g_resource.Draw(505, 295, FOOD);
		DrawFormatString(530, 295, BLACK, "+%d", tData.income[town.town][1][FOOD] - tData.income[town.town][0][FOOD]);

		g_town.Draw(640, 270, 2 + town.town*TOWNS);

		if (money < tData.cost[town.town][2][MONEY]){
			DrawFormatString(650, 340, RED, "%d", tData.cost[town.town][2][MONEY]);
		}
		else{
			DrawFormatString(650, 340, BLACK, "%d", tData.cost[town.town][2][MONEY]);

			if (!open){
				PutDevButton(640, 270, 2);
			}
		}

		g_resource.Draw(705, 270, MONEY);
		DrawFormatString(730, 270, BLACK, "+%d", tData.income[town.town][2][MONEY] - tData.income[town.town][0][MONEY]);
		g_resource.Draw(705, 295, FOOD);
		DrawFormatString(730, 295, BLACK, "+%d", tData.income[town.town][2][FOOD] - tData.income[town.town][0][FOOD]);
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
