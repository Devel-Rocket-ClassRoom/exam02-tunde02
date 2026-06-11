#include "Common.h"
#include"GameEngine.h"
#include <iostream>

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

Transform& Transform::operator=(const Transform& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->Position = other.Position;
    this->Delta = other.Delta;
    this->Width = other.Width;
    this->Height = other.Height;

    return *this;
}

Collider::Collider(const Vector2& Position, size_t InWidth, size_t InHeight)
{
    X = Position.X;
    Y = Position.Y;
    Width = InWidth;
    Height = InHeight;
}

void Collider::Initialize(const Transform& InTransform)
{
    X = InTransform.Position.X;
    Y = InTransform.Position.Y;
    Width = InTransform.Width;
    Height = InTransform.Height;
}

std::vector<std::string> Logger::Logs;

void Logger::Log(const std::string InLog)
{
    Logs.push_back(InLog);
    PrintLog();
}

void Logger::Log(int InRow, const std::string InLog)
{
    if (Logs.size() > InRow)
    {
        Logs[InRow] = InLog;
    }
    else
    {
        Logs.push_back(InLog);
    }
    PrintLog();
}

void Logger::AppendLog(int InRow, const std::string InLog)
{
    Logs[InRow] += InLog;
    PrintLog();
}

void Logger::PrintLog()
{
    GameEngine::Instance().SetCursorPosition(0, 26);
    for (int i = 0; i < 10; i++)
    {
        printf("          ");
    }
    GameEngine::Instance().SetCursorPosition(0, 26);

    size_t Size = Logs.size();
    printf("[From Logger] ");
    for (size_t i = 0; i < Size; i++)
    {
        printf("%s\n", Logs[i].c_str());
    }
}
