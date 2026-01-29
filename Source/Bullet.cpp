#include "Bullet.h"
#include "Stage.h"
#include "Enemy.h"
#include <assert.h>

Bullet::Bullet(VECTOR2 pos, int dir)
{
    position = pos;
    direction = dir;
    speed = 8.0f;

    hImage = LoadGraph("data/image/player.png");
    assert(hImage > 0);

    imageSize = VECTOR2(32, 32);
    imageSize = VECTOR2(16, 16);
}

void Bullet::Update()
{
    position.x += speed * direction;

    // ï«Ç…ìñÇΩÇ¡ÇΩÇÁè¡Ç∑
    Stage* st = FindGameObject<Stage>();
    if (st && st->IsWall(position))
    {
        isDead = true;
        return;
    }

    // ìGÇ…ìñÇΩÇ¡ÇΩÇÁÉ_ÉÅÅ[ÉW
    Enemy* enemy = FindGameObject<Enemy>();
    if (enemy && !enemy->isDead())
    {
        VECTOR2 epos = enemy->GetPosition();
        if (fabs(position.x - epos.x) < 24 &&
            fabs(position.y - epos.y) < 24)
        {
            enemy->Damage(3);
            isDead = true;
        }
    }
}

void Bullet::Draw()
{
    Object2D::Draw();
}
