#include "Bullet.h"
#include "GameEngine.h"

Bullet::Bullet()
{
    Transform_.Width = 2;
    Transform_.Height = 2;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Bullet;

    UpdatePeriod_ = 0.04f;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    //RenderString_.push_back(L"░");
    RenderString_.push_back(L"█ ");
    RenderString_.push_back(L"▒ ");
}

Bullet::Bullet(int InX, int InY, int InDeltaX, int InDeltaY)
{
    Transform_.Position = Vector2{ InX, InY };
    //Transform_.Delta = Vector2{ InDeltaX, InDeltaY };
    Delta_ = Vector2{ InDeltaX, InDeltaY };
    Transform_.Delta = Delta_;
    Transform_.Width = 2;
    Transform_.Height = 2;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Bullet;

    UpdatePeriod_ = 0.04f;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    //RenderString_.push_back(L"░");
    RenderString_.push_back(L"▒ ");
    RenderString_.push_back(L" █");
}

Bullet::Bullet(const Transform& InTransform, const Vector2 InDelta)
{
    Transform_ = InTransform;
    Delta_ = InDelta;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Bullet;

    UpdatePeriod_ = 0.04f;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    //RenderString_.push_back(L"░");
    RenderString_.push_back(L"█ ");
    RenderString_.push_back(L"▒ ");
}

void Bullet::Update()
{
    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (UpdateTimer_ >= UpdatePeriod_)
    {
        UpdateTimer_ -= UpdatePeriod_;


        Transform_.Delta = Delta_;
        NextPosition_ = Transform_.Position + Transform_.Delta;
    }
}

void Bullet::Update(int Gravity)
{
}

void Bullet::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        return;
    }

    if (Other->GetCollisionLayer() == CollisionLayer::Wall)
    {
        Destroy();
    }
}
