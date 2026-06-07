#pragma once
#include "Common.h"
#include <vector>
#include <string>

/// <summary>
/// 최상위 게임 오브젝트 추상 클래스
/// </summary>
class GameObject
{
protected:
    Transform Transform_{};
    Vector2 NextPosition_{};
    Collider Collider_{};
    CollisionLayer CollisionLayer_ = CollisionLayer::None;
    bool bIsDestroyed_ = false; // 지연 삭제용 플래그
    float UpdatePeriod_ = 0.0f;
    float UpdateTimer_ = 0.0f;
    std::vector<std::wstring> RenderString_;

public:
    virtual ~GameObject() = default;

    virtual void Update() = 0;
    virtual void ApplyMove();
    virtual void CancelMove();
    virtual void Render() {}
    virtual void OnCollisionEnter(GameObject* Other) {}
    virtual void OnCollisionStay(GameObject* Other) {}
    virtual void OnCollisionExit(GameObject* Other) {}

    inline void Destroy() { bIsDestroyed_ = true; }
    inline bool IsDestroyed() const { return bIsDestroyed_; }

    inline CollisionLayer GetCollisionLayer() const { return CollisionLayer_; }
    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }
    inline int GetNextMinX() const { return NextPosition_.X; }
    inline int GetNextMaxX() const { return NextPosition_.X + static_cast<int>(Transform_.Width); }
    inline int GetNextMinY() const { return NextPosition_.Y; }
    inline int GetNextMaxY() const { return NextPosition_.Y + static_cast<int>(Transform_.Height); }
    inline std::vector<std::wstring> GetRenderingVector() const { return RenderString_; }
};
