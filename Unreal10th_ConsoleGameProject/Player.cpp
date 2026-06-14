#include "Player.h"
#include "GameEngine.h"
#include "Bullet.h"
#include <Windows.h>

Player::Player()
{
    Transform_.Position = Vector2{ 30, 40 };
    Transform_.Width = 3;
    Transform_.Height = 3;
    CollisionLayer_ = CollisionLayer::Player;
    Hp = InitialHp;
    Damage = 1;
    Speed = 2.0f;
    Faction_ = Faction::Player;
    ShotDelay = PlayerShotDelay;
    CurrentBulletType = BulletType::Default;

    RenderString_.push_back(L" ▲ ");
    RenderString_.push_back(L"║▇║");
    RenderString_.push_back(L"◢▇◣");
}

void Player::Update()
{
    float DeltaTime = GameEngine::Instance().GetFixedDeltaTime();
    ShotDelay -= DeltaTime;
    InvincibleTimer += DeltaTime;

    if (InvincibleTimer > InvincibleDuration)
    {
        InvincibleTimer = 0.0f;
        bCanCollide = true;
        ChangePlayerState(PlayerState::Normal);
    }

    Delta_.X = 0.0f;
    Delta_.Y = 0.0f;

    if (GetAsyncKeyState(VK_UP) && CurrentPlayerState != PlayerState::Uncontrollable) // ↑
    {
        Delta_.Y = -1.0f;
    }
    else if (GetAsyncKeyState(VK_DOWN) && CurrentPlayerState != PlayerState::Uncontrollable) // ↓
    {
        Delta_.Y = 1.0f;
    }

    if (GetAsyncKeyState(VK_LEFT) && CurrentPlayerState != PlayerState::Uncontrollable) // ←
    {
        Delta_.X = -1.0f;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && CurrentPlayerState != PlayerState::Uncontrollable) // →
    {
        Delta_.X = 1.0f;
    }

    NormalizeDelta();

#if 0
    if (GetAsyncKeyState(VK_UP)) // ↑
    {
        AddDeltaDirection(Direction::Up);
    }
    else if (GetAsyncKeyState(VK_DOWN)) // ↓
    {
        AddDeltaDirection(Direction::Down);
    }

    if (GetAsyncKeyState(VK_LEFT)) // ←
    {
        AddDeltaDirection(Direction::Left);
    }
    else if (GetAsyncKeyState(VK_RIGHT)) // →
    {
        AddDeltaDirection(Direction::Right);
    }
#endif

    if (GetAsyncKeyState(VK_SPACE) && CurrentPlayerState != PlayerState::Uncontrollable)
    {
        if (ShotDelay <= 0.0f)
        {
            ShotDelay = PlayerShotDelay;
            FireBullet();
        }
    }

    UpdateNextPosition();
}

void Player::OnCollisionEnter(GameObject* Other)
{

}

void Player::OnCollisionExit(GameObject* Other)
{
}

void Player::TakeDamage(int InDamage)
{
    if (CurrentPlayerState == PlayerState::Invincible)
    {
        return;
    }

    GameObject::TakeDamage(InDamage);
    bCanCollide = false;
    ChangePlayerState(PlayerState::Invincible);
}

void Player::FireBullet() const
{
    // 플레이어의 위치, 크기와 총알의 따른 크기에 따라 총알의 생성 위치 조절
    // - OffsetX : Player.Width/2 - Bullet.Width/2
    // - OffsetY : -Bullet.Height
    Bullet* FiredBullet = new Bullet(Faction_, CurrentBulletType);
    Vector2 BulletDelta{ 0, -1 };
    Transform BulletTransform{};
    auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
    auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();
    BulletTransform.Position.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2)));
    BulletTransform.Position.Y = static_cast<float>(RoundedPosY + RoundedDeltaY - static_cast<int>(FiredBullet->GetHeight()));

    GameEngine::Instance().Instantiate(FiredBullet, BulletTransform, BulletDelta);
}
