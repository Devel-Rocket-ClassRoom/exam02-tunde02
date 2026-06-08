#pragma once
#include "GameObject.h"

class Ground : public GameObject
{
public:
    Ground();
    Ground(int InX, int InY);
    Ground(int InX, int InY, size_t InWidth, size_t InHeight);

    virtual void Update() override;
    virtual void Update(int Gravity) override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
};

