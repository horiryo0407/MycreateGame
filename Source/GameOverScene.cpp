#include "GameOverScene.h"
#include <DxLib.h>
#include "Screen.h"

//static void DrawCrackedTooth(int x, int y, int w, int h, int fill, int outline)
//{
//    DrawBox(x, y, x + w, y + h, fill, TRUE);
//    DrawBox(x, y, x + w, y + h, outline, FALSE);
//    DrawLine(x + w / 2, y + h / 6, x + w / 3, y + h - h / 6, outline);
//    DrawLine(x + w / 2, y + h / 2, x + 2 * w / 3, y + h - h / 4, outline);
//}

GameOverScene::GameOverScene()
{
    titleFont = CreateFontToHandle(nullptr, 72, 6);
    titleFontBig = CreateFontToHandle(nullptr, 80, 6); // ドクン用
    infoFont = CreateFontToHandle(nullptr, 24, 2);
    frame = 0;
}

GameOverScene::~GameOverScene()
{
    DeleteFontToHandle(titleFont);
    DeleteFontToHandle(titleFontBig);
    DeleteFontToHandle(infoFont);
}

void GameOverScene::Update()
{
    frame++;

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

    // ドクン判定
    float beat = sinf(frame * 0.15f);
    bool dokun = (beat > 0.85f);

    int offsetX = (int)(sinf(frame * 0.3f) * 2);
    int offsetY = (int)(cosf(frame * 0.25f) * 2);

    int x = Screen::WIDTH / 2 - 230 + offsetX;
    int y = Screen::HEIGHT / 2 - 140 + offsetY;

    // 大きいフォント
    int font = dokun ? titleFontBig : titleFont;

    DrawFormatStringToHandle(
        x+50,
        y,
        GetColor(255, 80, 80),
        font,
        "GAME OVER"
    );

    DrawFormatStringToHandle(
        Screen::WIDTH / 2 - 220,
        Screen::HEIGHT - 90,
        GetColor(220, 220, 220),
        infoFont,
        "P: RETRY   T: TITLE   ESC: EXIT"
    );
}

