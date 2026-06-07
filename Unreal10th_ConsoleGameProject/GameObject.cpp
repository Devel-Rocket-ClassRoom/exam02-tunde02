#include "GameObject.h"

void GameObject::ApplyMove()
{
    Transform_.Position = NextPosition_;
    Transform_.Delta = Vector2(0, 0);
}

void GameObject::CancelMove()
{
    NextPosition_ = Transform_.Position;
    Transform_.Delta = Vector2(0, 0);
}

void GameObject::UpdateCollisions()
{
    PrevCollisions = std::move(CurrentCollisions);
    CurrentCollisions.clear();
}
