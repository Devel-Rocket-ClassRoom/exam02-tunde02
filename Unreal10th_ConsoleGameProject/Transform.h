#pragma once
#include "Vector.h"

struct Transform
{
    Vector2 Position{};
    Vector2 Delta{};
    size_t Width = 0;
    size_t Height = 0;

    Transform() = default;
};
