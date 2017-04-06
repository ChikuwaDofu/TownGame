#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
CPicture SInfoBox::g_box;

SInfoBox::SInfoBox(){
	close = false;
	select = false;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
	g_town.Load("Chikuwa3/Towns.png", TOWNS, TLVS, GRID, GRID, TOWNS * TLVS);
	g_box.Load("Chikuwa3/Box.png");

	f_tData = fopen("Chikuwa3/TownData.txt", "r");
	for (int i = 0; i < TOWNS; i++){
		for (int j = 0; j < TLVS; j++){
			fscanf(f_tData, "%d %d", &tData.income_m[i][j], &tData.cost_m[i][j]);
		}
	}
	fclose(f_tData);
}

void SInfoBox::DrawIB(){
	g_box.Draw(300, 0);

	if (Event.LMouse.GetClick(859, -1, 900, 40) && !open){
		close = true;
	}
}

/////

SFoundBox::SFoundBox(ETerrain type){
	terrain = type;
}

void SFoundBox::PutButton(int x, int y, ETown type){
	if (Event.LMouse.GetClick(x - 1, y - 1, x + 60, y + 60)){
		town = type;
		select = true;
	}
}

void SFoundBox::DrawFB(int money){
	DrawIB();

	switch (terrain){
	case 0:
		g_town.Draw(570, 270, FARM);

		if (money < tData.cost_m[0][0]){
			DrawFormatString(580, 340, RED, "%d", tData.cost_m[0][0]);
		}
		else{
			if (!open){
				PutButton(570, 270, FARM);
			}

			DrawFormatString(580, 340, BLACK, "%d", tData.cost_m[0][0]);
		}

		DrawFormatString(570, 360, BLACK, "Žû“ü: %d", tData.income_m[0][0]);

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

void STownBox::DrawTB(ETown town, int lv, int money){
	DrawIB();

	DrawFormatString(570, 90, BLACK, "Žû“ü: %d", tData.income_m[town][lv]);

	switch (lv){
	case 1:

		g_town.Draw(470, 270, town + TOWNS * 2);

		if (money < tData.cost_m[town][2]){
			DrawFormatString(480, 340, RED, "%d", tData.cost_m[town][2]);
		}
		else{
			DrawFormatString(480, 340, BLACK, "%d", tData.cost_m[town][2]);

			if (!open){
				PutDevButton(470, 270, 2);
			}
		}

		DrawFormatString(470, 360, BLACK, "Žû“ü +%d", tData.income_m[town][2] - tData.income_m[town][1]);

		g_town.Draw(670, 270, town + TOWNS * 4);

		if (money < tData.cost_m[town][4]){
			DrawFormatString(680, 340, RED, "%d", tData.cost_m[town][4]);
		}
		else{
			DrawFormatString(680, 340, BLACK, "%d", tData.cost_m[town][4]);

			if (!open){
				PutDevButton(670, 270, 4);
			}
		}

		DrawFormatString(670, 360, BLACK, "Žû“ü +%d", tData.income_m[town][4] - tData.income_m[town][1]);

		break;

	case 0:
	case 2:
	case 4:

		g_town.Draw(570, 270, town + (lv + 1) * TOWNS);

		if (money < tData.cost_m[town][lv + 1]){
			DrawFormatString(580, 340, RED, "%d", tData.cost_m[town][lv + 1]);
		}
		else{
			DrawFormatString(580, 340, BLACK, "%d", tData.cost_m[town][lv + 1]);

			if (!open){
				PutDevButton(570, 270, lv + 1);
			}
		}

		DrawFormatString(570, 360, BLACK, "Žû“ü +%d", tData.income_m[town][lv + 1] - tData.income_m[town][lv]);

		break;

	default:

		break;
	}

	g_tile.Draw(570, 470, terrain);

	if (!open){
		PutRemoveButton(570, 470);
	}

	open = false;
}
