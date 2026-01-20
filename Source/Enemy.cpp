#include "Enemy.h"
#include "Stage.h"
#include "Player.h"
#include "CsvReader.h"
#include <DxLib.h>

Enemy::Enemy() : Enemy(VECTOR2(300, 200))
{
}

Enemy::Enemy(VECTOR2 pos)
{
    if (Gravity <= 0) Gravity = 0.6f;
    if (JumpHeight <= 0) JumpHeight = 64;
    if (moveSpeed <= 0) moveSpeed = 2;

    hImage = LoadGraph("data/image/tamadot.png");
    JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);

    imageSize = VECTOR2(64, 64);

    position = pos;

    maxHp = 50;
    hp = 50;

    damageTimer = 0;
    attackTimer = 0;
    jumpTimer = 60;

    velocityY = 0.0f;
    onGround = false;
    isDead = false;
    dir = -1;

    isDead = false;
    seDie = LoadSoundMem("Sound/メニューを開く3.mp3");
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    if (isDead) return;
    Stage* st = FindGameObject<Stage>();
    Player* pl = FindGameObject<Player>();
    if (!st) return;

    if (hp <= 0)
    {
        Kill();
        return;
    }

    // ===== プレイヤー方向を見る =====
    if (pl)
    {
        dir = (pl->GetPosition().x > position.x) ? 1 : -1;
    }

    // ===== 横移動 =====
    position.x += moveSpeed * dir;

    // ===== ステージ端で止める =====
    const int HALF = 32;

    int leftLimit = HALF;
    int rightLimit = st->GetMapWidth() - HALF;

    if (position.x < leftLimit)
        position.x = leftLimit;
    else if (position.x > rightLimit)
        position.x = rightLimit;

    // ===== ランダムジャンプ =====
    if (jumpTimer > 0)
        jumpTimer--;

    if (onGround && jumpTimer <= 0)
    {
        velocityY = JumpV0;
        onGround = false;
        jumpTimer = 60 + GetRand(120);
    }

    // ===== 壁前ジャンプ =====
    int frontX = position.x + dir * HALF;
    int frontY = position.y;

    bool frontWall = st->IsBlock(frontX, frontY);
    bool ground = st->IsBlock(position.x, position.y + 33);

    if (ground && frontWall)
    {
        velocityY = JumpV0;
        onGround = false;
    }

    // ===== 重力 =====
    position.y += velocityY;
    velocityY += Gravity;
    onGround = false;

    if (st->IsBlock(position.x, position.y + 32))
    {
        velocityY = 0;
        onGround = true;
        position.y = (int(position.y / 64) * 64) + 32;
    }

    // ===== タイマー =====
    if (damageTimer > 0) damageTimer--;
    if (attackTimer > 0) attackTimer--;

    // ===== 攻撃 =====
    if (attackTimer == 0 && pl)
    {
        VECTOR2 p = pl->GetPosition();
        if (fabs(position.x - p.x) < 40 &&
            fabs(position.y - p.y) < 40)
        {
            pl->Damage(5);
            attackTimer = 60;
        }
    }
}

void Enemy::Draw()
{
    if (isDead) return;

    Object2D::Draw();

    DrawBox(position.x - 24, position.y - 32,
        position.x + 24, position.y + 32,
        GetColor(0, 0, 255), FALSE);

    DrawUI();
}

void Enemy::DrawUI()
{
    int w = 200;
    int h = 20;

    int x = 1280 - w - 20;
    int y = 20;

    float rate = (float)hp / maxHp;
    int curW = (int)(w * rate);

    DrawBox(x - 1, y - 1, x + w + 1, y + h + 1,
        GetColor(0, 0, 0), FALSE);

    DrawBox(x, y, x + curW, y + h,
        GetColor(255, 0, 0), TRUE);

    DrawFormatString(x, y + 24, GetColor(255, 255, 255),
        "ENEMY %d / %d", hp, maxHp);
}

void Enemy::Damage(int value)
{
    if (isDead) return;

    hp -= value;

    if (hp <= 0)
    {
        hp = 0;
        isDead = true;
        PlaySoundMem(seDie, DX_PLAYTYPE_BACK);
    }
}

