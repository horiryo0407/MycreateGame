#pragma once
#include "Object2D.h"

class Player : public Object2D {
public:
	Player();
	Player(VECTOR2 pos);
	~Player();
	void Update() override;
	void Draw() override;
	void DrawUI();
	void Damage(int value);
private:
	float velocityY;
	bool onGround;
	bool prevPushed;
	bool prevAttack;

	float Gravity;
	float JumpHeight;
	float JumpV0;
	float moveSpeed;

	int hp;
	int maxHp;
	int damageTimer;
	int attackTimer;
	int seAttack;
	int seHit;


};