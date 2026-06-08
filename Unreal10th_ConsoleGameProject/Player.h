#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    float JumpGauge = 0.0f;
    const int JumpDeltaYArray[10]{ -1, -1, -1, -2, -2, -2, -2, -1, -1, -1 };

public:
    Player();

    virtual void Update() override;
    virtual void Update(int Gravity) override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
};
