#include "Ground.h"
#include <algorithm>

Ground::Ground()
{
    Transform_.Position = Vector2{ 0, 18 };
    Transform_.Width = 50;
    Transform_.Height = 2;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Ground;

    bUseGravity_ = false;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j =0; j < Transform_.Width; j++)
        {
            Str += L"█";
        }
        RenderString_.push_back(Str);
    }
}

Ground::Ground(int InX, int InY)
{
}

Ground::Ground(int InX, int InY, size_t InWidth, size_t InHeight)
{
    Transform_.Position = Vector2{ InX, InY };
    Transform_.Width = InWidth;
    Transform_.Height = InHeight;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Ground;

    bUseGravity_ = false;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j =0; j < Transform_.Width; j++)
        {
            Str += L"█";
        }
        RenderString_.push_back(Str);
    }
}

void Ground::Update()
{
}

void Ground::Update(int Gravity)
{

}

void Ground::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        return;
    }

    Other->SetUseGravity(false);
}

void Ground::OnCollisionExit(GameObject* Other)
{
    if (Other == nullptr)
    {
        return;
    }

    auto Collisions = Other->GetCurrentCollisions();
    bool IsCollidedWithGround = std::any_of(
        Collisions.begin(),
        Collisions.end(),
        [](GameObject* Obj) {
            return Obj->GetCollisionLayer() == CollisionLayer::Ground;
        });

    if (!IsCollidedWithGround)
    {
        Other->SetUseGravity(true);
    }
}
