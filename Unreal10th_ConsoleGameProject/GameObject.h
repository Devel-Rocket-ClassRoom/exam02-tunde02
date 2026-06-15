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

    /// <summary>
    /// 씬에 생성됐을 때 실행될 초기화 함수
    /// </summary>
    /// <param name="InPosition">생성될 위치</param>
    /// <param name="InDelta">생성 시 가질 운동 방향</param>
    virtual void Initialize(const Vector2& InPosition, const Vector2& InDelta);

    /// <summary>
    /// 씬에 의해 매 프레임 실행될 로직 함수
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// X축 방향으로 InX만큼 이동하는 함수
    /// </summary>
    /// <param name="InX">이동할 X축 길이</param>
    virtual void ApplyXMove(float InX);

    /// <summary>
    /// Y축 방향으로 InX만큼 이동하는 함수
    /// </summary>
    /// <param name="InY">이동할 Y축 길이</param>
    virtual void ApplyYMove(float InY);

    /// <summary>
    /// 씰에 의해 매 프레임 실행될 렌더링 함수
    /// </summary>
    virtual void Render() {}

    /// <summary>
    /// 충돌이 감지되면 씬에 의해 실행되는 함수
    /// </summary>
    /// <param name="Other">GameObject 포인터 형식의 충돌한 오브젝트</param>
    virtual void OnCollisionEnter(GameObject* Other) {}

    /// <summary>
    /// InDamage 만큼 피해를 입는 함수
    /// </summary>
    /// <param name="InDamage">입을 피해량</param>
    virtual void TakeDamage(int InDamage);

    inline void Destroy() { Transform_.Width = 0; Transform_.Height = 0; bIsDestroyed_ = true; }
    inline bool IsDestroyed() const { return bIsDestroyed_; }

    /// <summary>
    /// 오브젝트가 현재 충돌할 수 있는 상태인지 반환하는 함수
    /// </summary>
    /// <returns>충돌할 수 있다면 true, 아니라면 false</returns>
    inline bool CanCollide() const { return bCanCollide; }

    inline CollisionLayer GetCollisionLayer() const { return CollisionLayer_; }
    inline Faction GetFaction() const { return Faction_; }
    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }

    inline Vector2 GetDelta() const { return Delta_; }
    inline void SetDelta(Vector2 InDelta) { Delta_ = InDelta; }
    inline float GetSpeed() const { return Speed; }
    inline void SetSpeed(float InSpeed) { Speed = InSpeed; }
    inline int GetHp() const { return Hp; }
    inline void SetHp(int InHp) { Hp = InHp; }
    inline int GetDamage() const { return Damage; }
    inline void SetDamage(int InDamage) { Damage = InDamage; }
    inline std::vector<std::wstring> GetRenderingVector() const { return RenderString_; }

protected:
    Transform Transform_{};
    Vector2 Delta_{};
    Vector2 NextPosition_{};
    CollisionLayer CollisionLayer_ = CollisionLayer::None;
    Faction Faction_ = Faction::None;
    int Hp = 0;
    int Damage = 0;
    float Speed = 0.0f;
    bool bIsDestroyed_ = false; // 지연 삭제용 플래그
    bool bCanCollide = true; // 충돌 가능 여부 플래그
    float UpdatePeriod_ = 0.0f; // 업데이트 함수를 호출할 주기
    float UpdateTimer_ = 0.0f; // 업데이트 함수를 호출하기 위해 사용하는 타이머
    std::vector<std::wstring> RenderString_;

    inline void UpdateNextPosition() { NextPosition_ = Transform_.Position + Delta_; }
};
