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
	VECTOR2 ddrawSize;
	bool isDead() const;
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
	int shotTimer;
	int seAttack;
	int seHit;
	int dir;
	// ===== 弾数管理 =====
	int ammo;          // 現在弾数
	int maxAmmo;       // 最大弾数
	int reloadTimer;   // 回復用タイマー



	bool isDead_;

};