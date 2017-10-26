#include"DxLib.h"
#include"Suken.h"
#include"Music.h"

CMusic *music;

CMusic::CMusic() {
	memset(sFlag, false, sEnd);
	memset(loopFlag, false, sEnd);

	sound[sFound] = LoadSoundMem("Chikuwa3/Found.mp3");
	sound[sDev] = LoadSoundMem("Chikuwa3/Develop.mp3");
	sound[sBuild] = LoadSoundMem("Chikuwa3/Building.mp3");
	sound[sBuy] = LoadSoundMem("Chikuwa3/Purchase.mp3");
}

void CMusic::AddSound(Sounds sound) {
	sFlag[sound] = true;
}

void CMusic::Loop(Sounds sound) {
	if (loopFlag[sound] == false) {
		PlaySoundMem(this->sound[sound], DX_PLAYTYPE_LOOP);
		loopFlag[sound] = true;
	}
}

void CMusic::StopLoop(Sounds sound) {
	if (loopFlag[sound] == true) {
		StopSoundMem(this->sound[sound]);
		loopFlag[sound] = false;
	}
}

void CMusic::Play() {
	for (int i = 0; i< sEnd; i++) {
		if (sFlag[i] == true) {
			PlaySoundMem(sound[i], DX_PLAYTYPE_BACK);
		}
		sFlag[i] = false;
	}

}

void AwakeMusic() {
	music = new CMusic();
}