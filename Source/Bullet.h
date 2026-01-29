#pragma once
#include "Object2D.h"
#include <vector>

class Bullet : public Object2D
{
public:
    Bullet(VECTOR2 pos, int dir);

    void Update() override;
    void Draw() override;

private:
    float speed;
    int direction; // -1:ç∂ / 1:âE
    bool isDead = false;
};

