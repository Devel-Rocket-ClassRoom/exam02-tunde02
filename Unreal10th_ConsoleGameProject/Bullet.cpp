#include "Bullet.h"
#include "GameEngine.h"

Bullet::Bullet()
{
    BulletType_ = BulletType::Default;
    BulletSpec Spec = BulletSpecs.at(BulletType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    CollisionLayer_ = CollisionLayer::Bullet;
    Faction_ = Faction::None;
    Hp = Spec.Hp;
    Damage = Spec.Damage;
    Speed = Spec.Speed;
    UpdatePeriod_ = 0.05f / Speed;
    RenderString_ = Spec.RenderString;
}

Bullet::Bullet(Faction InFaction, BulletType InBulletType)
{
    BulletType_ = InBulletType;
    BulletSpec Spec = BulletSpecs.at(BulletType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    CollisionLayer_ = CollisionLayer::Bullet;
    Faction_ = InFaction;
    Hp = Spec.Hp;
    Damage = Spec.Damage;
    Speed = Spec.Speed;
    UpdatePeriod_ = 0.05f / Speed;
    RenderString_ = Spec.RenderString;
}

#if 0
//Bullet::Bullet(int InX, int InY, int InDeltaX, int InDeltaY)
//{
//    Transform_.Position = Vector2{ InX, InY };
//    //Transform_.Delta = Vector2{ InDeltaX, InDeltaY };
//    Delta_ = Vector2{ InDeltaX, InDeltaY };
//    Transform_.Delta = Delta_;
//    Transform_.Width = 2;
//    Transform_.Height = 2;
//    NextPosition_ = Transform_.Position;
//    Collider_ = Collider(Transform_, CollisionLayer::Bullet);
//
//    UpdatePeriod_ = 0.04f;
//
//    RenderString_.reserve(Transform_.Width * Transform_.Height);
//    //RenderString_.push_back(L"░");
//    RenderString_.push_back(L"▒ ");
//    RenderString_.push_back(L" █");
//}

//Bullet::Bullet(const Transform& InTransform, const Vector2& InDelta, Faction InFaction, BulletType InBulletType)
//{
//    BulletType_ = InBulletType;
//    const BulletSpec Spec = BulletSpecs.at(BulletType_);
//
//    Transform_.Position = InTransform.Position + Spec.BarrelOffset;
//    Transform_.Width = Spec.Width;
//    Transform_.Height = Spec.Height;
//    Delta_ = InDelta;
//    UpdatePeriod_ = 0.05f / Spec.Speed;
//    NextPosition_ = Transform_.Position;
//    Hp = Spec.Hp;
//    Damage = Spec.Damage;
//    RenderString_ = Spec.RenderString;
//    Faction_ = InFaction;
//}

//Bullet::Bullet(const Vector2& InTransform, const Vector2& InDelta, Faction InFaction, BulletType InBulletType)
//{
//    const BulletSpec Spec = BulletSpecs.at(BulletType_);
//
//    Transform_.Position = InTransform + Spec.BarrelOffset;
//    Transform_.Width = Spec.Width;
//    Transform_.Height = Spec.Height;
//    Delta_ = InDelta;
//    UpdatePeriod_ = 0.05f / Spec.Speed;
//    NextPosition_ = Transform_.Position;
//    Hp = Spec.Hp;
//    Damage = Spec.Damage;
//    RenderString_ = Spec.RenderString;
//}

//Bullet::Bullet(const Transform& InTransform, const Vector2 InDelta, const Faction InFaction, int InHp, BulletType InBulletType)
//{
//    Transform_ = InTransform;
//    Delta_ = InDelta;
//    NextPosition_ = Transform_.Position;
//    Collider_ = Collider(Transform_, CollisionLayer::Bullet);
//    Hp = InHp;
//    Damage = 2;
//    BulletType_ = BulletType::Default;
//
//    UpdatePeriod_ = 0.04f;
//
//    RenderString_.reserve(Transform_.Width * Transform_.Height);
//    //RenderString_.push_back(L"░");
//    RenderString_.push_back(L"▒");
//    RenderString_.push_back(L"█");
//
//    Faction_ = InFaction;
//}
#endif

void Bullet::Initialize(const Transform& InTransform, const Vector2& InDelta)
{
    Transform_.Position = InTransform.Position;
    Delta_ = InDelta;
    Delta_ = Delta_.Normalized();
    //if (std::sqrt(Delta_.X * Delta_.X + Delta_.Y * Delta_.Y) > 1.0f)
    //{
    //}
    NextPosition_ = Transform_.Position;
}

void Bullet::Initialize(const Transform InTransform, const Vector2 InDelta)
{
    GameObject::Initialize(InTransform, InDelta);
    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j = 0; j < Transform_.Width; j++)
        {
            Str += L"█";
        }
        RenderString_.push_back(Str);
    }
}

void Bullet::Update()
{
    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (UpdateTimer_ >= UpdatePeriod_)
    {
        UpdateTimer_ -= UpdatePeriod_;

        UpdateNextPosition();
    }
}

void Bullet::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        Destroy();
    }
    else if ((Other->GetCollisionLayer() == CollisionLayer::Bullet)
             || (Other->GetCollisionLayer() == CollisionLayer::Item))
    {
        return;
    }
    else if (Other->GetCollisionLayer() == CollisionLayer::Wall)
    {
        Destroy();
    }
    else if (Other->GetFaction() != Faction_)
    {
        Other->TakeDamage(Damage);
        TakeDamage(1);
    }
}
