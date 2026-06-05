#pragma once
#include "BaseScene.h"

class MenuScene1 : public BaseScene
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    int CurrentMenuIndex = 0;
};
