#pragma once
#include <type_traits>
#include <vector>
#include <string>

struct Vector2
{
    int X = 0;
    int Y = 0;

    Vector2() = default;
    Vector2(int InX, int InY) : X(InX), Y(InY) {}

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2& operator=(const Vector2& other);
};

struct Transform
{
    Vector2 Position{};
    Vector2 Delta{};
    size_t Width = 0;
    size_t Height = 0;

    Transform() = default;
};

struct Collider
{
    int X = 0;
    int Y = 0;
    size_t Width = 0;
    size_t Height = 0;

    Collider() = default;
    Collider(int InX, int InY, int InWidth, int InHeight) : X(InX), Y(InY), Width(InWidth), Height(InHeight) {}
    Collider(const Vector2& Position, size_t InWidth, size_t InHeight);

    void Initialize(const Transform& InTransform);
};

enum class CollisionLayer
{
    None,
    Player,
    Ground,
    Wall,
    Monster,
    Flag
};

enum class Direction
{
    None = 0,
    Up = 1 << 0,
    Down = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3
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

class Logger
{
public:
    static std::vector<std::string> Logs;

    static void Log(const std::string InLog);
    static void Log(int InRow, const std::string InLog);
    static void AppendLog(int InRow, const std::string InLog);
    static void PrintLog();
};
