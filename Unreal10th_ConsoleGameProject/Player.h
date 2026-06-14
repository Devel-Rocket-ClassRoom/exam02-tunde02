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

    enum class PlayerState { Normal, Invincible, Uncontrollable };
    PlayerState CurrentPlayerState = PlayerState::Normal;
    const float InvincibleDuration = 0.8f;
    float InvincibleTimer = 0.0f;

    inline void ChangePlayerState(PlayerState InPlayerState) { CurrentPlayerState = InPlayerState; }

public:
    Player();

    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;
    virtual void OnCollisionExit(GameObject* Other) override;
    virtual void TakeDamage(int InDamage) override;
    //void NormalizeDelta();

    void FireBullet() const;

    inline int GetHp() const { return Hp; }
    inline bool IsInvincible() const { return CurrentPlayerState == PlayerState::Invincible; }
};
