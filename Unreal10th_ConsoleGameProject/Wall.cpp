#include "Wall.h"

Wall::Wall()
{
}

Wall::Wall(int InX, int InY)
{
}

Wall::Wall(int InX, int InY, size_t InWidth, size_t InHeight)
{
    Transform_.Position = Vector2{ static_cast<float>(InX), static_cast<float>(InY) };
    Transform_.Width = InWidth;
    Transform_.Height = InHeight;
    NextPosition_ = Transform_.Position;
    CollisionLayer_ = CollisionLayer::Wall;

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

Wall::Wall(float InX, float InY, size_t InWidth, size_t InHeight)
{
    Transform_.Position = Vector2{ InX, InY };
    Transform_.Width = InWidth;
    Transform_.Height = InHeight;
    NextPosition_ = Transform_.Position;
    CollisionLayer_ = CollisionLayer::Wall;

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

void Wall::Update()
{
}

void Wall::OnCollisionEnter(GameObject* Other)
{
}

void Wall::OnCollisionExit(GameObject* Other)
{
}
