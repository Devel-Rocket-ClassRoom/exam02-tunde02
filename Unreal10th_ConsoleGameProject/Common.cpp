#include "Common.h"

Vector2 Vector2::operator+(const Vector2& other) const
{
    Vector2 Result{};

    Result.X = this->X + other.X;
    Result.Y = this->Y + other.Y;

    return Result;
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    Vector2 Result{};

    Result.X = this->X - other.X;
    Result.Y = this->Y - other.Y;

    return Result;
}

Vector2& Vector2::operator=(const Vector2& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->X = other.X;
    this->Y = other.Y;

    return *this;
}

Collider::Collider(const Vector2& Position, size_t InWidth, size_t InHeight)
{
    X = Position.X;
    Y = Position.Y;
    Width = InWidth;
    Height = InHeight;
}

void Collider::Initialize(const Transform& InTransform)
{
    X = InTransform.Position.X;
    Y = InTransform.Position.Y;
    Width = InTransform.Width;
    Height = InTransform.Height;
}
