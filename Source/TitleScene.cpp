#include "TitleScene.h"
#include <assert.h>
#include <DxLib.h>

TitleScene::TitleScene()
{
	hTitleImage = LoadGraph("data/image/haTitle.png");
	assert(hTitleImage > 0);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, hTitleImage, TRUE);
	//DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	//DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
}
