#pragma once
#include "GameObject.h"

class Wall : public GameObject
{
public:
    Wall();
    Wall(int InX, int InY);
    Wall(int InX, int InY, size_t InWidth, size_t InHeight);
    Wall(float InX, float InY, size_t InWidth, size_t InHeight);

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
};
