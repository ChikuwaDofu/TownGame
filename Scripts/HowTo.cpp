#include"DxLib.h"
#include"Suken.h"
#include"HowTo.h"

CPicture CHowTo::g_HtP[HT];
CPicture CHowTo::g_AR;
CPicture CHowTo::g_AL;

void CHowTo::Set() {
	htNum = 0;
	end = false;

	g_HtP[0].Load("Chikuwa3/HtPF.png");
	g_HtP[1].Load("Chikuwa3/HtPTu.png");
	g_HtP[2].Load("Chikuwa3/HtPR.png");
	g_HtP[3].Load("Chikuwa3/HtPBT.png");
	g_HtP[4].Load("Chikuwa3/HtPTe.png");
	g_HtP[5].Load("Chikuwa3/HtPT1.png");
	g_HtP[6].Load("Chikuwa3/HtPT2.png");
	g_HtP[7].Load("Chikuwa3/HtPTi.png");
	g_HtP[8].Load("Chikuwa3/HtPI.png");
	g_AR.Load("Chikuwa3/ArrowR.png");
	g_AL.Load("Chikuwa3/ArrowL.png");
}

void CHowTo::Reset() {
	htNum = 0;
	end = false;
}

void CHowTo::Draw() {
	g_HtP[htNum].Draw(0, 0);
	if (htNum > 0) {
		g_AL.Draw(0, 250);
		if (Event.LMouse.GetClick(-1, 249, 50, 350)) {
			htNum--;
		}
	}
	if (htNum < HT - 1) {
		g_AR.Draw(850, 250);
		if (Event.LMouse.GetClick(849, 249, 900, 350)) {
			htNum++;
		}
	}
	if (Event.LMouse.GetClick(859, -1, 900, 40)) {
		end = true;
	}
}