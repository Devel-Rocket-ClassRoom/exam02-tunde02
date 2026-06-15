#pragma once
#include "BaseScene.h"
#include <Windows.h>

class MenuScene : public BaseScene
{
public:
    MenuScene(int InWidth, int InHeight) : BaseScene(InWidth, InHeight) {}

    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    const int MenuCount = 2;
    int CurrentMenuIndex = 0;

    KeyPressState Keys[3] = {
        { VK_UP, false },
        { VK_DOWN, false },
        { VK_RETURN, false }
    };

    /// <summary>
    /// 위, 아래 방향키, 엔터 키에 따라 할당된 동작을 수행하는 함수
    /// </summary>
    /// <param name="InKeyCode">입력된 키 코드</param>
    void ProcessKeyPress(int InKeyCode);
};
