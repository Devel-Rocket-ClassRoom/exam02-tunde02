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
    float StageTimer = 0.0f; // 스테이지가 진행되는 동안 계속 쌓이는 타이머. 보스 몬스터 소환 시 사용
    bool bIsBossSpawned = false; // 보스 몬스터가 소환 됐는지 여부
    bool bIsStageFinished = false; // 스테이지가 종료되었는지 여부

    const int MenuCount = 2;
    int CurrentMenuIndex = 0;
    KeyPressState Keys[3] = {
        { VK_UP, false },
        { VK_DOWN, false },
        { VK_RETURN, false }
    };

    void ProcessKeyPress(int InKeyCode);

    /// <summary>
    /// 현재 스테이지, 점수, 플레이어에 대한 정보를 Screen에 렌더링하는 함수
    /// </summary>
    void RenderStatus();

    /// <summary>
    /// 스테이지가 종료됐을 때 알맞은 메뉴 창을 Screen에 렌더링하는 함수
    /// </summary>
    void RenderStageClearMenu();

    /// <summary>
    /// 현재 씬의 플레이어가 살아 있는지 여부를 반환하는 함수
    /// </summary>
    /// <returns>플레이어가 살아 있다면 true, 아니라면 false</returns>
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

    /// <summary>
    /// 현재 씬의 보스 몬스터가 살아 있는지 여부를 반환하는 함수
    /// </summary>
    /// <returns>보스 몬스터가 살아 있다면 true, 아니라면 false</returns>
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
