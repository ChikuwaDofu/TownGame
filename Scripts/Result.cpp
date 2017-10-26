#include"DxLib.h"
#include"Suken.h"
#include"Result.h"

CPicture CResult::g_town;
CPicture CResult::g_resource;
CPicture CResult::g_goods;
CPicture CResult::g_sArea;

SEndData::SEndData() {
	for (int i = 0; i < 9; i++) {
		townPop[i] = 0;
	}
	for (int i = 0; i < 8; i++) {
		spA[i] = false;
		spLv[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		resPro[i] = 0;
	}
	for (int i = 0; i < 22; i++) {
		goods[i] = 0;
	}
	tIncome = 0;
}

CPicture CResult::g_resB;

void CResult::Set() {
	g_resB.Load("Chikuwa3/Result.png");
	g_town.Load("Chikuwa3/Towns.png", 1, 9 + 1, 60, 60, 9 + 1);
	g_resource.Load("Chikuwa3/RIcons.png", 4, 1, 20, 20, 4);
	g_goods.Load("Chikuwa3/Goods.png", 22, 1, 40, 40, 22);
	g_sArea.Load("Chikuwa3/SpArea.png", 1, 8 + 1, 60, 60, 8 + 1);

	back = false;
}

void CResult::Draw() {
	g_resB.Draw(0, 0);

	for (int i = 0; i < 9; i++) {
		g_town.Draw(40 + i * 95, 120, i + 1);
		DrawFormatString(66 + i * 95, 185, BLACK, "%d", data.townPop[i]);
	}
	for (int i = 0; i < 8; i++) {
		if (data.spA[i]) {
			g_sArea.Draw(50 + i * 105, 250, i + 1);
			if (i >= 5) {
				DrawFormatString(50 + i * 105, 315, BLACK, "ãKñÕÅF%d", data.spLv[i]);
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		g_resource.Draw(50 + i * 200, 380, i);
		DrawFormatString(100 + i * 200, 382, BLACK, "%d", data.resPro[i]);
	}
	for (int i = 0; i < 11; i++) {
		g_goods.Draw(40 + i * 75, 435, i + 1);
		DrawFormatString(53 + i * 75, 480, BLACK, "%d", data.goods[i + 1]);
	}
	for (int i = 0; i < 11; i++) {
		g_goods.Draw(40 + i * 75, 505, i + 11);
		DrawFormatString(53 + i * 75, 550, BLACK, "%d", data.goods[i + 11]);
	}

	if (Event.LMouse.GetClick(859, -1, 900, 40)) {
		back = true;
	}
}