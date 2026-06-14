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

    // Instantiate Function
    virtual void Initialize(const Transform& InTransform, const Vector2& InDelta);

    // Engine Functions
    virtual void Update() = 0;
    virtual void ApplyMove();
    virtual void ApplyMove(float InX, float InY);
    virtual void ApplyXMove(float InX);
    virtual void ApplyYMove(float InY);
    virtual void CancelMove();
    virtual void CancelXMove();
    virtual void CancelYMove();
    virtual void Render() {}
    virtual void OnCollisionEnter(GameObject* Other) {}
    virtual void OnCollisionStay(GameObject* Other) {}
    virtual void OnCollisionExit(GameObject* Other) {}
    virtual void TakeDamage(int InDamage);

    void UpdateCollisions();

    inline void Destroy() { Transform_.Width = 0; Transform_.Height = 0; bIsDestroyed_ = true; }
    inline bool IsDestroyed() const { return bIsDestroyed_; }
    inline bool CanCollide() const { return bCanCollide; }
    inline void AddCurrentCollision(GameObject* Other) { CurrentCollisions.insert(Other); }
    inline bool WasCollidedWith(GameObject* Other) { return PrevCollisions.count(Other) > 0; }
    inline bool IsCollidedWith(GameObject* Other) { return CurrentCollisions.count(Other) > 0; }

    inline CollisionLayer GetCollisionLayer() const { return CollisionLayer_; }
    inline Faction GetFaction() const { return Faction_; }
    inline std::unordered_set<GameObject*> GetCurrentCollisions() const { return CurrentCollisions; }
    inline std::unordered_set<GameObject*> GetPrevCollisions() const { return PrevCollisions; }
    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }
    inline int GetNextMinX() const { return static_cast<int>(NextPosition_.X); }
    inline int GetNextMaxX() const { return static_cast<int>(NextPosition_.X) + static_cast<int>(Transform_.Width); }
    inline int GetNextMinY() const { return static_cast<int>(NextPosition_.Y); }
    inline int GetNextMaxY() const { return static_cast<int>(NextPosition_.Y) + static_cast<int>(Transform_.Height); }
    inline Vector2 GetDelta() const { return Delta_; }
    inline void SetDelta(Vector2 InDelta) { Delta_ = InDelta; }
    inline float GetSpeed() const { return Speed; }
    inline void SetSpeed(float InSpeed) { Speed = InSpeed; }
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
    CollisionLayer CollisionLayer_ = CollisionLayer::None;
    //Collider Collider_{};
    Faction Faction_ = Faction::None;
    std::unordered_set<GameObject*> CurrentCollisions;
    std::unordered_set<GameObject*> PrevCollisions;
    int Hp = 0;
    int Damage = 0;
    float Speed = 0.0f;
    bool bIsDestroyed_ = false; // 지연 삭제용 플래그
    bool bCanCollide = true;
    float UpdatePeriod_ = 0.0f;
    float UpdateTimer_ = 0.0f;
    std::vector<std::wstring> RenderString_;

    inline void UpdateNextPosition() { NextPosition_ = Transform_.Position + Delta_; }
    inline void NormalizeDelta()
    {
        float len = std::sqrt(Delta_.X * Delta_.X + Delta_.Y * Delta_.Y);

        // 길이가 0인 경우(정지 상태) 나누기 에러 방지
        if (len == 0.0f)
        {
            Delta_.X = 0.0f;
            Delta_.Y = 0.0f;
            return;
        }

        Delta_.X /= len;
        Delta_.Y /= len;
    }
};
