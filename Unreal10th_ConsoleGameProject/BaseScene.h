#pragma once
#include "GameObject.h"
#include "Player.h"
#include <vector>

/// <summary>
/// 씬 관리를 위한 부모 클래스
/// </summary>
class BaseScene
{
protected:
    /// <summary>
    /// 오브젝트 생성 요청에 대한 정보를 가진 구조체
    /// </summary>
    struct InstantiateRequest
    {
        GameObject* Object = nullptr;
        Vector2 ObjectPosition{};
        Vector2 ObjectDelta{};
        float Timer = 0.0f;

        InstantiateRequest() = default;
        InstantiateRequest(GameObject* InObject, const Vector2& InObjectPosition, const Vector2& InObjectDelta, float InTimer = 0.0f) : Object(InObject), ObjectPosition(InObjectPosition), ObjectDelta(InObjectDelta), Timer(InTimer) {}
    };

    /// <summary>
    /// 렌더링할 문자열 벡터
    /// </summary>
    std::vector<std::wstring> Screen;

    /// <summary>
    /// 독립적으로 관리하는 씬의 오브젝트 벡터
    /// </summary>
    std::vector<GameObject*> SceneObjects;

    /// <summary>
    /// 다음 프레임에 생성해야 하는 생성 요청 벡터
    /// </summary>
    std::vector<InstantiateRequest> InstantiateRequests;

    Player* Player_ = nullptr;
    size_t Width_ = 0; // 화면에 출력할 수 있는 행 개수
    size_t Height_ = 0; // 화면에 출력할 수 있는 열 개수
    size_t GameAreaWidth = 60; // 실제로 플레이어가 움직일 수 있는 행의 길이
    size_t GameAreaHeight = 46; // 실제로 플레이어가 움직일 수 있는 열의 길이
    int Score = 0;

    /// <summary>
    /// 렌더링할 Screen을 화면의 행, 열 만큼 초기화하는 함수
    /// </summary>
    /// <param name="C">초기화할 문자열</param>
    void InitializeScreen(const std::wstring C = L" ");

    /// <summary>
    /// 씬이 가진 오브젝트들의 문자열을 Screen의 각 위치에 갱신하는 함수
    /// </summary>
    void RenderSceneObjects();

    /// <summary>
    /// UI를 화면 중앙에 그리고자 할 때 시작 열 인덱스를 반환하는 함수
    /// </summary>
    /// <param name="UiStartX">그리고자 하는 화면의 시작 열 인덱스</param>
    /// <param name="Length">그리고자 하는 문자열의 길이</param>
    /// <returns>문자열을 그릴 시작 열 인덱스</returns>
    size_t GetTextStartX(const size_t UiStartX, const size_t Length) const;

    /// <summary>
    /// 모든 UI 및 오브젝트가 그려진 Screne을 버퍼에 넣어 한 번에 출력하는 함수
    /// </summary>
    void PrintScreen();

public:
    BaseScene(int Width, int Height);
    virtual ~BaseScene();

    /// <summary>
    /// 씬 시작 시 실행되는 함수 (데이터 로드, 배치)
    /// </summary>
    virtual void Enter() = 0;

    /// <summary>
    /// 씬 종료 시 (자원 해제)
    /// </summary>
    virtual void Exit() = 0;

    /// <summary>
    /// 매개변수로 전달받은 오브젝트를 전달받은 위치, 운동 방향, 생성 시간이 지난 이후의 프레임에 실제로 SceneObjects에 추가하도록 하는 InstantiateRequest 구조체를 만들어 벡터에 추가하는 함수
    /// </summary>
    /// <param name="InGameObject">생성할 오브젝트를 가리키는 GameObject 포인터</param>
    /// <param name="InPosition">오브젝트를 생성할 위치</param>
    /// <param name="InDelta">생성된 오브젝트가 가질 운동 방향</param>
    /// <param name="InTimer">오브젝트를 생성할 시간</param>
    /// <returns></returns>
    GameObject* Instantiate(GameObject* InGameObject, const Vector2& InPosition, const Vector2& InDelta, float InTimer = 0.0f);

    /// <summary>
    /// 현재 씬에 속한 오브젝트들의 업데이트 및 지연 삭제를 처리하는 함수
    /// </summary>
    virtual void Update();

    /// <summary>
    /// 현재 씬의 오브젝트들을 렌더링하여 Screen을 채우는 함수
    /// </summary>
    virtual void Render();

private:
    /// <summary>
    /// X축에 대하여, 오브젝트가 가진 운동 방향만큼 한 번씩 움직여가며 움직일 때마다 외곽 및 충돌을 검사한 뒤 실제로 이동시키는 함수
    /// </summary>
    /// <param name="ObjA">움직일 오브젝트를 가리키는 GameObject 포인터</param>
    void TryXMove(GameObject* ObjA);

    /// <summary>
    /// Y축에 대하여, 오브젝트가 가진 운동 방향만큼 한 번씩 움직여가며 움직일 때마다 외곽 및 충돌을 검사한 뒤 실제로 이동시키는 함수
    /// </summary>
    /// <param name="ObjA">움직일 오브젝트를 가리키는 GameObject 포인터</param>
    void TryYMove(GameObject* ObjA);

    /// <summary>
    /// AABB 충돌 알고리즘을 이용해 충돌 여부를 반환하는 함수
    /// </summary>
    /// <param name="ObjAPosX">A 오브젝트의 X좌표</param>
    /// <param name="ObjAPosY">A 오브젝트의 Y좌표</param>
    /// <param name="ObjAWidth">A 오브젝트의 가로 길이</param>
    /// <param name="ObjAHeight">A 오브젝트의 세로 길이</param>
    /// <param name="ObjBPosX">B 오브젝트의 X좌표</param>
    /// <param name="ObjBPosY">B 오브젝트의 Y좌표</param>
    /// <param name="ObjBWidth">B 오브젝트의 가로 길이</param>
    /// <param name="ObjBHeight">B 오브젝트의 세로 길이</param>
    /// <returns>검사 결과 A 오브젝트와 B 오브젝트가 충돌한 상태라면 true, 아니라면 false</returns>
    bool CheckAABBCollision(int ObjAPosX, int ObjAPosY, size_t ObjAWidth, size_t ObjAHeight, int ObjBPosX, int ObjBPosY, size_t ObjBWidth, size_t ObjBHeight);

    /// <summary>
    /// A 오브젝트와 B 오브젝트가 플레이어와 아이템인지 여부를 반환하는 함수
    /// </summary>
    /// <param name="ObjA">검사할 A 오브젝트를 가리키는 GameObject 포인터</param>
    /// <param name="ObjB">검사할 B 오브젝트를 가리키는 GameObject 포인터</param>
    /// <returns>A 오브젝트가 플레이어, B 오브젝트가 아이템 혹은 A 오브젝트가 아이템, B 오브젝트가 플레이어라면 true, 나머지 경우엔 false</returns>
    inline bool IsPlayerAndItem(const GameObject* ObjA, const GameObject* ObjB)
    {
        return (ObjA->GetCollisionLayer() == CollisionLayer::Player && ObjB->GetCollisionLayer() == CollisionLayer::Item)
            || (ObjA->GetCollisionLayer() == CollisionLayer::Item && ObjB->GetCollisionLayer() == CollisionLayer::Player);
    }
};
