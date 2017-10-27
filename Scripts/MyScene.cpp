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
bool start = true;

void SetClass(){
	title.Set();
	tileM.Set();
	how.Set();
	result.Set();
}

void DrawGame(){
	if (start) {
		music->Loop(sbTitle);
	}
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
				music->StopLoop(tileM.bgm);
				game = false;
				tileM.back = false;
				tileM.Set();
				music->Loop(sbTitle);
			}
			if (tileM.end) {
				music->StopLoop(tileM.bgm);
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
			music->Loop(sbTitle);
		}
	}
	else {
		title.Draw();

		if (title.newGame) {
			music->StopLoop(sbTitle);
			game = true;
			title.newGame = false;
			music->Loop(tileM.bgm);
		}
		if (title.loadGame) {
			music->StopLoop(sbTitle);
			game = true;
			title.loadGame = false;
			tileM.Load();
			music->Loop(tileM.bgm);
		}
	}
	start = false;
}