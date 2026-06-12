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
    Collider_ = Collider(Transform_, CollisionLayer::Monster);

    //UpdatePeriod_ = 0.016f;
    UpdatePeriod_ = 0.2f;

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
    //Transform_.Delta = Vector2{ 0, 0 };
    DeltaDirection = Direction::Down | Direction::Left;
    Transform_.Width = 2;
    Transform_.Height = 2;
    Delta_ = Vector2{ 0, 0 };
    NextPosition_ = Transform_.Position;
    Collider_ = Collider(Transform_, CollisionLayer::Monster);
    Hp = 2;
    Speed = 1.0f;

    //UpdatePeriod_ = 0.016f;
    UpdatePeriod_ = 0.02f / Speed;

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

#if 0
        //Transform_.Delta.X = (Direction_ & Direction::Right) == Direction::None ? -1 : 1;
        //Transform_.Delta = Delta_;
        Transform_.Position.fX += Delta_.fX;
        Transform_.Position.fY += Delta_.fY;
        Transform_.Position.X = Transform_.Position.fX;
        Transform_.Position.Y = Transform_.Position.fY;
        //NextPosition_.fX += Transform_.Position.fX + Delta_.fX;
        //NextPosition_.fY += Transform_.Position.fY + Delta_.fY;
        NextPosition_ = Transform_.Position;
        //NextPosition_.X = NextPosition_.fX;
        //NextPosition_.Y = NextPosition_.fY;
        //printf("(%d, %d)\n", NextPosition_.X, NextPosition_.Y);
#endif
        CalcNextPosition();
    }
}

void Monster::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        return;
    }

    else if (Other->GetCollisionLayer() == CollisionLayer::Wall)
    {
        if (Other->GetPosition().Y > Transform_.Position.Y)
        {
            Destroy();
        }
        else
        {
            TurnAround();
            CalcNextPosition();
            //Transform_.Delta = Delta_;
            //NextPosition_ = Transform_.Position + Transform_.Delta;
        }
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
    bool isRight = HasFlag(DeltaDirection, Direction::Right);

    SubDeltaDirection(isRight ? Direction::Right : Direction::Left);
    AddDeltaDirection(isRight ? Direction::Left : Direction::Right);

#if 0
    Delta_.X *= -1;
#endif
}
