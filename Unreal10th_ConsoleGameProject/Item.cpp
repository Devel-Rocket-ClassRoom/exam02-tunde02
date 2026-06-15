#include "Item.h"
#include "GameEngine.h"
#include "Player.h"

Item::Item(ItemType InItemType)
{
    ItemType_ = InItemType;
    ItemSpec Spec = ItemSpecs.at(ItemType_);

    Transform_.Width = Spec.Width;
    Transform_.Height = Spec.Height;
    CollisionLayer_ = CollisionLayer::Item;
    bCanCollide = false;
    Faction_ = Faction::Monster;
    Speed = 1.0f;
    UpdatePeriod_ = 0.2f;
    RenderString_ = Spec.RenderString;
}

void Item::Initialize(const Vector2& InPosition, const Vector2& InDelta)
{
    Transform_.Position = InPosition;
    Delta_ = InDelta;
    NextPosition_ = Transform_.Position;
}

void Item::Update()
{
    //UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    //if (UpdateTimer_ >= UpdatePeriod_)
    //{
    //    UpdateTimer_ -= UpdatePeriod_;

    //    UpdateNextPosition();
    //}
}

void Item::OnCollisionEnter(GameObject* Other)
{
    if (Other == nullptr)
    {
        Destroy();
    }
    else if (Other->GetCollisionLayer() == CollisionLayer::Player)
    {
        ResolveItemEffect(static_cast<Player*>(Other));
        Destroy();
    }
}

void Item::ResolveItemEffect(Player* InPlayer)
{
    switch (ItemType_)
    {
        case ItemType::Recovery:
            InPlayer->RecoverHp(2);
            break;
        case ItemType::BulletUpgrade:
            InPlayer->UpgradeBullet();
            break;
        case ItemType::None:
        default:
            break;
    }
}
