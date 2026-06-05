#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
};