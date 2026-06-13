#include "GameObject.h"

void GameObject::ApplyMove()
{
    Transform_.Position = NextPosition_;
    //Transform_.Delta = Vector2(0, 0);
}

void GameObject::ApplyMove(float InX, float InY)
{
    Transform_.Position.X = InX;
    Transform_.Position.Y = InY;
}

void GameObject::ApplyXMove(float InX)
{
    Transform_.Position.X += InX;
}

void GameObject::ApplyYMove(float InY)
{
    Transform_.Position.Y += InY;
}

void GameObject::CancelMove()
{
    NextPosition_ = Transform_.Position;
    //Transform_.Delta = Vector2(0, 0);
}

void GameObject::CancelXMove()
{
    NextPosition_.X = Transform_.Position.X;
    //Transform_.Delta.X = 0;
}

void GameObject::CancelYMove()
{
    NextPosition_.Y = Transform_.Position.Y;
    //Transform_.Delta.Y = 0;
}

void GameObject::Initialize(const Transform& InTransform, const Vector2& InDelta)
{
    Transform_ = InTransform;
    Delta_ = InDelta;
    NextPosition_ = Transform_.Position;
}

void GameObject::UpdateCollisions()
{
    PrevCollisions = std::move(CurrentCollisions);
    CurrentCollisions.clear();
}

void GameObject::TakeDamage(int InDamage)
{
    Hp -= InDamage;

    if (Hp <= 0)
    {
        Destroy();
    }
}
