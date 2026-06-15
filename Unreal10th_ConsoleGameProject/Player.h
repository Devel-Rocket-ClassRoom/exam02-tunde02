#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    const int InitialHp = 5;
    const int MaxHp = 10;
    const float PlayerShotDelay = 0.025f;

    float ShotDelay = 0.0f;
    BulletType CurrentBulletType = BulletType::Default;

    PlayerState CurrentPlayerState = PlayerState::Normal;
    const float InvincibleDuration = 0.5f;
    float InvincibleTimer = 0.0f;


public:
    Player();

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
    virtual void TakeDamage(int InDamage) override;
    //void NormalizeDelta();

    void FireBullet() const;
    void RecoverHp(int InAmount);
    void UpgradeBullet();

    inline int GetHp() const { return Hp; }
    inline void ChangePlayerState(PlayerState InPlayerState) { CurrentPlayerState = InPlayerState; }
    inline bool IsInvincible() const { return CurrentPlayerState == PlayerState::Invincible; }
};
