#pragma once
#include "../Library/SceneBase.h"

class Stage;

class PlayScene : public SceneBase
{
public:
    PlayScene();
    ~PlayScene();

    void Update() override;
    void Draw() override;

private:
    Stage* stage_;
};
