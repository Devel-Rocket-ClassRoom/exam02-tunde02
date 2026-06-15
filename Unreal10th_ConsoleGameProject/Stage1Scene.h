#pragma once
#include "BaseScene.h"
#include <Windows.h>

class Stage1Scene : public BaseScene
{
public:
    Stage1Scene(int Width, int Height) : BaseScene(Width, Height) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    float StageTimer = 0.0f;
    bool bIsBossSpawned = false;
    bool bIsStageCleared = false;
    GameObject* BossMonster = nullptr;

    const int MenuCount = 2;
    int CurrentMenuIndex = 0;
    KeyPressState Keys[3] = {
        { VK_UP, false },
        { VK_DOWN, false },
        { VK_RETURN, false }
    };

    void ProcessKeyPress(int InKeyCode);
    void RenderStatus();
    void RenderStageClearMenu();
};
