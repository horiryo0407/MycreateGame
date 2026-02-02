#include "Bullet.h"
#include "Stage.h"
#include "Enemy.h"
#include "Player.h"
#include <assert.h>

Bullet::Bullet(VECTOR2 pos, int dir)
{
    position = pos;
    direction = dir;
    speed = 8.0f;
    isEnemyBullet = false;

    hImage = LoadGraph("data/image/ha.png");
    assert(hImage > 0);

    imageSize = VECTOR2(128, 128);
    //imageSize = VECTOR2(16, 16);
}

void Bullet::Update()
{
    Stage* st = FindGameObject<Stage>();
    if (st && !st->IsStarted()) return;
    position.x += speed * direction;

    // ï«Ç…ìñÇΩÇ¡ÇΩÇÁè¡Ç∑
    //Stage* st = FindGameObject<Stage>();
    if (st && st->IsWall(position))
    {
        isDead = true;
        return;
    }

    if (!isEnemyBullet)
    {
        Enemy* enemy = FindGameObject<Enemy>();
        if (enemy && !enemy->isDead())
        {
            VECTOR2 epos = enemy->GetPosition();
            if (fabs(position.x - epos.x) < 24 &&
                fabs(position.y - epos.y) < 24)
            {
                enemy->Damage(3);
                isDead = true;
                return;
            }
        }
    }

    // ===== ìGíe Å® ÉvÉåÉCÉÑÅ[Ç…ìñÇΩÇÈ =====
    if (isEnemyBullet)
    {
        Player* pl = FindGameObject<Player>();
        if (pl)
        {
            VECTOR2 ppos = pl->GetPosition();
            if (fabs(position.x - ppos.x) < 24 &&
                fabs(position.y - ppos.y) < 24)
            {
                pl->Damage(1);
                isDead = true;
                return;
            }
        }
    }
}

void Bullet::Draw()
{
    Object2D::Draw();
}
