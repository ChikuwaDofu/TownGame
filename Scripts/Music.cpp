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
	sound[sTDemo] = LoadSoundMem("Chikuwa3/TDem.mp3");
	sound[sBDemo] = LoadSoundMem("Chikuwa3/BDem.mp3");
	sound[sSABuild] = LoadSoundMem("Chikuwa3/SaBuild.mp3");
	sound[sCut] = LoadSoundMem("Chikuwa3/Cut.mp3");
	sound[sSerSuc] = LoadSoundMem("Chikuwa3/SeSuc.mp3");
	sound[sSerFail] = LoadSoundMem("Chikuwa3/SeFail.mp3");
	sound[sTool] = LoadSoundMem("Chikuwa3/Tool.wav");
	sound[sChar] = LoadSoundMem("Chikuwa3/CC.mp3");
	sound[sEndUse] = LoadSoundMem("Chikuwa3/EndUse.mp3");
	sound[sBuy] = LoadSoundMem("Chikuwa3/Purchase.mp3");
	sound[sbTitle] = LoadSoundMem("Chikuwa3/BGM_Ti.mp3");
	sound[sbNormal] = LoadSoundMem("Chikuwa3/BGM_No.mp3");
	sound[sbCastle] = LoadSoundMem("Chikuwa3/BGM_Ca.mp3");
	sound[sbAgri] = LoadSoundMem("Chikuwa3/BGM_Ag.mp3");
	sound[sbWood] = LoadSoundMem("Chikuwa3/BGM_Wo.mp3");
	sound[sbStone] = LoadSoundMem("Chikuwa3/BGM_St.mp3");
	sound[sbComm] = LoadSoundMem("Chikuwa3/BGM_Cm.mp3");
	sound[sbPasture] = LoadSoundMem("Chikuwa3/BGM_Pa.mp3");
	sound[sbChar] = LoadSoundMem("Chikuwa3/BGM_CC.mp3");
	sound[sbFactory] = LoadSoundMem("Chikuwa3/BGM_Fc.mp3");
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