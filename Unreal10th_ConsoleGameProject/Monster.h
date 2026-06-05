#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
private:
    int CollisionCount = 0;
public:
    Monster();

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;

    void TurnAround()
    {
        Transform_.Delta.X *= -1;
    }

    //void TurnAround()
    //{
    //    Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
    //    if (NextPosition.X >= 99)
    //    {
    //        Transform_.Delta.X = -1;
    //        NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
    //    }
    //    else if (NextPosition.X < 0)
    //    {
    //        Transform_.Delta.X = 1;
    //        NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
    //    }

    //    Transform_.Position = NextPosition;
    //}
};
