#include"Suken.h"
#include"MyScene.h"

void suken::Awake(){
	SetClass();
}

void suken::GameLoopEnter(){
	DrawGame();
}

void suken::GameLoopExit(){

}