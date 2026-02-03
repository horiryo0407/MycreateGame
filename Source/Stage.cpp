#include "Stage.h"
#include <assert.h>
#include "Player.h"
#include "Enemy.h"
#include "CsvReader.h"
#include "Screen.h"
#include <DxLib.h>

// ------------------------------
// 便利関数
// ------------------------------
static int LerpInt(int a, int b, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (int)(a + (b - a) * t);
}

// ------------------------------
// 背景描画
// ------------------------------
static void DrawFightBackground()
{
    const int w = Screen::WIDTH;
    const int h = Screen::HEIGHT;

    const int topR = 255, topG = 196, topB = 205;
    const int botR = 180, botG = 60, botB = 80;

    for (int y = 0; y < h; y += 4) {
        float t = (float)y / (float)h;
        int r = LerpInt(topR, botR, t);
        int g = LerpInt(topG, botG, t);
        int b = LerpInt(topB, botB, t);
        DrawBox(0, y, w, y + 4, GetColor(r, g, b), TRUE);
    }
}

// ------------------------------
// Stage
// ------------------------------
Stage::Stage()
{
    bgmHandle = LoadSoundMem("data/image/hori.mp3");
    assert(bgmHandle != -1);

    // マップ読み込み
    CsvReader* csv = new CsvReader("data/map/stage0.csv");
    for (int y = 0; y < csv->GetLines(); y++) {
        std::vector<int> line;
        for (int x = 0; x < csv->GetColumns(y); x++) {
            line.push_back(csv->GetInt(y, x));
        }
        map.push_back(line);
    }
    delete csv;

    imageSize = VECTOR2(64, 64);

    hToothImage = LoadGraph("data/image/ha.png");
    ReversehToothImage = LoadGraph("data/image/sakasanoha.png");
    assert(hToothImage > 0);
    assert(ReversehToothImage > 0);

    // 天井
    ceilingY = -1;
    ceilingSpeed = 0.05f;   // ← 迫る速さ

    // オブジェクト生成
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            int c = map[y][x];
            int px = x * imageSize.x + imageSize.x / 2;
            int py = y * imageSize.y + imageSize.y / 2;

            if (c == 9) {
                new Player(VECTOR2(px, py));
            }
            else if (c == 7) {
                new Enemy(VECTOR2(px, py));
            }
            else if (c == 4 && ceilingY < 0) {
                // 最上段の4を天井開始位置に
                ceilingY = y * imageSize.y;
            }
        }
    }

    startTimer = 180;
    flashTimer = 0;
    isStarted = false;
    isHoriPlaying = false;
    timerFont = CreateFontToHandle(nullptr, 64, 3);

    //PlaySoundMem(bgmHandle, DX_PLAYTYPE_LOOP);
}

Stage::~Stage()
{
    StopSoundMem(bgmHandle);
    DeleteFontToHandle(timerFont);
}

// ------------------------------
// Update
// ------------------------------
void Stage::Update()
{
    if (!isStarted) {
        startTimer--;
        if (startTimer <= 0) {
            isStarted = true;
        }
        return;
    }

    // 天井を下げる
    if (ceilingY >= 0) {
        ceilingY += ceilingSpeed;
    }

    Player* pl = FindGameObject<Player>();
    if (pl && ceilingY >= 0)
    {
        float distance =
            (pl->GetPosition().y - 32) - (ceilingY + imageSize.y);

        // hori 再生開始
        if (distance < 45 && !isHoriPlaying)
        {
            PlaySoundMem(bgmHandle, DX_PLAYTYPE_LOOP);
            isHoriPlaying = true;
        }
    }

    // ★ hori が鳴っている間ずっとカウント
    if (isHoriPlaying)
    {
        flashTimer++;
    }

    // 圧死判定
    if (pl) {
        float headY = pl->GetPosition().y - 32;
        if (headY <= ceilingY + imageSize.y) {
            pl->Damage(9999);
        }
    }

    Enemy* en = FindGameObject<Enemy>();
    if (en && !en->isDead()) {
        float headY = en->GetPosition().y - 32;
        if (headY <= ceilingY + imageSize.y) {
            en->Kill();
        }
    }
}


// ------------------------------
// Draw
// ------------------------------
void Stage::Draw()
{
    DrawFightBackground();

    if (!isStarted) {
        int sec = startTimer / 60 + 1;
        DrawFormatStringToHandle(
            600, 300,
            GetColor(255, 255, 255),
            timerFont,
            "%d",
            sec
        );
    }

    int w = imageSize.x;
    int h = imageSize.y;

    // マップ描画
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            int c = map[y][x];
            if (c == 1 || c == 2) {
                DrawRectGraph(x * w, y * h, 5 * w, 0, w, h, hToothImage, TRUE);
            }
            else if (c == 3) {
                DrawGraph(x * w, y * h, hToothImage, TRUE);
            }
        }
    }

    // 天井描画
    if (ceilingY >= 0) {
        for (int x = 0; x < Screen::WIDTH; x += w) {
            DrawGraph(x, (int)ceilingY, ReversehToothImage, TRUE);
        }
    }

    // ★ hori 中は画面をちかちか
    if (isHoriPlaying)
    {
        // sin波で 0〜1 を往復
        float t = (sinf(flashTimer * 0.08f) + 1.0f) * 0.5f;

        int alpha = (int)(t * 60);   // 強さ調整
        if (alpha > 100) alpha = 100;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT,
            GetColor(255, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

// ------------------------------
// 判定系
// ------------------------------
bool Stage::IsBlock(int px, int py)
{
    int tx = px / imageSize.x;
    int ty = py / imageSize.y;

    if (ty < 0 || ty >= map.size()) return false;
    if (tx < 0 || tx >= map[ty].size()) return false;

    int c = map[ty][tx];
    return (c == 1 || c == 2 || c == 3 || c ==4);
}

bool Stage::IsWall(VECTOR2 pos)
{
    int x = pos.x / imageSize.x;
    int y = pos.y / imageSize.y;

    if (y < 0 || y >= map.size()) return false;
    if (x < 0 || x >= map[y].size()) return false;

    int c = map[y][x];
    return (c == 1 || c == 2 || c == 3);
}

int Stage::CheckRight(VECTOR2 pos)
{
    if (!IsWall(pos)) return 0;
    int x = pos.x / imageSize.x;
    int dx = pos.x - x * imageSize.x;
    return dx + 1;
}

int Stage::CheckLeft(VECTOR2 pos)
{
    if (!IsWall(pos)) return 0;
    int x = pos.x / imageSize.x;
    int dx = pos.x - x * imageSize.x;
    return imageSize.x - dx;
}

int Stage::CheckDown(VECTOR2 pos)
{
    if (!IsWall(pos)) return 0;
    int y = pos.y / imageSize.y;
    int dy = pos.y - y * imageSize.y;
    return dy + 1;
}

int Stage::CheckUp(VECTOR2 pos)
{
    if (!IsWall(pos)) return 0;
    int y = pos.y / imageSize.y;
    int dy = pos.y - y * imageSize.y;
    return imageSize.y - dy;
}
