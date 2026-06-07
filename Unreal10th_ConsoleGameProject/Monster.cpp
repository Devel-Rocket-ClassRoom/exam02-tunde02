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
        bIsDestroyed_ = true;
#if 0
        printf("Monster.OnCollisionEnter() - Collide with Player : %d\n", CollisionCount);
        CollisionCount++;
        for (int i = 0; i < Transform_.Height; i++)
        {
            for (int j = 0; j < Transform_.Width; j++)
            {
                RenderString_[i][j] = CollisionCount % 10 + L'0';
            }
        }
        TurnAround();
#endif
    }
    else
    {
        printf("Monster.OnCollisionEnter() - Collide with outlin : %d\n", CollisionCount);
        CollisionCount++;
        TurnAround();
    }
}

void Monster::OnCollisionStay(GameObject* Other)
{
    CollisionCount++;
    printf("Monster.OnCollisionStay() - %d\n", CollisionCount);
}

void Monster::OnCollisionExit(GameObject* Other)
{
    printf("Monster.OnCollisionExit()\n");
}

void Monster::TurnAround()
{
    Direction_ = (Direction_ & Direction::Right) == Direction::None ? Direction::Right : Direction::Left;
}
