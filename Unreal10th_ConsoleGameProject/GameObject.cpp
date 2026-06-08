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

void GameObject::CancelXMove()
{
    NextPosition_.X = Transform_.Position.X;
    Transform_.Delta.X = 0;
}

void GameObject::CancelYMove()
{
    NextPosition_.Y = Transform_.Position.Y;
    Transform_.Delta.Y = 0;
}

void GameObject::UpdateCollisions()
{
    PrevCollisions = std::move(CurrentCollisions);
    CurrentCollisions.clear();
}
