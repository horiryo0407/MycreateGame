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
    void DrawUI();
    void Damage(int value);
    void Kill();
    bool isDead() const;

private:
    int hp;
    int maxHp;

    int damageTimer;
    int attackTimer;
    int jumpTimer;

    float velocityY;
    bool onGround;
    
    float Gravity;
    float JumpHeight;
    float JumpV0;

    float moveSpeed;
    int dir;

    bool isDead_;
};
