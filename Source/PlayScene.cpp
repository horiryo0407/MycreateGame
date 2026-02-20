#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"

PlayScene::PlayScene()
    : stage_(nullptr)
{
    stage_ = new Stage();
}

PlayScene::~PlayScene()
{
    /*delete stage_;
    stage_ = nullptr;*/
}

void PlayScene::Update()
{
    Player* player = FindGameObject<Player>();
    Enemy* enemy = FindGameObject<Enemy>();

    //  Player死亡 → GAMEOVER
    if (player && player->isDead())
    {
        SceneManager::ChangeScene("GAMEOVER");
        return;
    }

    // Enemy死亡 → CLEAR
    if (enemy && enemy->isDead())
    {
        SceneManager::ChangeScene("CLEAR");
        return;
    }

    //  デバッグ用
    if (CheckHitKey(KEY_INPUT_T))
    {
        SceneManager::ChangeScene("TITLE");
        return;
    }
}

void PlayScene::Draw()
{
    if (stage_)
    {
        stage_->Draw();   // ← これが無いとステージは表示されない
    }
    DrawString(60, 0, "Player", GetColor(255, 255, 255));
}
