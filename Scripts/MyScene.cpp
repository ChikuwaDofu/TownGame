#include"MyScene.h"
#include"TileM.h"
#include"Title.h"
#include"HowTo.h"
#include"Result.h"

CTileManager tileM;
CTitle title;
CHowTo how;
CResult result;

bool game = false;
bool res = false;

void SetClass(){
	title.Set();
	tileM.Set();
	how.Set();
	result.Set();
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
			if (tileM.end) {
				res = true;
				game = false;
				tileM.end = false;
				result.data = tileM.endD;
				tileM.Set();
			}
		}
	}
	else if (res) {
		result.Draw();

		if (result.back) {
			res = false;
			result.back = false;
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