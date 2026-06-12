#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
    Monster();
    Monster(int InX, int InY);

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionStay(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;

    void TurnAround();
};
