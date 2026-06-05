#include "Monster.h"
#include "GameEngine.h"

Monster::Monster()
{
    Transform_.Width = 3;
    Transform_.Height = 3;
    Transform_.Position = Vector2{ 7, 0 };
    Transform_.Delta = Vector2{ 1, 0 };

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
        //TurnAround();
        ShouldUpdated_ = true;
        UpdateTimer_ -= UpdatePeriod_;
    }
}

void Monster::OnCollisionEnter(GameObject* Other)
{
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%d", CollisionCount);
    if (Other != nullptr && Other->IsPlayer())
    {
        CollisionCount++;
        for (int i = 0; i < Transform_.Height; i++)
        {
            std::wstring Str{};
            for (int j = 0; j < Transform_.Width; j++)
            {
                Str += std::to_wstring(CollisionCount);
            }
            RenderString_.push_back(Str);
        }
    }
    else
    {
        TurnAround();
    }
}

void Monster::OnCollisionExit(GameObject* Other)
{
}
