#pragma once
#include "Object2D.h"
#include <vector>

class Stage : public Object2D {
public:
	Stage();
	~Stage();

	// 描画・更新
	void Draw() override;
	void Update() override;

	// スクロール
	float ScrollX() { return scrollX; }
	void SetScrollX(int sc) { scrollX = sc; }

	// ───────────────
	// 当たり判定系
	// ───────────────

	// 右・左・下・上の押し戻し量
	int CheckRight(VECTOR2 pos);
	int CheckLeft(VECTOR2 pos);
	int CheckDown(VECTOR2 pos);
	int CheckUp(VECTOR2 pos);

	// マップ情報
	int GetMapWidth() const { return map[0].size() * imageSize.x; }
	bool IsBlock(int px, int py);
	int GetBlock(int x, int y);
	bool IsWall(VECTOR2 pos);

	// ゲーム開始判定
	bool IsStarted() const { return isStarted; }

	// ───────────────
	// ★ 天井（圧死）関連 ★
	// ───────────────

	// 天井のY座標（px）
	int GetCeilingY() const { return ceilingY; }

	// 指定座標が圧死しているか
	bool IsCrushed(VECTOR2 pos) const;

private:
	// ───────────────
	// スクロール
	// ───────────────
	float scrollX;

	// ───────────────
	// 画像・音
	// ───────────────
	int hBgImage;
	int bgHandle;
	int bgmHandle;
	int hToothImage;
	int ReversehToothImage;

	// ───────────────
	// ゲーム開始演出
	// ───────────────
	int startTimer;   // 開始前カウントダウン
	bool isStarted;
	bool isHoriPlaying;
	int timerFont;
	int flashTimer;

	// ───────────────
	// マップ
	// ───────────────
	std::vector<std::vector<int>> map;

	// ───────────────
	// ★ 天井制御 ★
	// ───────────────
	float ceilingY;        // 天井の現在Y座標（px）
	float ceilingSpeed;   // 1フレームで下がる量
};
