#pragma once
#include "GameObject.h"
#include "Player.h"
#include <unordered_map>

class Item : public GameObject
{
public:
    Item(ItemType InItemType);

    virtual void Initialize(const Transform& InTransform, const Vector2& InDelta) override;
    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;

private:
    void ResolveItemEffect(Player* InPlayer);

    ItemType ItemType_ = ItemType::None;

    struct ItemSpec
    {
        size_t Width;
        size_t Height;
        std::vector<std::wstring> RenderString;
    };

    inline static const std::unordered_map<ItemType, ItemSpec> ItemSpecs = {
        {
            ItemType::Recovery,
            {
                5,
                4,
                {
                    L" ▓ ▓ ",
                    L"▒███▒",
                    L" ▒█▒ ",
                    L"  ░  "
                }
            }
        },
        {
            ItemType::BulletUpgrade,
            {
                5,
                4,
                {
                    L"┌───┐",
                    L"│█↑█│",
                    L"│███│",
                    L"└───┘"
                }
            }
        }
    };
};
