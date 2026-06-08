#include "Player.h"
#include "GameEngine.h"
#include <Windows.h>

Player::Player()
{
    Transform_.Position = Vector2{ 5, 5 };
    Transform_.Width = 3;
    Transform_.Height = 3;

    Collider_.Initialize(Transform_);
    CollisionLayer_ = CollisionLayer::Player;

    bUseGravity_ = false;

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

void Player::Update(int Gravity)
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

    // Gravity
    if (bUseGravity_ && JumpGauge <= 0.0f + FLT_EPSILON)
    {
        Transform_.Delta.Y += Gravity;
    }

    // 점프
    // space 꾹 -> 게이지가 오른다. 최대치가 있다
    //      - 이전에 스페이스를 누르고 있었는지를 알아야함
    //      - 공중에선 눌러도 점프할 수 없어야함
    // IsReachedHigh - > 게이지가 최대치에 도달한 적이 있는가?
    // deltaY[] -> (int)게이지 인덱스에 따라 다른 값
    // 
    if (GetAsyncKeyState(VK_SPACE))
    {
        //Transform_.Delta.Y = -2;

        if (!bUseGravity_ && JumpGauge <= 0.0f + FLT_EPSILON)
        {
            //JumpGauge += GameEngine::Instance().GetFixedDeltaTime();
            JumpGauge = 1.0f;
        }
    }

    if (JumpGauge >= 0.0f + FLT_EPSILON)
    {
        JumpGauge -= GameEngine::Instance().GetFixedDeltaTime() * 10;
        Transform_.Delta.Y = JumpDeltaYArray[(int)(JumpGauge * 10) / 10];
    }
    Logger::Log(2, "JumpGauge : " + std::to_string(JumpGauge));

    NextPosition_ = Transform_.Position + Transform_.Delta;
}

void Player::OnCollisionEnter(GameObject* Other)
{
}

void Player::OnCollisionExit(GameObject* Other)
{
}
