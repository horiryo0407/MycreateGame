#include "GameOverScene.h"
#include <DxLib.h>
#include "Screen.h"

static void DrawCrackedTooth(int x, int y, int w, int h, int fill, int outline)
{
    DrawBox(x, y, x + w, y + h, fill, TRUE);
    DrawBox(x, y, x + w, y + h, outline, FALSE);
    DrawLine(x + w / 2, y + h / 6, x + w / 3, y + h - h / 6, outline);
    DrawLine(x + w / 2, y + h / 2, x + 2 * w / 3, y + h - h / 4, outline);
}

GameOverScene::GameOverScene()
{
    titleFont = CreateFontToHandle(nullptr, 72, 6);
    infoFont = CreateFontToHandle(nullptr, 24, 2);
}

GameOverScene::~GameOverScene()
{
    DeleteFontToHandle(titleFont);
    DeleteFontToHandle(infoFont);
}

void GameOverScene::Update()
{
    if (CheckHitKey(KEY_INPUT_P)) {
        SceneManager::ChangeScene("PLAY");
    }
    if (CheckHitKey(KEY_INPUT_T)) {
        SceneManager::ChangeScene("TITLE");
    }
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        SceneManager::Exit();
    }
}

void GameOverScene::Draw()
{
    DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(30, 10, 15), TRUE);
    DrawBox(0, Screen::HEIGHT - 140, Screen::WIDTH, Screen::HEIGHT, GetColor(60, 10, 20), TRUE);

    int fill = GetColor(240, 240, 235);
    int outline = GetColor(150, 140, 130);
    DrawCrackedTooth(160, 120, 64, 72, fill, outline);
    DrawCrackedTooth(980, 160, 60, 68, fill, outline);
    DrawCrackedTooth(520, 460, 70, 78, fill, outline);

    DrawFormatStringToHandle(340, 240, GetColor(255, 80, 80), titleFont, "GAME OVER");
    DrawFormatStringToHandle(380, 360, GetColor(220, 220, 220), infoFont, "P: RETRY   T: TITLE   ESC: EXIT");
}