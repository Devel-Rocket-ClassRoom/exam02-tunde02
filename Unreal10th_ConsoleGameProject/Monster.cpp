#include "Monster.h"
#include "GameEngine.h"

Monster::Monster()
{
    Transform_.Position = Vector2{ 7, 0 };
    Transform_.Delta = Vector2{ 1, 0 };
    Transform_.Width = 3;
    Transform_.Height = 3;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Monster;

    UpdatePeriod_ = 0.016f;

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

void Monster::OnCollisionEnter(GameObject* Other)
{
    if (Other != nullptr && Other->GetCollisionLayer() == CollisionLayer::Player)
    {
        printf("Monster.OnCollisionEnter() - Collide With Player : %d\n", CollisionCount);
        CollisionCount++;
        for (int i = 0; i < Transform_.Height; i++)
        {
            for (int j = 0; j < Transform_.Width; j++)
            {
                RenderString_[i][j] = CollisionCount % 10 + L'0';
            }
        }
    }
    else
    {
        printf("Monster.OnCollisionEnter() - Collide with outline. : %d\n", CollisionCount);
        CollisionCount++;
        TurnAround();
    }
}

void Monster::OnCollisionExit(GameObject* Other)
{
    printf("Monster.OnCollisionExit()\n");
}

void Monster::TurnAround()
{
    //Transform_.Delta.X *= -1;
    Direction_ = (Direction_ & Direction::Right) == Direction::None ? Direction::Right : Direction::Left;
}
