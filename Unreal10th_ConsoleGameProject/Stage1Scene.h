#pragma once
#include "BaseScene.h"

class Stage1Scene : public BaseScene
{
public:
    Stage1Scene(int Width, int Height);

    virtual void Enter() override;
    virtual void Exit() override;
};
