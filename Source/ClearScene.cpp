#include "ClearScene.h"
#include <DxLib.h>
#include "Screen.h"

static void DrawMiniTooth(int x, int y, int w, int h, int fill, int outline)
{
    DrawBox(x, y, x + w, y + h, fill, TRUE);
    DrawBox(x, y, x + w, y + h, outline, FALSE);
    DrawLine(x + w / 3, y + h / 4, x + w / 3, y + h - h / 6, outline);
    DrawLine(x + 2 * w / 3, y + h / 4, x + 2 * w / 3, y + h - h / 6, outline);
}

ClearScene::ClearScene()
{
    titleFont = CreateFontToHandle(nullptr, 72, 6);
    infoFont = CreateFontToHandle(nullptr, 24, 2);
}

ClearScene::~ClearScene()
{
    DeleteFontToHandle(titleFont);
    DeleteFontToHandle(infoFont);
}

void ClearScene::Update()
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

void ClearScene::Draw()
{
    DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(20, 120, 90), TRUE);
    DrawBox(0, Screen::HEIGHT - 120, Screen::WIDTH, Screen::HEIGHT, GetColor(10, 70, 60), TRUE);

    int fill = GetColor(250, 250, 245);
    int outline = GetColor(180, 170, 160);
    for (int x = 40; x < Screen::WIDTH; x += 90) {
        DrawMiniTooth(x, 40, 48, 56, fill, outline);
    }

    DrawFormatStringToHandle(420, 240, GetColor(255, 255, 255), titleFont, "CLEAR!");
    DrawFormatStringToHandle(380, 360, GetColor(240, 240, 240), infoFont, "P: RETRY   T: TITLE   ESC: EXIT");
}