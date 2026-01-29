#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include "Enemy.h"
#include "Bullet.h"
#include "../ImGui/imgui.h"
#include "CsvReader.h"

Player::Player() : Player(VECTOR2(100, 200))
{
}

Player::Player(VECTOR2 pos)
{
    // パラメーター読込
    CsvReader* csv = new CsvReader("data/playerParam.csv");
    for (int i = 0; i < csv->GetLines(); i++) {
        std::string tag = csv->GetString(i, 0);
        if (tag == "Gravity") {
            Gravity = csv->GetFloat(i, 1);
        }
        else if (tag == "JumpHeight") {
            JumpHeight = csv->GetFloat(i, 1);
        }
        else if (tag == "MoveSpeed") {
            moveSpeed = csv->GetFloat(i, 1);
        }
    }
    JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);

    hImage = LoadGraph("data/image/tamadot.png");
    assert(hImage > 0);

    imageSize = VECTOR2(64, 64);
    anim = 0;
    animY = 3;

    position = pos;
    velocityY = 0.0f;
    attackTimer = 0;
    shotTimer = 0;          // ★追加
    maxHp = 10;
    hp = 10;

    prevAttack = false;
    prevPushed = false;
    onGround = false;
    damageTimer = 0;

    isDead_ = false;
    dir = 1;
}

Player::~Player()
{
}

void Player::Update()
{
    if (hp <= 0)
    {
        isDead_ = true;
        return;
    }

    Stage* st = FindGameObject<Stage>();
    if (!st) return;

    // --- 横移動 ---
    if (CheckHitKey(KEY_INPUT_D)) {
        dir =1; // ★向き更新
        animY = 3;
        position.x += moveSpeed;
        int push = st->CheckRight(position + VECTOR2(24, -31));
        position.x -= push;
        push = st->CheckRight(position + VECTOR2(24, 31));
        position.x -= push;
    }
    if (CheckHitKey(KEY_INPUT_A)) {
        dir = -1;                // ★向き更新
        animY = 1;               // ★左向き行
        position.x -= moveSpeed;

        int push = st->CheckLeft(position + VECTOR2(-24, -31));
        position.x += push;
        push = st->CheckLeft(position + VECTOR2(-24, 31));
        position.x += push;
    }


    // ===== ステージ端制限 =====
    const int HALF = 24;
    int leftLimit = HALF;
    int rightLimit = st->GetMapWidth() - HALF;

    if (position.x < leftLimit)
        position.x = leftLimit;
    else if (position.x > rightLimit)
        position.x = rightLimit;

    // --- ジャンプ ---
    if (onGround) {
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            if (!prevPushed) {
                velocityY = JumpV0;
            }
            prevPushed = true;
        }
        else {
            prevPushed = false;
        }
    }

    // --- 重力 ---
    position.y += velocityY;
    velocityY += Gravity;
    onGround = false;

    if (velocityY < 0.0f) {
        int push = st->CheckUp(position + VECTOR2(-24, -31));
        if (push > 0) {
            velocityY = 0.0f;
            position.y += push;
        }
        push = st->CheckUp(position + VECTOR2(24, -31));
        if (push > 0) {
            velocityY = 0.0f;
            position.y += push;
        }
    }
    else {
        int push = st->CheckDown(position + VECTOR2(-24, 32));
        if (push > 0) {
            velocityY = 0.0f;
            onGround = true;
            position.y -= push - 1;
        }
        push = st->CheckDown(position + VECTOR2(24, 32));
        if (push > 0) {
            velocityY = 0.0f;
            onGround = true;
            position.y -= push - 1;
        }
    }

    // --- ダメージ無敵 ---
    if (damageTimer > 0)
        damageTimer--;

    // --- 近接攻撃 ---
    bool nowAttack = CheckHitKey(KEY_INPUT_H);

    if (nowAttack && !prevAttack && attackTimer == 0)
    {
        Enemy* enemy = FindGameObject<Enemy>();
        if (enemy && !enemy->isDead())
        {
            VECTOR2 epos = enemy->GetPosition();
            if (fabs(position.x - epos.x) < 60 &&
                fabs(position.y - epos.y) < 60)
            {
                enemy->Damage(5);
                attackTimer = 30;
            }
        }
    }

    prevAttack = nowAttack;

    if (attackTimer > 0)
        attackTimer--;

    // --- 弾発射 ★追加 ---
    if (shotTimer > 0)
        shotTimer--;

    if (CheckHitKey(KEY_INPUT_J) && shotTimer == 0)
    {
        VECTOR2 bulletPos = position;
        bulletPos.x += dir * 30;
        bulletPos.y -= 10;

        new Bullet(bulletPos, dir);


        shotTimer = 20;
    }

    // --- Enemy 押し出し ---
    Enemy* enemy = FindGameObject<Enemy>();
    if (enemy && !enemy->isDead())
    {
        VECTOR2 epos = enemy->GetPosition();
        float dx = position.x - epos.x;
        float dy = position.y - epos.y;

        if (fabs(dx) < 48 && fabs(dy) < 40)
        {
            float push = 48 - fabs(dx);
            if (dx > 0)
                position.x += push;
            else
                position.x -= push;

            if (position.x < leftLimit)
                position.x = leftLimit;
            else if (position.x > rightLimit)
                position.x = rightLimit;
        }
    }
}

void Player::Draw()
{
    Object2D::Draw();

    DrawBox(position.x - 24, position.y - 32,
        position.x + 24, position.y + 32,
        GetColor(255, 0, 0), FALSE);

    DrawUI();
}

void Player::DrawUI()
{
    int x = 20;
    int y = 20;
    int w = 200;
    int h = 20;

    float rate = (float)hp / maxHp;
    int curW = (int)(w * rate);

    DrawBox(x - 1, y - 1, x + w + 1, y + h + 1,
        GetColor(0, 0, 0), FALSE);

    DrawBox(x, y, x + curW, y + h,
        GetColor(255, 0, 0), TRUE);

    DrawFormatString(x, y + 24, GetColor(255, 255, 255),
        "%d / %d", hp, maxHp);
}

void Player::Damage(int value)
{
    hp -= value;
    damageTimer = 60;
}

bool Player::isDead() const
{
    return isDead_;
}
