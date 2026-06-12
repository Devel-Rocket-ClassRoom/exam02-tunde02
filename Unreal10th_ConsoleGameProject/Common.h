#pragma once
//#include "BitFlags.h" // 다른 라이브러리의 비트 플래그 연산까지 덮어써버리는 것 같음
#include <type_traits>
#include <vector>
#include <string>

enum class GameObjectType
{
    None,
    Player,
    Monster,
    Bullet
};

enum class BulletType
{
    Default,
    Upgrade_1,
    Upgrade_2,
    Upgrade_3
};

enum class CollisionLayer
{
    None,
    Player,
    Wall,
    Monster,
    Bullet
};

enum class Direction
{
    None = 0,
    Up = 1 << 0,
    Down = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3
};

enum class Faction
{
    None = 1 << 0,
    Player = 1 << 1,
    Monster = 1 << 2
};

struct Vector2
{
    int X = 0;
    int Y = 0;

    Vector2() = default;
    Vector2(int InX, int InY) : X(InX), Y(InY) {}

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(const int multiplier) const;
    Vector2& operator=(const Vector2& other);
};

struct Transform
{
    Vector2 Position{};
    Vector2 Delta{};
    size_t Width = 0;
    size_t Height = 0;

    Transform() = default;
    Transform(int InX, int InY, int InDeltaX, int InDeltaY, size_t InWidth, size_t InHeight);

    Transform& operator=(const Transform& other);
};

struct Collider
{
    int X = 0;
    int Y = 0;
    size_t Width = 0;
    size_t Height = 0;
    CollisionLayer Layer = CollisionLayer::None;

    Collider() = default;
    Collider(const Vector2& InPosition, size_t InWidth, size_t InHeight, CollisionLayer InLayer);
    Collider(const Transform& InTransform, CollisionLayer InLayer);

    void Initialize(const Transform& InTransform);
};

inline Direction operator&(Direction Left, Direction Right)
{
    return static_cast<Direction>(
        static_cast<std::underlying_type_t<Direction>>(Left)
        & static_cast<std::underlying_type_t<Direction>>(Right));
}

inline Direction operator|(Direction Left, Direction Right)
{
    return static_cast<Direction>(
        static_cast<std::underlying_type_t<Direction>>(Left)
        | static_cast<std::underlying_type_t<Direction>>(Right));
}

inline Direction operator~(Direction InDirection)
{
    return static_cast<Direction>(~static_cast<std::underlying_type_t<Direction>>(InDirection));
}

inline bool HasFlag(Direction InDirection, Direction Flag)
{
    return (static_cast<std::underlying_type_t<Direction>>(InDirection) & static_cast<std::underlying_type_t<Direction>>(Flag)) != 0;
}

inline Vector2 CalcDeltaVector(Direction InDirection)
{
    Vector2 Delta{};

    if (HasFlag(InDirection, Direction::Up))
    {
        Delta = Delta + Vector2{ 0, -1 };
    }
    if (HasFlag(InDirection, Direction::Down))
    {
        Delta = Delta + Vector2{ 0, 1 };
    }
    if (HasFlag(InDirection, Direction::Left))
    {
        Delta = Delta + Vector2{ -1, 0 };
    }
    if (HasFlag(InDirection, Direction::Right))
    {
        Delta = Delta + Vector2{ 1, 0 };
    }

    return Delta;
}

class Logger
{
public:
    static std::vector<std::string> Logs;

    static void Log(const std::string InLog);
    static void Log(int InRow, const std::string InLog);
    static void AppendLog(int InRow, const std::string InLog);
    static void PrintLog();
};
