#include "GameEngine.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstdlib>

//  TODO:
//  [ ] 클래스 나누기 및 정리, main 브랜치에 커밋
//  [ ] 오브젝트들 Update()에서 끝자리 판정하는 걸 어떻게 못바꾸나?
//      - 씬 만들때 넘겨주는 건 충분히 가능
//      - 근데 씬 안의 오브젝트들도 씬의 가로세로를 꼭 알아야 할까?
//  [ ] 씬의 가로 세로를 맵으로 구현 -> 카메라가 플레이어를 따라가도록
//  [ ] 충돌 감지
//  [ ] 중력 적용
//  [ ] 점프 구현
//  [ ] 몬스터 AI
//      - 낭떠러지에서 떨어지면 파괴된다
//  [ ] 버섯
// 

/*
transform.width, height size_t로
update(frame)로 변경
GetRenderingVector()로 변경
씬에서 오브젝트 삭제할 때 메모리 해제 처리 추가
씬에서 씬으로 전환은 엔진.run()에서 씬의 멤버 속성 하나 최상단에서 확인하다가 바꼈으면 해당 enum 씬으로 전환
game engine to staic? 이미 staic임..
엔진은 카메라 가로세로 / 씬은 자기 가로세로
engine.update(frame) -> scene.update(frame, scenewidth, sceneheight)
위치 변경을 오브젝트가 하지 말고 자기 update에서 델타값만 변경 - 씬에서 델타값이 0이 아닌 애들만 뱉음 - 엔진(또는 씬)이 충돌감지한 다음 실제로 위치 변경
그럼 update도 파라미터 받지 말고 엔진에서 직접 가져다 쓰도록 하는 게
*/

class GameEngine;

struct Vector2
{
    int X = 0;
    int Y = 0;

    Vector2() = default;
    Vector2(int InX, int InY) : X(InX), Y(InY) {}
    Vector2 operator+(const Vector2& other) const
    {
        Vector2 Result{};

        Result.X = this->X + other.X;
        Result.Y = this->Y + other.Y;

        return Result;
    }
    Vector2 operator-(const Vector2& other) const
    {
        Vector2 Result{};

        Result.X = this->X - other.X;
        Result.Y = this->Y - other.Y;

        return Result;
    }
    Vector2& operator=(const Vector2& other)
    {
        if (this == &other)
        {
            return *this;
        }

        this->X = other.X;
        this->Y = other.Y;

        return *this;
    }
};

struct Transform
{
    Vector2 Position{};
    Vector2 Delta{};
    size_t Width = 0;
    size_t Height = 0;

    Transform() = default;
};

// ==========================================
// 1. 최상위 게임 오브젝트 추상 클래스
// ==========================================
class GameObject
{
protected:
    Transform Transform_{};
    bool IsDestroyed_ = false; // 지연 삭제용 플래그
    bool IsPlayer_ = false;
    bool ShouldUpdated_ = false;
    float UpdatePeriod_ = 0.0f;
    float UpdateTimer_ = 0.0f;
    std::vector<std::wstring> RenderString;

public:
    virtual ~GameObject() = default;
    virtual void Update() = 0;
    virtual void Render() {}
    virtual void OnCollisionEnter(GameObject* Other) = 0;
    virtual void OnCollisionExit(GameObject* Other) = 0;

    inline bool IsPlayer() const { return IsPlayer_; }
    inline bool IsDestroyed() const { return IsDestroyed_; }
    inline void Destroy() { IsDestroyed_ = true; }
    inline bool ShouldUpdated() const { return ShouldUpdated_; }
    inline void SetShouldUpdated(bool InShouldUpdated) { ShouldUpdated_ = InShouldUpdated; }
    inline Transform GetTransform() const { return Transform_; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline void SetPosition(Vector2 InPosition) { Transform_.Position = InPosition; }
    inline size_t GetWidth() const { return Transform_.Width; }
    inline size_t GetHeight() const { return Transform_.Height; }
    inline std::vector<std::wstring> GetRenderingVector() const { return RenderString; }
};


// 샘플 오브젝트: 몬스터
class Monster : public GameObject
{
private:
    int CollisionCount = 0;
public:
    Monster()
    {
        Transform_.Width = 3;
        Transform_.Height = 3;
        Transform_.Position = Vector2{ 7, 0 };
        Transform_.Delta = Vector2{ 1, 0 };

        UpdatePeriod_ = 0.016f;

        RenderString.reserve(Transform_.Width * Transform_.Height);
        for (int i = 0; i < Transform_.Height; i++)
        {
            std::wstring Str{};
            for (int j = 0; j < Transform_.Width; j++)
            {
                Str += L"X";
            }
            RenderString.push_back(Str);
        }
    }
    void Update() override;

    //void Update() override
    //{
    //    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    //    if (UpdateTimer_ >= UpdatePeriod_)
    //    {
    //        //TurnAround();
    //        ShouldUpdated_ = true;
    //        UpdateTimer_ -= UpdatePeriod_;
    //    }
    //}

    virtual void OnCollisionEnter(GameObject* Other) override
    {
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%d", CollisionCount);
        if (Other != nullptr && Other->IsPlayer())
        {
            CollisionCount++;
            for (int i = 0; i < Transform_.Height; i++)
            {
                std::wstring Str{};
                for (int j = 0; j < Transform_.Width; j++)
                {
                    Str += std::to_wstring(CollisionCount);
                }
                RenderString.push_back(Str);
            }
        }
        else
        {
            TurnAround();
        }
    }

    virtual void OnCollisionExit(GameObject* Other) {}

    void TurnAround()
    {
        Transform_.Delta.X *= -1;
    }

    //void TurnAround()
    //{
    //    Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
    //    if (NextPosition.X >= 99)
    //    {
    //        Transform_.Delta.X = -1;
    //        NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
    //    }
    //    else if (NextPosition.X < 0)
    //    {
    //        Transform_.Delta.X = 1;
    //        NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
    //    }

    //    Transform_.Position = NextPosition;
    //}
};

class Player : public GameObject
{
public:
    Player()
    {
        IsPlayer_ = true;

        Transform_.Width = 3;
        Transform_.Height = 3;

        RenderString.reserve(Transform_.Width * Transform_.Height);
        for (int i = 0; i < Transform_.Height; i++)
        {
            std::wstring Str{};
            for (int j = 0; j < Transform_.Width; j++)
            {
                Str += L"O";
            }
            RenderString.push_back(Str);
        }
        Transform_.Position = Vector2{ 5, 5 };
    }

    void Update() override
    {
        ShouldUpdated_ = true;
        Transform_.Delta.X = 0;
        Transform_.Delta.Y = 0;

        if (GetAsyncKeyState(VK_UP)) // ↑
        {
            Transform_.Delta.Y = -1;
        }
        else if (GetAsyncKeyState(VK_DOWN)) // ↓
        {
            Transform_.Delta.Y = 1;
        }

        if (GetAsyncKeyState(VK_LEFT)) // ←
        {
            Transform_.Delta.X = -1;
        }
        else if (GetAsyncKeyState(VK_RIGHT)) // →
        {
            Transform_.Delta.X = 1;
        }

        //Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
        //if (0 <= NextPosition.X && NextPosition.X < 120
        //    && 0 <= NextPosition.Y && NextPosition.Y < 29)
        //{
        //    NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
        //    Transform_.Position = NextPosition;
        //}
    }

    virtual void OnCollisionEnter(GameObject* Other) {}
    virtual void OnCollisionExit(GameObject* Other) {}
};


// ==========================================
// 2. 씬 / 스테이지 관리를 위한 부모 추상 클래스
// ==========================================
class BaseScene
{
protected:
    // 각 씬은 자신만의 독립된 오브젝트 리스트를 관리함
    std::vector<GameObject*> SceneObjects;
    size_t Width_ = 0;
    size_t Height_ = 0;

public:
    BaseScene(int Width, int Height) : Width_(Width), Height_(Height) {}
    virtual ~BaseScene()
    {
        printf("신 오브젝트 메모리 해제\n");
        for (GameObject* SceneObject : SceneObjects)
        {
            delete SceneObject;
            SceneObject = nullptr;
        }
        SceneObjects.clear();
    }

    virtual void Enter() = 0;  // 씬 시작 시 (데이터 로드, 배치)
    virtual void Exit() = 0;   // 씬 종료 시 (자원 해제)

    // 현재 씬에 속한 오브젝트들 업데이트 및 지연 삭제 처리
    virtual void Update()
    {
        // 1. 모든 오브젝트 로직 업데이트
        for (auto& obj : SceneObjects)
        {
            if (!obj->IsDestroyed())
            {
                obj->Update();

                if (!obj->ShouldUpdated())
                {
                    continue;
                }

                Vector2 PrevPosition = obj->GetPosition();
                Vector2 NextPosition = obj->GetTransform().Position + obj->GetTransform().Delta;

                // Collide with Player?
                bool IsCollidedWithPlayer = false;
                GameObject* player = SceneObjects[1];
                if (!obj->IsPlayer())
                {
                    if (obj->GetPosition().X <= player->GetPosition().X)
                    {
                        IsCollidedWithPlayer = obj->GetPosition().X + obj->GetWidth() > player->GetPosition().X
                            && obj->GetPosition().Y + obj->GetHeight() > player->GetPosition().Y;
                    }
                    else
                    {
                        IsCollidedWithPlayer = player->GetPosition().X + player->GetWidth() > obj->GetPosition().X
                            && player->GetPosition().Y + player->GetHeight() < obj->GetPosition().Y;
                    }
                }

                if (IsCollidedWithPlayer)
                {
                    obj->OnCollisionEnter(SceneObjects[1]);
                }
                else if (NextPosition.X == 30 || NextPosition.X == 5)
                {
                    obj->OnCollisionEnter(nullptr);
                }
                else if (0 <= NextPosition.X && NextPosition.X < Width_
                         && 0 <= NextPosition.Y && NextPosition.Y < Height_)
                {
                    obj->SetPosition(NextPosition);
                    obj->SetShouldUpdated(false);
                }
            }
        }

        // 2. 지연 삭제 (Update 루프가 완전히 끝난 후 플래그가 켜진 오브젝트 일괄 제거)
        // TODO: 별도 배열에 파괴된 오브젝트들 추가하고, for문 밖에서 이것들 delete
        SceneObjects.erase(
            std::remove_if(SceneObjects.begin(), SceneObjects.end(),
                           [](const GameObject* obj) {
                               return obj->IsDestroyed();
                           }),
            SceneObjects.end()
        );
    }

    // 현재 씬의 오브젝트들 렌더링
    virtual void Render()
    {
        std::vector<std::wstring> Screen(Height_, L"");
        for (size_t i = 0; i < Height_; i++)
        {
            for (size_t j = 0; j < Width_; j++)
            {
                Screen[i] += L"█";
            }
        }

        for (auto& obj : SceneObjects)
        {
            for (size_t i = 0; i < obj->GetHeight(); i++)
            {
                for (size_t j = 0; j < obj->GetWidth(); j++)
                {
                    Screen[obj->GetPosition().Y + i][obj->GetPosition().X + j] = obj->GetRenderingVector()[i][j];
                }
            }
        }

        std::wstring Buffer{};
        Buffer.reserve(Width_ + 1 * Height_);
        for (int i = 0; i < Height_; i++)
        {
            Buffer += Screen[i];
            Buffer += L"\n";
        }

        wprintf(L"%s", Buffer.c_str());
    }
};


// ==========================================
// 4. 싱글톤 기반 게임 엔진 클래스
// ==========================================
class GameEngine
{
private:
    const int TargetFPS = 60;
    float FixedDeltaTime = 0.0f;
    BaseScene* currentScene = nullptr; // 현재 활성화된 씬 포인터

    // 싱글톤 핵심: 생성자를 private으로 감춰 외부 선언 및 new 차단
    GameEngine() { FixedDeltaTime = 1.0f / TargetFPS; }

    void SetCursorPosition(int X, int Y)
    {
        COORD Coord = { (SHORT)X, (SHORT)Y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);
    }

    void HideCursor()
    {
        HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO Info{};
        Info.dwSize = 100;
        Info.bVisible = FALSE;
        SetConsoleCursorInfo(ConsoleHandle, &Info);
    }

    void SetConsoleSize(int Width, int Height)
    {
        // 1. 터미널 창이 완전히 초기화될 때까지 50ms 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // 2. 현재 콘솔의 핸들을 가져옴
        HWND hwnd = GetConsoleWindow();

        // 3. 최상위 부모(Owner) 창의 핸들을 추적
        HWND topWindow = GetWindow(hwnd, GW_OWNER);

        // 만약 부모 창이 없다면 이전 세대 Windows 콘솔이므로 자기 자신을 타겟으로 설정
        if (topWindow == NULL)
        {
            topWindow = hwnd;
        }

        // 설정할 좌표와 크기 (픽셀 단위)
        int x = 200;
        int y = 100;

        // 4. 최상위 창의 위치와 크기 변경
        MoveWindow(topWindow, x, y, Width, Height, TRUE);
    }

public:
    ~GameEngine()
    {
        delete currentScene;
        currentScene = nullptr;
    }

    // 외부 복사 및 대입 원천 차단
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // 전역 어디서나 단 하나의 원본 객체에 접근할 수 있는 통로 (Meyers' Singleton)
    static GameEngine& Instance()
    {
        static GameEngine instance; // 함수 내부 정적 변수: 프로그램 라이프사이클 동안 딱 '한 번'만 생성
        return instance;
    }

    void Run()
    {
        SetConsoleSize(1600, 800);

        HideCursor();
        system("cls");

        while (true)
        {
            Update();
            Render();

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    // 씬 전환 핵심 함수 (이전 씬 안전하게 해제 후 새 씬 교체)
    void ChangeScene(BaseScene* newScene)
    {
        if (currentScene)
        {
            currentScene->Exit();
        }
        currentScene = newScene;
        if (currentScene)
        {
            currentScene->Enter();
        }
    }

    // 메인 게임 루프에서 호출되는 Update/Render 대행
    void Update()
    {
        if (currentScene) currentScene->Update();
    }

    void Render()
    {
        if (currentScene)
        {
            SetCursorPosition(0, 0);
            currentScene->Render();
        }
    }

    inline float GetFixedDeltaTime() const { return FixedDeltaTime; }
};


void Monster::Update()
{
    UpdateTimer_ += GameEngine::Instance().GetFixedDeltaTime();
    if (UpdateTimer_ >= UpdatePeriod_)
    {
        //TurnAround();
        ShouldUpdated_ = true;
        UpdateTimer_ -= UpdatePeriod_;
    }
}


// ==========================================
// 3. 구체적인 씬 구현 (메뉴 씬 & Stage 1 씬)
// ==========================================
class MenuScene : public BaseScene
{
private:
    int CurrentMenuIndex = 0;

public:
    void Enter() override { std::cout << "▶ [MenuScene] 진입: 타이틀 메뉴 생성" << std::endl; }
    void Exit() override { std::cout << "◀ [MenuScene] 탈출: 메뉴 리소스 해제" << std::endl; }

    void Update() override
    {
        if (_kbhit())
        {
            int Key = _getch();
            if (Key == 72) // ↑
            {
                CurrentMenuIndex--;
                if (CurrentMenuIndex < 0)
                {
                    CurrentMenuIndex = 2;
                }
            }
            else if (Key == 80) // ↓
            {
                CurrentMenuIndex++;
                if (CurrentMenuIndex > 2)
                {
                    CurrentMenuIndex = 0;
                }
            }
        }
    }

    void Render() override
    {
        std::cout << "==[MenuScene]==" << std::endl;
        for (int i = 0; i < 3; i++)
        {
            if (CurrentMenuIndex == i)
            {
                printf("[MenuScene] 메뉴 %d  ◀\n", i + 1);
            }
            else
            {
                printf("[MenuScene] 메뉴 %d----\n", i + 1);
            }
        }
    }
};

class Stage1Scene : public BaseScene
{
public:
    Stage1Scene(int Width, int Height) : BaseScene(Width, Height) {}
    void Enter() override
    {
        std::cout << "▶ [Stage1Scene] 진입: 1스테이지 맵 및 몬스터 생성" << std::endl;

        // Stage 1 전용 오브젝트 배치
        SceneObjects.push_back(new Monster());
        SceneObjects.push_back(new Player());
    }
    void Exit() override
    {
        std::cout << "◀ [Stage1Scene] 탈출: 1스테이지 데이터 메모리 자동 소멸" << std::endl;
    }
};





// ==========================================
// 5. 엔트리 포인트 (실행 흐름)
// ==========================================
int main()
{
    // Need to use std::wstring
    setlocale(LC_ALL, "");

    GameEngine::Instance().ChangeScene(new Stage1Scene(120, 25));
    GameEngine::Instance().Run();

    return 0;
}
