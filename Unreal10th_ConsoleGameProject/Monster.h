#pragma once
#include "GameObject.h"
#include <unordered_map>

class Monster : public GameObject
{
public:
    Monster();
    Monster(int InX, int InY);
    Monster(MonsterType InMonsterType);
    Monster(MonsterType InMonsterType, Vector2 InDelta);

    virtual void Initialize(const Vector2& InPosition, const Vector2& InDelta) override;
    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;

    inline MonsterType GetMonsterType() const { return MonsterType_; }

private:
    /// <summary>
    /// 외곽에 충돌하면 X축 진행 방향을 반대로 바꾸는 함수
    /// </summary>
    void TurnAround();

    void FireBullet() const;

    MonsterType MonsterType_ = MonsterType::Default;
    BulletType BulletType_ = BulletType::Default;
    float MonsterShotDelay = 0.1f;
    float ShotDelay = 0.0f;

    /// <summary>
    /// 몬스터의 타입에 따라 가질 스펙 정보를 가진 구조체
    /// </summary>
    struct MonsterSpec
    {
        MonsterType Type;
        size_t Width;
        size_t Height;
        int Hp;
        int Damage;
        float Speed;
        float ShotDelay;
        BulletType BulletType_;
        std::vector<std::wstring> RenderString;
    };

    inline static const std::unordered_map<MonsterType, MonsterSpec> MonsterSpecs = {
        {
            MonsterType::Default,
            {
                MonsterType::Default,
                5,
                3,
                5,
                1,
                1.0f,
                0.3f,
                BulletType::Default,
                {
                    L"█ ░ █",
                    L"▓███▓",
                    L" ░▓░ "
                }
            }
        },
        {
            MonsterType::TripleShot,
            {
                MonsterType::TripleShot,
                7,
                5,
                15,
                1,
                0.5f,
                0.3f,
                BulletType::Default,
                {
                    L"  ░█░  ",
                    L" ░▓█▓░ ",
                    L"▒▓███▓▒",
                    L" ░▓▓▓░ ",
                    L" █ █ █ "
                }
            }
        },
        {
            MonsterType::Settled,
            {
                MonsterType::Settled,
                7,
                5,
                40,
                2,
                0.0f,
                0.3f,
                BulletType::Upgrade_2,
                {
                    L"   ░   ",
                    L"▒ ▓▓▓ ▒",
                    L" ░▓█▓░ ",
                    L"▒ ▓▓▓ ▒",
                    L"   ░   "
                }
            }
        },
        {
            MonsterType::Boss,
            {
                MonsterType::Boss,
                43,
                16,
                800,
                5,
                0.2f,
                0.15f,
                BulletType::Upgrade_1,
                {
                    L"                  ░▒▓█▓▒░                  ",
                    L"                ░▒▓█████▓▒░                ",
                    L"            ░▒▓█████████████▓▒░            ",
                    L"        ░▒▓█████████████████████▓▒░        ",
                    L"    ░▒▓█████████████████████████████▓▒░    ",
                    L"▒▓█████████████▒▒▓▓█████▓▓▒▒█████████████▓▒",
                    L" ░▓▓███████████░░░░░███░░░░░███████████▓▓░ ",
                    L"     ░▒▓█████████████████████████▓▒░       ",
                    L"       ▒▓███████░░░░███░░░░███████▓▒       ",
                    L"      ░▓██████▓░    ░█░    ░▓██████▓░      ",
                    L"     ░▒▓████▓▒░             ░▒▓████▓▒░     ",
                    L"       ░▓██▓░                 ░▓██▓░       ",
                    L"        ░█░                    ░█░         ",
                    L"        ░█░                    ░█░         ",
                    L"        ░ ░                    ░ ░         ",
                    L"        ░ ░                    ░ ░         "
                }
            }
        },
    };
};
