#include"MyScene.h"
#include"TileM.h"
#include"Title.h"
#include"HowTo.h"

CTileManager tileM;
CTitle title;
CHowTo how;

bool game = false;

void SetClass(){
	title.Set();
	tileM.Set();
	how.Set();
}

void DrawGame(){
	if (game) {
		if (tileM.howTo) {
			how.Draw();
			if (how.end) {
				tileM.howTo = false;
			}
		}
		else {
			tileM.Draw();
			if (tileM.howTo) {
				how.Reset();
			}
			if (tileM.back) {
				game = false;
				tileM.back = false;
				tileM.Set();
			}
		}
	}
	else {
		title.Draw();

		if (title.newGame) {
			game = true;
			title.newGame = false;
		}
		if (title.loadGame) {
			game = true;
			title.loadGame = false;
			tileM.Load();
		}
	}
}