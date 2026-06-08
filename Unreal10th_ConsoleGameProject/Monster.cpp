#include "Monster.h"
#include "GameEngine.h"

#include <stdlib.h>

Monster::Monster()
{
    Transform_.Position = Vector2{ 3, 0 };
    Transform_.Delta = Vector2{ 1, 0 };
    Transform_.Width = 3;
    Transform_.Height = 3;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Monster;

    //UpdatePeriod_ = 0.016f;
    UpdatePeriod_ = 0.2f;
    GravityPeriod_ = 0.04f;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j = 0; j < Transform_.Width; j++)
        {
            Str += L"X";
        }
        RenderString_.push_back(Str);
    }
}

Monster::Monster(int InX, int InY)
{
    Transform_.Position = Vector2{ InX, InY };
    Transform_.Delta = Vector2{ 1, 0 };
    Transform_.Width = 2;
    Transform_.Height = 2;
    NextPosition_ = Transform_.Position;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Monster;

    //UpdatePeriod_ = 0.016f;
    UpdatePeriod_ = 0.02f;
    GravityPeriod_ = 0.03f;

    RenderString_.reserve(Transform_.Width * Transform_.Height);
    for (int i = 0; i < Transform_.Height; i++)
    {
        std::wstring Str{};
        for (int j = 0; j < Transform_.Width; j++)
        {
            //Str += L"X";
            Str += std::to_wstring(rand() % 10);
        }
        RenderString_.push_back(Str);
    }
}

void Monster::Update()
{
    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (UpdateTimer_ >= UpdatePeriod_)
    {
        UpdateTimer_ -= UpdatePeriod_;

        Transform_.Delta.X = (Direction_ & Direction::Right) == Direction::None ? -1 : 1;
        NextPosition_ = Transform_.Position + Transform_.Delta;
    }
}

void Monster::Update(int Gravity)
{
    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (UpdateTimer_ >= UpdatePeriod_)
    {
        UpdateTimer_ -= UpdatePeriod_;

        Transform_.Delta.X = (Direction_ & Direction::Right) == Direction::None ? -1 : 1;
        NextPosition_ = Transform_.Position + Transform_.Delta;
    }

    GravityTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (bUseGravity_ && GravityTimer_ >= GravityPeriod_)
    {
        GravityTimer_ -= GravityPeriod_;
        NextPosition_.Y += Gravity;
        Logger::Log(0, { "Monster's Delta Y : " + std::to_string(NextPosition_.Y) });
    }
}

void Monster::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        return;
    }

    if (Other->GetCollisionLayer() == CollisionLayer::Player)
    {
        bIsDestroyed_ = true;
    }
    else if (Other->GetCollisionLayer() == CollisionLayer::Monster)
    {
        TurnAround();
    }
    else if (Other->GetCollisionLayer() == CollisionLayer::Ground
             && Other->GetPosition().Y < Transform_.Position.Y)
    {
        TurnAround();
    }
}

void Monster::OnCollisionStay(GameObject* Other)
{
}

void Monster::OnCollisionExit(GameObject* Other)
{
}

void Monster::TurnAround()
{
    Direction_ = (Direction_ & Direction::Right) == Direction::None ? Direction::Right : Direction::Left;
}
