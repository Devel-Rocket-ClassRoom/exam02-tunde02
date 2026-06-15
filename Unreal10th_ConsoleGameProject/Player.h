#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    const int InitialHp = 5;
    const int MaxHp = 10;
    const float PlayerShotDelay = 0.025f;
    const float InvincibleDuration = 0.5f;

    BulletType CurrentBulletType = BulletType::Default;
    PlayerState CurrentPlayerState = PlayerState::Normal;
    float ShotDelay = 0.0f;
    float InvincibleTimer = 0.0f;


public:
    Player();

    virtual void Update() override;
    virtual void TakeDamage(int InDamage) override;

    void FireBullet() const;
    void RecoverHp(int InAmount);
    void UpgradeBullet();

    inline int GetHp() const { return Hp; }

    /// <summary>
    /// 플레이어의 현재 상태를 바꾸는 함수
    /// </summary>
    /// <param name="InPlayerState">바꿀 플레이어 상태 열거체</param>
    inline void ChangePlayerState(PlayerState InPlayerState) { CurrentPlayerState = InPlayerState; }
    inline bool IsInvincible() const { return CurrentPlayerState == PlayerState::Invincible; }
};
