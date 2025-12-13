#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include "Enemy.h"
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
    maxHp = 100;
    hp = 100;

    prevAttack = false;
}

Player::~Player()
{
}

void Player::Update()
{
    Stage* st = FindGameObject<Stage>();

    // --- 横移動 ---
    if (CheckHitKey(KEY_INPUT_D)) {
        position.x += moveSpeed;
        int push = st->CheckRight(position + VECTOR2(24, -31));
        position.x -= push;
        push = st->CheckRight(position + VECTOR2(24, 31));
        position.x -= push;
    }
    if (CheckHitKey(KEY_INPUT_A)) {
        position.x -= moveSpeed;
        int push = st->CheckLeft(position + VECTOR2(-24, -31));
        position.x += push;
        push = st->CheckLeft(position + VECTOR2(-24, 31));
        position.x += push;
    }

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

    if (damageTimer > 0)
    {
        damageTimer--;
    }

    bool nowAttack = CheckHitKey(KEY_INPUT_H);

    if (nowAttack && !prevAttack && attackTimer == 0)
    {
        Enemy* enemy = FindGameObject<Enemy>();
        if (enemy)
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
    {
        attackTimer--;
    }

    Enemy* enemy = FindGameObject<Enemy>();
    if (enemy && !enemy->isDead)
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
        }
    }

    //ImGui::Begin("Player");
    //ImGui::Checkbox("onGround", &onGround);
    //ImGui::InputFloat("positionY", &position.y);
    //ImGui::End();
}

void Player::Draw()
{
    Object2D::Draw();

    // 本体描画（スクロールは外で行う前提）
    DrawBox(position.x - 24, position.y - 32,
        position.x + 24, position.y + 32,
        GetColor(255, 0, 0), FALSE);
    DrawUI();
}

//HPバー描画
void Player::DrawUI()
{
    int x = 20;
    int y = 20;
    int w = 200;
    int h = 20;

    float rate = (float)hp / maxHp;
    int curW = (int)(w * rate);

    // 枠
    DrawBox(x - 1, y - 1, x + w + 1, y + h + 1,
        GetColor(0, 0, 0), FALSE);

    // 中身
    DrawBox(x, y, x + curW, y + h,
        GetColor(255, 0, 0), TRUE);

    // 数字
    DrawFormatString(x, y + 24, GetColor(255, 255, 255),
        "%d / %d", hp, maxHp);

   
}

void Player::Damage(int value)
{

    hp -= value;
    damageTimer = 60;
}
