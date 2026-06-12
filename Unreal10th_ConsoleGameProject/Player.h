#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    const int InitialHp = 5;
    const int MaxHp = 10;
    const float PlayerShotDelay = 0.033f;

    float ShotDelay = 0.0f;
    BulletType CurrentBulletType = BulletType::Default;

public:
    Player();

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;

    void FireBullet() const;

    inline int GetHp() const { return Hp; }
};
