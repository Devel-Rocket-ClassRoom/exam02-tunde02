#include "Vector.h"

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