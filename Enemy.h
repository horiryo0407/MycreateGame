#pragma once
#include "Object2D.h"

class Enemy : public Object2D
{
public:
    Enemy();
    Enemy(VECTOR2 pos);
    ~Enemy();

    void Update() override;
    void Draw() override;

    void Damage(int value);   // š ƒ_ƒ[ƒWˆ—

private:
    int hp;
    int maxHp;

    int damageTimer;          // š –³“GŠÔ
};


