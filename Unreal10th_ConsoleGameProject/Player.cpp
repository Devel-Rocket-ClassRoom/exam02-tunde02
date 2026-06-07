#include "Player.h"
#include <Windows.h>

Player::Player()
{
    Transform_.Position = Vector2{ 5, 5 };
    Transform_.Width = 3;
    Transform_.Height = 3;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Player;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j = 0; j < Transform_.Width; j++)
        {
            Str += L"O";
        }
        RenderString_.push_back(Str);
    }
}

void Player::Update()
{
    Transform_.Delta.X = 0;
    Transform_.Delta.Y = 0;

    if (GetAsyncKeyState(VK_UP)) // ↑
    {
        Transform_.Delta.Y = -1;
    }
    else if (GetAsyncKeyState(VK_DOWN)) // ↓
    {
        Transform_.Delta.Y = 1;
    }

    if (GetAsyncKeyState(VK_LEFT)) // ←
    {
        Transform_.Delta.X = -1;
    }
    else if (GetAsyncKeyState(VK_RIGHT)) // →
    {
        Transform_.Delta.X = 1;
    }

    NextPosition_ = Transform_.Position + Transform_.Delta;

    //Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
    //if (0 <= NextPosition.X && NextPosition.X < 120
    //    && 0 <= NextPosition.Y && NextPosition.Y < 29)
    //{
    //    NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
    //    Transform_.Position = NextPosition;
    //}
}

void Player::OnCollisionEnter(GameObject* Other)
{
}

void Player::OnCollisionExit(GameObject* Other)
{
}
