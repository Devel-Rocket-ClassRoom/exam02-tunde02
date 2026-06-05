#pragma once
#include "Transform.h"
#include <vector>
#include <string>

// ==========================================
// 1. 최상위 게임 오브젝트 추상 클래스
// ==========================================
class GameObject
{
protected:
    Transform Transform_{};
    bool IsDestroyed_ = false; // 지연 삭제용 플래그
    bool ShouldUpdated_ = false;
    float UpdatePeriod_ = 0.0f;
    float UpdateTimer_ = 0.0f;
    std::vector<std::wstring> RenderString_;

    // Delete me
    bool IsPlayer_ = false;

public:
    virtual ~GameObject() = default;

    virtual void Update() = 0;
    virtual void Render() {}
    virtual void OnCollisionEnter(GameObject* Other) = 0;
    virtual void OnCollisionExit(GameObject* Other) = 0;

    inline void Destroy() { IsDestroyed_ = true; }
    inline bool IsDestroyed() const { return IsDestroyed_; }

    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }
    inline bool ShouldUpdated() const { return ShouldUpdated_; }
    inline void SetShouldUpdated(bool InShouldUpdated) { ShouldUpdated_ = InShouldUpdated; }
    inline std::vector<std::wstring> GetRenderingVector() const { return RenderString_; }

    // Delete me
    inline bool IsPlayer() const { return IsPlayer_; }
};
