#pragma once
#include "GameObject.h"
#include <unordered_map>

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(Faction InFaction, BulletType InBulletType);
    //Bullet(int InX, int InY, int InDeltaX, int InDeltaY);
    //Bullet(const Transform& InTransform, const Vector2& InDelta, Faction InFaction, BulletType InBulletType = BulletType::Default);
    //Bullet(const Vector2& InTransform, const Vector2& InDelta, Faction InFaction, BulletType InBulletType);
    //Bullet(const Transform& InTransform, const Vector2 InDelta, const Faction InFaction, int InHp = 1, BulletType InBulletType = BulletType::Default);

    virtual void Initialize(const Transform& InTransform, const Vector2& InDelta) override;
    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;

    inline Faction GetFaction() const { return Faction_; }
    inline void SetFaction(Faction InFaction) { Faction_ = InFaction; }

private:
    struct BulletSpec
    {
        //BulletType Type;
        size_t Width;
        size_t Height;
        int Hp;
        int Damage;
        float Speed;
        std::vector<std::wstring> RenderString;
        Vector2 BarrelOffset;
    };

    inline static const std::unordered_map<BulletType, BulletSpec> BulletSpecs = {
        {
            BulletType::Default,
            {
                1,
                2,
                1,
                1,
                1.0f,
                { L"█", L"▒" },
                { 0, 2 }
            }
        },
        {
            BulletType::Upgrade_1,
            {
                2,
                2,
                1,
                1,
                1.0f,
                { L"██", L"▒▒" },
                { 0, 2 }
            }
        },
        {
            BulletType::Upgrade_2,
            {
                3,
                2,
                2,
                2,
                1.0f,
                { L"███", L"▒▒▒" },
                { 0, 2 }
            }
        },
        {
            BulletType::Upgrade_3,
            {
                5,
                3,
                3,
                2,
                2.0f,
                { L"█ █ █", L"▓ ▓ ▓", L"▒ ▒ ▒" },
                { 0, 2 }
            }
        }
    };

    BulletType BulletType_ = BulletType::Default;
};
