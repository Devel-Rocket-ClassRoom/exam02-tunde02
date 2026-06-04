#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <chrono>
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
    int Width = 0;
    int Height = 0;

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
    float UpdatePeriod_ = 0.2f;
    float UpdateTimer_ = 0.0f;

public:
    std::vector<std::wstring> RenderString;

    virtual ~GameObject() = default;
    virtual void Update() = 0;
    virtual void Render() {}

    // 지연 삭제 상태 확인 및 설정 함수
    inline bool IsDestroyed() const { return IsDestroyed_; }
    inline void Destroy() { IsDestroyed_ = true; }
    inline Vector2 GetPosition() const { return Transform_.Position; }
    inline int GetWidth() const { return Transform_.Width; }
    inline int GetHeight() const { return Transform_.Height; }
};

// 샘플 오브젝트: 몬스터
class Monster : public GameObject
{
public:
    Monster()
    {
        Transform_.Width = 3;
        Transform_.Height = 3;
        RenderString.reserve(static_cast<size_t>(Transform_.Width) * Transform_.Height);
        for (int i = 0; i < Transform_.Height; i++)
        {
            std::wstring Str{};
            for (int j = 0; j < Transform_.Width; j++)
            {
                Str += L"X";
            }
            RenderString.push_back(Str);
        }
        Transform_.Delta = Vector2{ 1, 0 };
    }

    void Update() override
    {
        UpdateTimer_ += 0.016f;
        if (UpdateTimer_ >= UpdatePeriod_)
        {
            TurnAround();
            UpdateTimer_ -= UpdatePeriod_;
        }
    }

    void TurnAround()
    {
        Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
        if (NextPosition.X >= 99)
        {
            Transform_.Delta.X = -1;
            NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
        }
        else if (NextPosition.X < 0)
        {
            Transform_.Delta.X = 1;
            NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
        }

        Transform_.Position = NextPosition;
    }
};

class Player : public GameObject
{
public:
    Player()
    {
        Transform_.Width = 3;
        Transform_.Height = 3;
        RenderString.reserve(static_cast<size_t>(Transform_.Width) * Transform_.Height);
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

        Vector2 NextPosition{ Transform_.Position + Transform_.Delta };
        if (0 <= NextPosition.X && NextPosition.X < 120
            && 0 <= NextPosition.Y && NextPosition.Y < 29)
        {
            NextPosition = Vector2{ Transform_.Position + Transform_.Delta };
            Transform_.Position = NextPosition;
        }
    }
};


// ==========================================
// 2. 씬 / 스테이지 관리를 위한 부모 추상 클래스
// ==========================================
class BaseScene
{
protected:
    // 각 씬은 자신만의 독립된 오브젝트 리스트를 관리함
    std::vector<GameObject*> SceneObjects;
    int Width_ = 0;
    int Height_ = 0;

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
            }
        }

        // 2. 지연 삭제 (Update 루프가 완전히 끝난 후 플래그가 켜진 오브젝트 일괄 제거)
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
        COORD Coord = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);

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
                    Screen[obj->GetPosition().Y + i][obj->GetPosition().X + j] = obj->RenderString[i][j];
                }
            }
        }

        std::wstring Buffer{};
        Buffer.reserve(static_cast<size_t>(Width_ + 1) * Height_);
        for (int i = 0; i < Height_; i++)
        {
            Buffer += Screen[i];
            Buffer += L"\n";
        }

        wprintf(L"%s", Buffer.c_str());
    }
};


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
        SceneObjects.push_back(new Player());
        SceneObjects.push_back(new Monster());
    }
    void Exit() override
    {
        std::cout << "◀ [Stage1Scene] 탈출: 1스테이지 데이터 메모리 자동 소멸" << std::endl;
    }
};


// ==========================================
// 4. 싱글톤 기반 게임 엔진 클래스
// ==========================================
class GameEngine
{
private:
    BaseScene* currentScene = nullptr; // 현재 활성화된 씬 포인터

    // 싱글톤 핵심: 생성자를 private으로 감춰 외부 선언 및 new 차단
    GameEngine() = default;

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
        int x = 1000;
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
