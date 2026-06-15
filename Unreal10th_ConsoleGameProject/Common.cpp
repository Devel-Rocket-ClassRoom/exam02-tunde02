#include "Common.h"
#include "GameEngine.h"
#include <iostream>

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

Vector2 Vector2::operator*(const int multiplier) const
{
    Vector2 Result{};

    Result.X = this->X * multiplier;
    Result.Y = this->Y * multiplier;

    return Result;;
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

Transform::Transform(float InX, float InY, size_t InWidth, size_t InHeight)
{
    Position = Vector2{ InX, InY };
    Width = InWidth;
    Height = InHeight;
}

Transform& Transform::operator=(const Transform& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->Position = other.Position;
    this->Width = other.Width;
    this->Height = other.Height;

    return *this;
}

//Collider::Collider(size_t InWidth, size_t InHeight, CollisionLayer InLayer)
//{
//    Width = InWidth;
//    Height = InHeight;
//    Layer = InLayer;
//}
//
//Collider::Collider(const Transform& InTransform, CollisionLayer InLayer)
//{
//    Width = InTransform.Width;
//    Height = InTransform.Height;
//    Layer = InLayer;
//}
