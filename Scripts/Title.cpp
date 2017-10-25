#include"DxLib.h"
#include"Suken.h"
#include"Title.h"

CPicture CTitle::g_title;
CPicture CTitle::g_newGame;
CPicture CTitle::g_newGameOn;
CPicture CTitle::g_loadGame;
CPicture CTitle::g_loadGameOn;
CPicture CTitle::g_HtP[HTP];
CPicture CTitle::g_AR;
CPicture CTitle::g_AL;
CPicture CTitle::g_htpB;
CPicture CTitle::g_htpBOn;

CTitle::CTitle() {
	newGame = false;
	loadGame = false;
	hFlag = false;
	htNum = 0;
}

void CTitle::Set() {
	g_title.Load("Chikuwa3/Title.png");
	g_newGame.Load("Chikuwa3/NewGame.png");
	g_newGameOn.Load("Chikuwa3/NewGameOn.png");
	g_loadGame.Load("Chikuwa3/LoadGame.png");
	g_loadGameOn.Load("Chikuwa3/LoadGameOn.png");
	g_HtP[0].Load("Chikuwa3/HtPF.png");
	g_HtP[1].Load("Chikuwa3/HtPTu.png");
	g_HtP[2].Load("Chikuwa3/HtPR.png");
	g_HtP[3].Load("Chikuwa3/HtPBT.png");
	g_HtP[4].Load("Chikuwa3/HtPTe.png");
	g_HtP[5].Load("Chikuwa3/HtPT1.png");
	g_HtP[6].Load("Chikuwa3/HtPT2.png");
	g_HtP[7].Load("Chikuwa3/HtPTi.png");
	g_HtP[8].Load("Chikuwa3/HtPI.png");
	g_htpB.Load("Chikuwa3/HowTo.png");
	g_htpBOn.Load("Chikuwa3/HowToOn.png");
	g_AR.Load("Chikuwa3/ArrowR.png");
	g_AL.Load("Chikuwa3/ArrowL.png");
}

void CTitle::Draw() {
	if (!hFlag) {
		g_title.Draw(0, 0);
		g_newGame.Draw(300, 180);
		if (Event.RMouse.GetOn(299, 179, 600, 240)) {
			g_newGameOn.Draw(300, 180);
		}
		if (Event.LMouse.GetClick(299, 179, 600, 240)) {
			newGame = true;
		}
		g_loadGame.Draw(300, 280);
		if (Event.RMouse.GetOn(299, 279, 600, 340)) {
			g_loadGameOn.Draw(300, 280);
		}
		if (Event.LMouse.GetClick(299, 279, 600, 340)) {
			loadGame = true;
		}
		g_htpB.Draw(300, 380);
		if (Event.RMouse.GetOn(299, 379, 600, 440)) {
			g_htpBOn.Draw(300, 380);
		}
		if (Event.LMouse.GetClick(299, 379, 600, 440)) {
			hFlag = true;
			htNum = 0;
		}
	}
	else {
		g_HtP[htNum].Draw(0, 0);
		if (htNum > 0) {
			g_AL.Draw(0, 250);
			if (Event.LMouse.GetClick(-1, 249, 50, 350)) {
				htNum--;
			}
		}
		if (htNum < HTP - 1) {
			g_AR.Draw(850, 250);
			if (Event.LMouse.GetClick(849, 249, 900, 350)) {
				htNum++;
			}
		}
		if (Event.LMouse.GetClick(859, -1, 900, 40)) {
			hFlag = false;
		}
	}
}
