#include "GameObject.h"

void GameObject::ApplyXMove(float InX)
{
    Transform_.Position.X += InX;
}

void GameObject::ApplyYMove(float InY)
{
    Transform_.Position.Y += InY;
}

void GameObject::Initialize(const Vector2& InPosition, const Vector2& InDelta)
{
    Transform_.Position = InPosition;
    Delta_ = InDelta;
    NextPosition_ = Transform_.Position;
}

void GameObject::TakeDamage(int InDamage)
{
    Hp -= InDamage;

    if (Hp <= 0)
    {
        Destroy();
    }
}
