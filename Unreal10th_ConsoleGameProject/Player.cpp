#include "Player.h"
#include "GameEngine.h"
#include "Bullet.h"
#include <Windows.h>

Player::Player()
{
    Transform_.Position = Vector2{ 5, 5 };
    Transform_.Width = 3;
    Transform_.Height = 3;
    Collider_ = Collider(Transform_, CollisionLayer::Player);
    Hp = InitialHp;
    Damage = 1;
    Faction_ = Faction::Player;
    ShotDelay = PlayerShotDelay;
    CurrentBulletType = BulletType::Default;

    RenderString_.push_back(L" ▲ ");
    RenderString_.push_back(L"║▇║");
    RenderString_.push_back(L"◢▇◣");
}

void Player::Update()
{
    Transform_.Delta.X = 0;
    Transform_.Delta.Y = 0;
    ShotDelay -= GameEngine::Instance().GetFixedDeltaTime();

    if (GetAsyncKeyState(VK_UP)) // ↑
    {
        Transform_.Delta.Y = -1;
    }
    else if (GetAsyncKeyState(VK_DOWN)) // ↓
    {
        Transform_.Delta.Y = 1;
    }

    if (GetAsyncKeyState(VK_LEFT)) // ←
    {
        Transform_.Delta.X = -1;
    }
    else if (GetAsyncKeyState(VK_RIGHT)) // →
    {
        Transform_.Delta.X = 1;
    }

    if (GetAsyncKeyState(VK_SPACE))
    {
        if (ShotDelay <= 0.0f)
        {
            ShotDelay = PlayerShotDelay;
            FireBullet();
        }
    }

    NextPosition_ = Transform_.Position + Transform_.Delta;
}

void Player::OnCollisionEnter(GameObject* Other)
{
}

void Player::OnCollisionExit(GameObject* Other)
{
}

void Player::FireBullet() const
{
    // 플레이어의 위치, 크기와 총알의 따른 크기에 따라 총알의 생성 위치 조절
    // - OffsetX : Player.Width/2 - Bullet.Width/2
    // - OffsetY : -Bullet.Height
    Bullet* FiredBullet = new Bullet(Faction_, CurrentBulletType);
    Vector2 BulletDelta{ 0, -1 };
    Transform BulletTransform{};
    BulletTransform.Position.X = Transform_.Position.X + Transform_.Delta.X + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2));
    BulletTransform.Position.Y = Transform_.Position.Y + Transform_.Delta.Y - static_cast<int>(FiredBullet->GetHeight());

    GameEngine::Instance().Instantiate(FiredBullet, BulletTransform, BulletDelta);
}
