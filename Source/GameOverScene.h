#pragma once
#include "../Library/SceneBase.h"

class GameOverScene : public SceneBase
{
public:
    GameOverScene();
    ~GameOverScene();

    void Update() override;
    void Draw() override;

private:
    int titleFont;
    int infoFont;
    int frame;
    int titleFontBig;
};