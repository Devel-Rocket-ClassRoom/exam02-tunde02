#pragma once
#include "BaseScene.h"

/// <summary>
/// 전체 게임을 실행하고 관리하는 싱글톤 클래스
/// </summary>
class GameEngine
{
public:
    ~GameEngine();

    // 외부 복사 및 대입 원천 차단
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // 전역 변수 객체
    static GameEngine& Instance();

    /// <summary>
    /// 새 씬을 추가하는 함수
    /// </summary>
    /// <param name="Scene">추가할 씬을 가리키는 BaseScene 포인터</param>
    void AddNewScene(BaseScene* Scene);


    /// <summary>
    /// 현재 씬을 매개변수로 받은 씬 타입인 씬으로 변경하는 함수
    /// </summary>
    /// <param name="InSceneType">변경할 씬의 타입 열거체</param>
    void ChangeScene(SceneType InSceneType);

    /// <summary>
    /// 게임 진행 (Update()) 및 출력 (Render())을 매 프레임 실행하는 함수
    /// </summary>
    void Run();

    /// <summary>
    /// 현재 씬의 Update()를 호출하여 결과적으로 씬의 오브젝트들을 프레임단위로 동작하게끔 하는 함수
    /// </summary>
    void Update();

    /// <summary>
    /// 현재 씬의 Render()를 호출하여 결과적으로 씬이 스크린을 출력하여 화면에 보여주게끔 하는 함수
    /// </summary>
    void Render();

    /// <summary>
    /// 현재 씬의 Instantiate() 함수를 호출하여 결과적으로 매개변수로 전달받은 오브젝트를 전달받은 위치, 운동 방향, 생성 시간에 생성하도록 하는 함수
    /// </summary>
    /// <param name="InGameObject">생성할 오브젝트를 가리키는 GameObject 포인터</param>
    /// <param name="InPosition">오브젝트를 생성할 위치</param>
    /// <param name="InDelta">생성된 오브젝트가 가질 운동 방향</param>
    /// <param name="InTimer">오브젝트를 생성할 시간</param>
    /// <returns></returns>
    GameObject* Instantiate(GameObject* InGameObject, const Vector2& InPosition, const Vector2& InDelta, float InTimer = 0.0f);

    inline float GetFixedDeltaTime() const { return FixedDeltaTime; }
    inline int GetScreenWidth() const { return ScreenWidth; }
    inline int GetScreenHeight() const { return ScreenHeight; }

private:
    const int TargetFPS = 120;
    int ScreenWidth = 83;
    int ScreenHeight = 48;
    float FixedDeltaTime = 0.0f;
    int FrameMilliseconds = 0;
    std::vector<BaseScene*> Scenes;
    int CurrentSceneIndex = 0;
    BaseScene* CurrentScene = nullptr; // 현재 활성화된 씬 포인터

    // 생성자를 private으로 감춰 외부 선언 및 new 차단
    GameEngine();

    void HideCursor();

    /// <summary>
    /// 콘솔창의 크기, 시작 위치를 수정하는 함수
    /// </summary>
    /// <param name="Width">콘솔창의 가로 픽셀 길이</param>
    /// <param name="Height">콘솔창의 세로 픽셀 길이</param>
    void SetConsoleSize(int Width, int Height);

    /// <summary>
    /// 실제로 현재 씬을 매개변수로 받은 씬으로 바꾸는 함수
    /// </summary>
    /// <param name="Scene">바꿀 씬을 가리키는 BaseScene 포인터</param>
    void ChangeScene(BaseScene* Scene);

    /// <summary>
    /// 출력 커서의 위치를 (X, Y)로 변경하는 함수
    /// </summary>
    /// <param name="X">변경할 커서의 X좌표 값</param>
    /// <param name="Y">변경할 커서의 Y좌표 값</param>
    void SetCursorPosition(int X, int Y);
};
