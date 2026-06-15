#pragma once
#include "BaseScene.h"
#include "Monster.h"
#include <Windows.h>
#include <vector>

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
    bool bIsStageFinished = false;
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

    inline bool IsPlayerAlive()
    {
        return std::find_if(
            SceneObjects.begin(),
            SceneObjects.end(),
            [](GameObject* Obj)
            {
                return Obj->GetCollisionLayer() == CollisionLayer::Player;
            }) != SceneObjects.end();
    }
    inline bool IsBossAlive()
    {
        return std::find_if(
            SceneObjects.begin(),
            SceneObjects.end(),
            [](GameObject* Obj)
            {
                return (Obj->GetCollisionLayer() == CollisionLayer::Monster)
                    && static_cast<Monster*>(Obj)->GetMonsterType() == MonsterType::Boss;
            }) != SceneObjects.end();
    }
};
