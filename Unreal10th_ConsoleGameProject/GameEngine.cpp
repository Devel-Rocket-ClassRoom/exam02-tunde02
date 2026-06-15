#include "GameEngine.h"
#include <Windows.h>
#include <chrono>
#include <thread>

GameEngine::GameEngine()
{
    FixedDeltaTime = 1.0f / TargetFPS;
    FrameMilliseconds = static_cast<int>(FixedDeltaTime * 1000);
}

GameEngine::~GameEngine()
{
    for (BaseScene* Scene : Scenes)
    {
        delete Scene;
        Scene = nullptr;
    }
}

GameEngine& GameEngine::Instance()
{
    static GameEngine instance;
    return instance;
}

void GameEngine::AddNewScene(BaseScene* Scene)
{
    Scenes.push_back(Scene);
}

void GameEngine::Run()
{
    SetConsoleSize(800, 1000);
    HideCursor();
    system("cls");

    while (true)
    {
        Update();
        Render();

        std::this_thread::sleep_for(std::chrono::milliseconds(FrameMilliseconds));
    }
}

void GameEngine::ChangeScene(BaseScene* Scene)
{
    if (CurrentScene)
    {
        CurrentScene->Exit();
    }

    CurrentScene = Scene;

    if (CurrentScene)
    {
        CurrentScene->Enter();
    }
}

void GameEngine::ChangeScene(SceneType InSceneType)
{
    ChangeScene(Scenes[static_cast<int>(InSceneType)]);
}

void GameEngine::Update()
{
    if (CurrentScene)
    {
        CurrentScene->Update();
    }
}

void GameEngine::Render()
{
    if (CurrentScene)
    {
        SetCursorPosition(0, 0);
        CurrentScene->Render();
    }
}

void GameEngine::SetCursorPosition(int X, int Y)
{
    COORD Coord = { (SHORT)X, (SHORT)Y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);
}

GameObject* GameEngine::Instantiate(GameObject* InGameObject, const Vector2& InPosition, const Vector2& InDelta, float InTimer)
{
    return CurrentScene->Instantiate(InGameObject, InPosition, InDelta, InTimer);
}

void GameEngine::HideCursor()
{
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO Info{};
    Info.dwSize = 100;
    Info.bVisible = FALSE;
    SetConsoleCursorInfo(ConsoleHandle, &Info);
}

void GameEngine::SetConsoleSize(int Width, int Height)
{
    // 터미널 창이 완전히 초기화될 때까지 50ms 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // 현재 콘솔의 핸들을 가져옴
    HWND hwnd = GetConsoleWindow();

    // 최상위 부모(Owner) 창의 핸들을 추적
    HWND topWindow = GetWindow(hwnd, GW_OWNER);

    // 만약 부모 창이 없다면 이전 세대 Windows 콘솔이므로 자기 자신을 타겟으로 설정
    if (topWindow == NULL)
    {
        topWindow = hwnd;
    }

    // 설정할 좌표
    int x = 200;
    int y = 10;

    // 4. 최상위 창의 위치와 크기 변경
    MoveWindow(topWindow, x, y, Width, Height, TRUE);
}
