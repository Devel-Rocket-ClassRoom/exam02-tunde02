#pragma once
#include "BaseScene.h"

// ==========================================
// 4. 싱글톤 기반 게임 엔진 클래스
// ==========================================
class GameEngine
{
public:
    ~GameEngine();

    // 외부 복사 및 대입 원천 차단
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // 전역 어디서나 단 하나의 원본 객체에 접근할 수 있는 통로 (Meyers' Singleton)
    static GameEngine& Instance();
    void ChangeScene(BaseScene* newScene);
    void Run();
    void Update();
    void Render();

    inline float GetFixedDeltaTime() const { return FixedDeltaTime; }

private:
    const int TargetFPS = 60;
    float FixedDeltaTime = 0.0f;
    BaseScene* currentScene = nullptr; // 현재 활성화된 씬 포인터

    // 싱글톤 핵심: 생성자를 private으로 감춰 외부 선언 및 new 차단
    GameEngine();

    void SetCursorPosition(int X, int Y);
    void HideCursor();
    void SetConsoleSize(int Width, int Height);
};
