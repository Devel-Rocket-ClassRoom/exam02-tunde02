#pragma once
#include "Common.h"
#include <vector>
#include <string>
#include <unordered_set>

/// <summary>
/// 최상위 게임 오브젝트 추상 클래스
/// </summary>
class GameObject
{
public:
    virtual ~GameObject() = default;

    // Engine Functions
    virtual void Update() = 0;
    virtual void ApplyMove();
    virtual void CancelMove();
    virtual void CancelXMove();
    virtual void CancelYMove();
    virtual void Render() {}
    virtual void OnCollisionEnter(GameObject* Other) {}
    virtual void OnCollisionStay(GameObject* Other) {}
    virtual void OnCollisionExit(GameObject* Other) {}

    // Instantiate Function
    virtual void Initialize(const Transform InTransform, const Vector2 InDelta);

    void UpdateCollisions();
    void TakeDamage(int InDamage);

    inline void Destroy() { bIsDestroyed_ = true; }
    inline bool IsDestroyed() const { return bIsDestroyed_; }
    inline void AddCurrentCollision(GameObject* Other) { CurrentCollisions.insert(Other); }
    inline bool WasCollidedWith(GameObject* Other) { return PrevCollisions.count(Other) > 0; }
    inline bool IsCollidedWith(GameObject* Other) { return CurrentCollisions.count(Other) > 0; }

    inline CollisionLayer GetCollisionLayer() const { return Collider_.Layer; }
    inline Faction GetFaction() const { return Faction_; }
    inline std::unordered_set<GameObject*> GetCurrentCollisions() const { return CurrentCollisions; }
    inline std::unordered_set<GameObject*> GetPrevCollisions() const { return PrevCollisions; }
    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }
    inline int GetNextMinX() const { return NextPosition_.X; }
    inline int GetNextMaxX() const { return NextPosition_.X + static_cast<int>(Transform_.Width); }
    inline int GetNextMinY() const { return NextPosition_.Y; }
    inline int GetNextMaxY() const { return NextPosition_.Y + static_cast<int>(Transform_.Height); }
    inline Vector2 GetDelta() const { return Delta_; }
    inline void SetDelta(Vector2 InDelta) { Delta_ = InDelta; }
    inline Direction GetDeltaDirection() const { return DeltaDirection; }
    inline void AddDeltaDirection(Direction InDirection) { DeltaDirection = DeltaDirection | InDirection; }
    inline void SubDeltaDirection(Direction InDirection) { DeltaDirection = DeltaDirection & ~InDirection; }
    inline int GetHp() const { return Hp; }
    inline void SetHp(int InHp) { Hp = InHp; }
    inline int GetDamage() const { return Damage; }
    inline void SetDamage(int InDamage) { Damage = InDamage; }
    inline std::vector<std::wstring> GetRenderingVector() const { return RenderString_; }

protected:
    Transform Transform_{};
    Vector2 Delta_{};
    Direction DeltaDirection = Direction::None;
    Vector2 NextPosition_{};
    Collider Collider_{};
    Faction Faction_ = Faction::None;
    std::unordered_set<GameObject*> CurrentCollisions;
    std::unordered_set<GameObject*> PrevCollisions;
    int Hp = 0;
    int Damage = 0;
    float Speed = 0.0f;
    bool bIsDestroyed_ = false; // 지연 삭제용 플래그
    float UpdatePeriod_ = 0.0f;
    float UpdateTimer_ = 0.0f;
    std::vector<std::wstring> RenderString_;

    inline void CalcNextPosition() { NextPosition_ = Transform_.Position + CalcDeltaVector(DeltaDirection); }
};
