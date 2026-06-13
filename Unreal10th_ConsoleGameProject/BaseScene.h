#pragma once
#include "GameObject.h"
#include <vector>

// ==========================================
// 2. 씬 / 스테이지 관리를 위한 부모 추상 클래스
// ==========================================
class BaseScene
{
protected:
    struct InstantiateRequest
    {
        GameObject* Object = nullptr;
        Transform ObjectTransform{};
        Vector2 ObjectDelta{};
        float Timer = 0.0f;

        InstantiateRequest() = default;
        InstantiateRequest(GameObject* InObject, const Transform& InObjectTransform, const Vector2& InObjectDelta, float InTimer = 0.0f) : Object(InObject), ObjectTransform(InObjectTransform), ObjectDelta(InObjectDelta), Timer(InTimer) {}
        //InstantiateRequest(GameObject* InObject, const Transform& InObjectTransform, const Vector2& InObjectDelta) : Object(InObject), ObjectTransform(InObjectTransform), ObjectDelta(InObjectDelta) {}
    };

    // 각 씬은 자신만의 독립된 오브젝트 리스트를 관리함
    std::vector<std::wstring> Screen;
    std::vector<GameObject*> SceneObjects;
    std::vector<InstantiateRequest> InstantiateRequests;
    GameObject* Player_ = nullptr;
    size_t Width_ = 0;
    size_t Height_ = 0;

public:
    BaseScene(int Width, int Height);
    virtual ~BaseScene();

    virtual void Enter() = 0;  // 씬 시작 시 (데이터 로드, 배치)
    virtual void Exit() = 0;   // 씬 종료 시 (자원 해제)

    GameObject* Instantiate(GameObject* InGameObject, const Transform& InTransform, const Vector2& InDelta, float InTimer = 0.0f);
    GameObject* Instantiate(GameObjectType InGameObjectType, const Transform& InTransform, const Vector2& InDelta, float InTimer = 0.0f);

    // 현재 씬에 속한 오브젝트들 업데이트 및 지연 삭제 처리
    virtual void Update();

    // 현재 씬의 오브젝트들 렌더링
    virtual void Render();

private:
    void TryXMove(GameObject* ObjA, size_t i);
    void TryYMove(GameObject* ObjA, size_t i);
    bool CheckAABBCollision(const GameObject* ObjA, const GameObject* ObjB);
    bool CheckAABBCollision(int ObjAPosX, int ObjAPosY, size_t ObjAWidth, size_t ObjAHeight, int ObjBPosX, int ObjBPosY, size_t ObjBWidth, size_t ObjBHeight);
    void InitializeScreen(const std::wstring C = L" ");
    void RenderSceneObjects();
    void RenderStatus();
    size_t GetTextStartX(const size_t UiStartX, const size_t Length) const;
    void PrintScreen();
};
