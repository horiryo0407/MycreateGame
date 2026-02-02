#include "Enemy.h"
#include "Stage.h"
#include "Player.h"
#include "Bullet.h"
#include "CsvReader.h"
#include <DxLib.h>

Enemy::Enemy() : Enemy(VECTOR2(300, 200))
{
}

Enemy::Enemy(VECTOR2 pos)
{
    if (Gravity <= 0) Gravity = 0.5f;
    if (JumpHeight <= 0) JumpHeight = 192;
    if (moveSpeed <= 0) moveSpeed = 3.5;

    hImage = LoadGraph("data/image/tamadot.png");
    JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);

    imageSize = VECTOR2(64, 64);

    position = pos;

    maxHp = 50;
    hp = 50;
    anim = 0;
    animY = 1;

    damageTimer = 0;
    attackTimer = 0;
    shotTimer = 0;
    jumpTimer = 60;

    velocityY = 0.0f;
    onGround = false;
    isDead_ = false;
    dir = -1;

    
    //Die = LoadSoundMem("Sound/メニューを開く3.mp3");
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    Stage* st = FindGameObject<Stage>();
    if (st && !st->IsStarted()) return;
    if (isDead_) return;
    //Stage* st = FindGameObject<Stage>();
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
        float dx = pl->GetPosition().x - position.x;

        const float FACE_THRESHOLD = 12.0f; // ★重要

        if (dx > FACE_THRESHOLD)
        {
            dir = 1;
            animY = 3;   // 右向き
        }
        else if (dx < -FACE_THRESHOLD)
        {
            dir = -1;
            animY = 1;   // 左向き
        }
        // ★それ以外（ほぼ同じx）は向きを固定
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


    // ===== 弾発射タイマー =====
    if (shotTimer > 0)
        shotTimer--;

    // ===== 弾を撃つ =====
    if (shotTimer == 0 && pl)
    {
        // プレイヤーとの距離チェック（撃ちすぎ防止）
        VECTOR2 p = pl->GetPosition();
        float dx = fabs(position.x - p.x);
        float dy = fabs(position.y - p.y);

        if (dx < 400 && dy < 100) // 射程
        {
            VECTOR2 bulletPos = position;
            bulletPos.y -= 10;          // 口あたり
            bulletPos.x += dir * 30;    // 体の前

            Bullet* b = new Bullet(bulletPos,dir); // ★敵の弾
            b->isEnemyBullet = true;
            shotTimer = 90; // 1.5秒に1発（60fps想定）
        }
    }

    //// ===== 攻撃 =====
    //if (attackTimer == 0 && pl)
    //{
    //    VECTOR2 p = pl->GetPosition();
    //    if (fabs(position.x - p.x) < 40 &&
    //        fabs(position.y - p.y) < 40)
    //    {
    //        pl->Damage(5);
    //        attackTimer = 60;
    //    }
    //}
}

void Enemy::Draw()
{
    if (isDead_) return;

   /* Object2D::Draw();

    DrawBox(position.x - 24, position.y - 32,
        position.x + 24, position.y + 32,
        GetColor(0, 0, 255), FALSE);*/
    DrawRectGraph(
        position.x - 32,
        position.y - 32,
        anim * 64,
        animY * 64,
        64, 64,
        hImage,
        TRUE
    );

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

bool Enemy::isDead() const
{
    return isDead_;
}


void Enemy::Damage(int value)
{
    if (isDead_) return;

    hp -= value;

    if (hp <= 0)
    {
        hp = 0;
        isDead_ = true;
        //PlaySoundMem(seDie, DX_PLAYTYPE_BACK);
    }
}

void Enemy::Kill()
{
    isDead_ = true;
}
