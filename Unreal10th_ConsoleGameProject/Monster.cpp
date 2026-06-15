#include "Monster.h"
#include "GameEngine.h"
#include "Bullet.h"

Monster::Monster()
{
    MonsterType_ = MonsterType::Default;
    MonsterSpec Spec = MonsterSpecs.at(MonsterType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    CollisionLayer_ = CollisionLayer::Monster;
    Faction_ = Faction::Monster;
    Hp = Spec.Hp;
    Damage = Spec.Damage;
    Speed = Spec.Speed;
    UpdatePeriod_ = GameEngine::Instance().GetFixedDeltaTime() / Speed;
    BulletType_ = Spec.BulletType_;
    MonsterShotDelay = Spec.ShotDelay;
    RenderString_ = Spec.RenderString;
}

Monster::Monster(MonsterType InMonsterType)
{
    MonsterType_ = InMonsterType;
    MonsterSpec Spec = MonsterSpecs.at(MonsterType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    CollisionLayer_ = CollisionLayer::Monster;
    Faction_ = Faction::Monster;
    Hp = Spec.Hp;
    Damage = Spec.Damage;
    Speed = Spec.Speed;
    UpdatePeriod_ = GameEngine::Instance().GetFixedDeltaTime() / Speed;
    BulletType_ = Spec.BulletType_;
    MonsterShotDelay = Spec.ShotDelay;
    RenderString_ = Spec.RenderString;
}

Monster::Monster(MonsterType InMonsterType, Vector2 InDelta)
{
    MonsterType_ = InMonsterType;
    MonsterSpec Spec = MonsterSpecs.at(MonsterType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    Delta_ = InDelta;
    if (std::sqrt(Delta_.X * Delta_.X + Delta_.Y * Delta_.Y) > 1.0f)
    {
        Delta_ = Delta_.Normalized();
    }
    CollisionLayer_ = CollisionLayer::Monster;
    Faction_ = Faction::Monster;
    Hp = Spec.Hp;
    Damage = Spec.Damage;
    Speed = Spec.Speed;
    UpdatePeriod_ = GameEngine::Instance().GetFixedDeltaTime() / Speed;
    BulletType_ = Spec.BulletType_;
    MonsterShotDelay = Spec.ShotDelay;
    RenderString_ = Spec.RenderString;
}

void Monster::Initialize(const Vector2& InPosition, const Vector2& InDelta)
{
    Transform_.Position = InPosition;
    Delta_ = InDelta;
    if (std::sqrt(Delta_.X * Delta_.X + Delta_.Y * Delta_.Y) > 1.0f)
    {
        Delta_ = Delta_.Normalized();
    }
    NextPosition_ = Transform_.Position;
}

void Monster::Update()
{
    const float DeltaTime = GameEngine::Instance().GetFixedDeltaTime();
    UpdateTimer_ += DeltaTime;
    ShotDelay -= DeltaTime;

    if (UpdateTimer_ >= UpdatePeriod_)
    {
        UpdateTimer_ -= UpdatePeriod_;
        UpdateNextPosition();
    }

    if (ShotDelay <= 0.0f)
    {
        ShotDelay = MonsterShotDelay;
        FireBullet();
    }
}

void Monster::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        // 충돌한 오브젝트가 nullptr -> 외곽에 충돌했다는 뜻
        if (Transform_.Position.Y + Transform_.Height > GameEngine::Instance().GetScreenHeight() - 4)
        {
            // 아래쪽 외곽에 충돌하면 몬스터를 파괴
            Destroy();
        }
        else
        {
            // 나머지 외곽에 충돌하면 X축 이동 방향 반전
            TurnAround();
            UpdateNextPosition();
        }
    }
    else if (Other->GetCollisionLayer() == CollisionLayer::Player)
    {
        // 충돌한 오브젝트가 플레이어라면 플레이어에게 피해를 입힘
        Other->TakeDamage(Damage);
    }
}

void Monster::TurnAround()
{
    Delta_.X *= -1.0f;
}

void Monster::FireBullet() const
{
    // MonsterType에 따라 총구와 발사하는 총알 개수가 달라진다

    if (MonsterType_ == MonsterType::Default)
    {
        Bullet* FiredBullet = new Bullet(Faction_, BulletType_);
        Vector2 BulletPosition{};
        Vector2 BulletDelta{ 0, 1 };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletPosition.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2)));
        BulletPosition.Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));

        GameEngine::Instance().Instantiate(FiredBullet, BulletPosition, BulletDelta);
    }
    else if (MonsterType_ == MonsterType::TripleShot)
    {
        std::vector<Bullet*> FiredBullets = { new Bullet(Faction_, BulletType_) , new Bullet(Faction_, BulletType_), new Bullet(Faction_, BulletType_) };
        std::vector<Vector2> BulletPositions = { Vector2{}, Vector2{}, Vector2{} };
        std::vector<Vector2> BulletDeltas = { { -1.0f, 0.5f }, { 0.0f, 1.0f }, { 1.0f, 0.5f } };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletPositions[1].X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullets[1]->GetWidth() / 2)));
        BulletPositions[1].Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));
        BulletPositions[0].X = BulletPositions[1].X - 1.0f;
        BulletPositions[0].Y = BulletPositions[1].Y;
        BulletPositions[2].X = BulletPositions[1].X + 1.0f;
        BulletPositions[2].Y = BulletPositions[1].Y;

        for (int i = 0; i < 3; i++)
        {
            GameEngine::Instance().Instantiate(FiredBullets[i], BulletPositions[i], BulletDeltas[i]);
        }
    }
    else if (MonsterType_ == MonsterType::Settled)
    {
        Bullet* FiredBullet = new Bullet(Faction_, BulletType_);
        Vector2 BulletPosition{};
        Vector2 BulletDelta{ 0, 1 };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletPosition.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2)));
        BulletPosition.Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));

        GameEngine::Instance().Instantiate(FiredBullet, BulletPosition, BulletDelta);
    }
    else if (MonsterType_ == MonsterType::Boss)
    {
        Bullet* FiredBullet = new Bullet(Faction_, BulletType_);
        Vector2 MidBarrel{};
        Vector2 LeftWing{};
        Vector2 RightWing{};
        Vector2 RandomBarrel[12]{};
        Vector2 BulletDelta{ 0, 1 };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        // Mid
        MidBarrel.X = RoundedPosX + RoundedDeltaX + static_cast<float>(Transform_.Width / 2);
        MidBarrel.Y = RoundedPosY + RoundedDeltaY + 8.0f;

        GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Upgrade_3), { MidBarrel.X - 2, MidBarrel.Y }, { 0.0f, 1.0f });

        // Left Wing
        LeftWing.X = MidBarrel.X - 14;
        LeftWing.Y = MidBarrel.Y + 4;

        GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Upgrade_2), LeftWing, { 0.0f, 1.0f });

        // Right Wing
        RightWing.X = MidBarrel.X + 10;
        RightWing.Y = MidBarrel.Y + 4;

        GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Upgrade_2), RightWing, { 0.0f, 1.0f });

        // Left Random Barrel
        RandomBarrel[0].X = MidBarrel.X - 20;
        RandomBarrel[0].Y = RoundedPosY + RoundedDeltaY + 7.0f;
        RandomBarrel[1].X = MidBarrel.X - 16;
        RandomBarrel[1].Y = RoundedPosY + RoundedDeltaY + 8.0f;
        RandomBarrel[2].X = MidBarrel.X - 16;
        RandomBarrel[2].Y = RoundedPosY + RoundedDeltaY + 11.0f;
        RandomBarrel[3].X = MidBarrel.X - 9;
        RandomBarrel[3].Y = RoundedPosY + RoundedDeltaY + 10.0f;
        RandomBarrel[4].X = MidBarrel.X - 6;
        RandomBarrel[4].Y = RoundedPosY + RoundedDeltaY + 11.0f;
        RandomBarrel[5].X = MidBarrel.X - 7;
        RandomBarrel[5].Y = RoundedPosY + RoundedDeltaY + 12.0f;

        //Right Random Barrel
        RandomBarrel[6].X = MidBarrel.X + 20;
        RandomBarrel[6].Y = RoundedPosY + RoundedDeltaY + 7.0f;
        RandomBarrel[7].X = MidBarrel.X + 16;
        RandomBarrel[7].Y = RoundedPosY + RoundedDeltaY + 8.0f;
        RandomBarrel[8].X = MidBarrel.X + 16;
        RandomBarrel[8].Y = RoundedPosY + RoundedDeltaY + 11.0f;
        RandomBarrel[9].X = MidBarrel.X + 9;
        RandomBarrel[9].Y = RoundedPosY + RoundedDeltaY + 10.0f;
        RandomBarrel[10].X = MidBarrel.X + 6;
        RandomBarrel[10].Y = RoundedPosY + RoundedDeltaY + 11.0f;
        RandomBarrel[11].X = MidBarrel.X + 7;
        RandomBarrel[11].Y = RoundedPosY + RoundedDeltaY + 12.0f;

        for (int i = 0; i < 3; i++)
        {
            float RandomDeltaX = (float)rand() / (float)RAND_MAX * -1.0f;
            float RandomDeltaY = (float)rand() / (float)RAND_MAX;
            float RandomTimer = ((float)rand() / (float)RAND_MAX) / 2;
            GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Default), RandomBarrel[i], { RandomDeltaX, RandomDeltaY }, RandomTimer);
        }
        for (int i = 3; i < 6; i++)
        {
            float RandomDeltaX = (float)rand() / (float)RAND_MAX;
            float RandomDeltaY = (float)rand() / (float)RAND_MAX;
            float RandomTimer = ((float)rand() / (float)RAND_MAX) / 2;
            GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Default), RandomBarrel[i], { RandomDeltaX, RandomDeltaY }, RandomTimer);
        }
        for (int i = 6; i < 9; i++)
        {
            float RandomDeltaX = (float)rand() / (float)RAND_MAX * -1.0f;
            float RandomDeltaY = (float)rand() / (float)RAND_MAX;
            float RandomTimer = ((float)rand() / (float)RAND_MAX) / 2;
            GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Default), RandomBarrel[i], { RandomDeltaX, RandomDeltaY }, RandomTimer);
        }
        for (int i = 9; i < 12; i++)
        {
            float RandomDeltaX = (float)rand() / (float)RAND_MAX;
            float RandomDeltaY = (float)rand() / (float)RAND_MAX;
            float RandomTimer = ((float)rand() / (float)RAND_MAX) / 2;
            GameEngine::Instance().Instantiate(new Bullet(Faction_, BulletType::Default), RandomBarrel[i], { RandomDeltaX, RandomDeltaY }, RandomTimer);
        }
    }
}
