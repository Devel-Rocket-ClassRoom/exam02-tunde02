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

#if 0
Monster::Monster(int InX, int InY)
{
    Transform_.Position = Vector2{ InX, InY };
    //Transform_.Delta = Vector2{ 0, 0 };
    DeltaDirection = Direction::Down | Direction::Left;
    Transform_.Width = 5;
    Transform_.Height = 5;
    Delta_ = Vector2{ 0, 0 };
    NextPosition_ = Transform_.Position;
    CollisionLayer_ = CollisionLayer::Monster;
    Hp = 2;
    Speed = 1.0f;
    Damage = 1;

    //UpdatePeriod_ = 0.016f;
    UpdatePeriod_ = 0.02f / Speed;

    RenderString_.reserve(Transform_.Width * Transform_.Height);

    //for (int i = 0; i < Transform_.Height; i++)
    //{
    //    std::wstring Str{};
    //    for (int j = 0; j < Transform_.Width; j++)
    //    {
    //        //Str += L"X";
    //        Str += std::to_wstring(rand() % 10);
    //    }
    //    RenderString_.push_back(Str);
    //}
}
#endif

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
    NormalizeDelta();
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

void Monster::Initialize(const Transform& InTransform, const Vector2& InDelta)
{
    Transform_.Position = InTransform.Position;
    Delta_ = InDelta;
    NormalizeDelta();
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

#if 0
        //Transform_.Delta.X = (Direction_ & Direction::Right) == Direction::None ? -1 : 1;
        //Transform_.Delta = Delta_;
        Transform_.Position.fX += Delta_.fX;
        Transform_.Position.fY += Delta_.fY;
        Transform_.Position.X = Transform_.Position.fX;
        Transform_.Position.Y = Transform_.Position.fY;
        //NextPosition_.fX += Transform_.Position.fX + Delta_.fX;
        //NextPosition_.fY += Transform_.Position.fY + Delta_.fY;
        NextPosition_ = Transform_.Position;
        //NextPosition_.X = NextPosition_.fX;
        //NextPosition_.Y = NextPosition_.fY;
        //printf("(%d, %d)\n", NextPosition_.X, NextPosition_.Y);
#endif
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
        if (Transform_.Position.Y + Transform_.Height > GameEngine::Instance().GetScreenHeight() - 4)
        {
            Destroy();
        }
        else
        {
            TurnAround();
            UpdateNextPosition();
        }
    }

    //if (Other->GetCollisionLayer() == CollisionLayer::Wall)
    //{
    //    if (Other->GetPosition().Y > Transform_.Position.Y)
    //    {
    //        Destroy();
    //    }
    //    else
    //    {
    //        TurnAround();
    //        UpdateNextPosition();
    //    }
    //}
    //else if (Other->GetCollisionLayer() == CollisionLayer::Player)
    //{
    //    Other->TakeDamage(Damage);
    //}
}

void Monster::TurnAround()
{
#if 0
    bool isRight = HasFlag(DeltaDirection, Direction::Right);

    SubDeltaDirection(isRight ? Direction::Right : Direction::Left);
    AddDeltaDirection(isRight ? Direction::Left : Direction::Right);
#endif

    Delta_.X *= -1.0f;
}

void Monster::FireBullet() const
{
    // MonsterType에 따라 총구와 발사하는 총알 개수가 달라짐
    if (MonsterType_ == MonsterType::Default)
    {
        Bullet* FiredBullet = new Bullet(Faction_, BulletType_);
        Transform BulletTransform{};
        Vector2 BulletDelta{ 0, 1 };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletTransform.Position.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2)));
        BulletTransform.Position.Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));

        GameEngine::Instance().Instantiate(FiredBullet, BulletTransform, BulletDelta);
    }
    else if (MonsterType_ == MonsterType::TripleShot)
    {
        std::vector<Bullet*> FiredBullets = { new Bullet(Faction_, BulletType_) , new Bullet(Faction_, BulletType_), new Bullet(Faction_, BulletType_) };
        std::vector<Transform> BulletTransforms = { Transform{}, Transform{}, Transform{} };
        std::vector<Vector2> BulletDeltas = { { -1.0f, 0.5f }, { 0.0f, 1.0f }, { 1.0f, 0.5f } };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletTransforms[1].Position.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullets[1]->GetWidth() / 2)));
        BulletTransforms[1].Position.Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));
        BulletTransforms[0].Position.X = BulletTransforms[1].Position.X - 1.0f;
        BulletTransforms[0].Position.Y = BulletTransforms[1].Position.Y;
        BulletTransforms[2].Position.X = BulletTransforms[1].Position.X + 1.0f;
        BulletTransforms[2].Position.Y = BulletTransforms[1].Position.Y;

        for (int i = 0; i < 3; i++)
        {
            GameEngine::Instance().Instantiate(FiredBullets[i], BulletTransforms[i], BulletDeltas[i]);
        }
    }
    else if (MonsterType_ == MonsterType::Settled)
    {
        Bullet* FiredBullet = new Bullet(Faction_, BulletType_);
        Transform BulletTransform{};
        Vector2 BulletDelta{ 0, 1 };

        auto [RoundedPosX, RoundedPosY] = Transform_.Position.ToRoundInt();
        auto [RoundedDeltaX, RoundedDeltaY] = Delta_.ToRoundInt();

        BulletTransform.Position.X = static_cast<float>(RoundedPosX + RoundedDeltaX + static_cast<int>((Transform_.Width / 2) - (FiredBullet->GetWidth() / 2)));
        BulletTransform.Position.Y = static_cast<float>(RoundedPosY + RoundedDeltaY + static_cast<int>(Transform_.Height - 1));

        GameEngine::Instance().Instantiate(FiredBullet, BulletTransform, BulletDelta);
    }
    else if (MonsterType_ == MonsterType::Boss)
    {
        // TODO:
    }
}
