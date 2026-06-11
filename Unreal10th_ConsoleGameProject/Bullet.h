#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(int InX, int InY, int InDeltaX, int InDeltaY);
    Bullet(const Transform& InTransform, const Vector2 InDelta);

    virtual void Update() override;
    virtual void Update(int Gravity) override;
    virtual void OnCollisionEnter(GameObject* Other) override;

private:
};
