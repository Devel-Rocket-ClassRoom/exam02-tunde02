#include "Stage1Scene.h"
#include "GameEngine.h"
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "Item.h"
#include <iostream>

void Stage1Scene::Enter()
{
    Player_ = new Player();
    SceneObjects.push_back(Player_);

    float Timer = 0.25f;

    for (int i = 0; i < 10; i++)
    {
        Instantiate(new Monster(MonsterType::Default), Vector2(5.0f, 2.0f), Vector2(2.5f, 0.2f).Normalized(), Timer);
        Timer += 0.25f;
    }

    Timer += 0.4f;

    Instantiate(new Item(ItemType::BulletUpgrade), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    for (int i = 0; i < 3; i++)
    {
        Instantiate(new Monster(MonsterType::TripleShot), Vector2(2.0f, 2.0f), Vector2(1.0f, 0.2f).Normalized(), Timer);
        Instantiate(new Monster(MonsterType::TripleShot), Vector2(50.0f, 2.0f), Vector2(-1.0f, 0.2f).Normalized(), Timer);
        Timer += 0.4f;
    }

    Timer += 0.75f;

    Instantiate(new Item(ItemType::BulletUpgrade), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    Timer += 0.75f;

    Instantiate(new Monster(MonsterType::Settled), Vector2(2.0f, 2.0f), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Vector2(26.0f, 9.0f), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Vector2(51.0f, 2.0f), { 0.0f, 0.0f }, Timer);
    Timer += 0.4f;
    Instantiate(new Monster(MonsterType::Settled), Vector2(2.0f, 9.0f), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Vector2(26.0f, 2.0f), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Vector2(51.0f, 9.0f), { 0.0f, 0.0f }, Timer);
    Timer += 1.75f;

    Instantiate(new Item(ItemType::Recovery), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    Timer += 1.75f;

    Instantiate(new Monster(MonsterType::TripleShot), Vector2(2.0f, 2.0f), Vector2(1.0f, 1.2f).Normalized(), Timer);
    Instantiate(new Monster(MonsterType::TripleShot), Vector2(50.0f, 2.0f), Vector2(-1.0f, 1.2f).Normalized(), Timer);
    Instantiate(new Monster(MonsterType::TripleShot), Vector2(2.0f, 2.0f), Vector2(1.0f, 0.4f).Normalized(), Timer + 0.2f);
    Instantiate(new Monster(MonsterType::TripleShot), Vector2(50.0f, 2.0f), Vector2(-1.0f, 0.4f).Normalized(), Timer + 0.2f);
    Instantiate(new Monster(MonsterType::Settled), Vector2(13.0f, 9.0f), Vector2(0.0f, 0.0f).Normalized(), Timer);
    Instantiate(new Monster(MonsterType::Settled), Vector2(38.0f, 9.0f), Vector2(0.0f, 0.0f).Normalized(), Timer);

    Instantiate(new Item(ItemType::BulletUpgrade), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    for (int i = 0; i < 20; i++)
    {
        Instantiate(new Monster(MonsterType::Default), Vector2(5.0f, 2.0f), { 1.0f, 0.0f }, Timer);
        Instantiate(new Monster(MonsterType::Default), Vector2(56.0f, 2.0f), { -1.0f, 0.0f }, Timer);
        Timer += 0.07f;
    }

    Timer += 1.5f;

    Instantiate(new Item(ItemType::Recovery), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    Timer += 0.75f;

    Instantiate(new Item(ItemType::BulletUpgrade), Vector2(27.0f, 2.0f), { 0.0f, 0.2f }, Timer);

    Timer += 0.75f;

    CurrentMenuIndex = 0;
    for (auto& Key : Keys)
    {
        Key.bIsKeyPressed = false;
    }
}

void Stage1Scene::Exit()
{
    for (auto& Obj : SceneObjects)
    {
        if (Obj != nullptr)
        {
            delete Obj;
            Obj = nullptr;
        }
    }

    SceneObjects.clear();
}

void Stage1Scene::Update()
{
    // 플레이어가 죽었거나, 보스 몬스터가 죽었다면 스테이지 종료
    bIsStageFinished = !IsPlayerAlive() || (bIsBossSpawned && !IsBossAlive());

    if (!bIsStageFinished)
    {
        StageTimer += GameEngine::Instance().GetFixedDeltaTime();

        // 1. 보스가 이전에 소환된 적이 없다
        // 2. 현재 스테이지가 시작된지 12.0f만큼 흘렀다
        // 3. 현재 스테이지에 존재하는 오브젝트가 1개이며, 동시에 그 오브젝트가 플레이어이다
        // 위 조건을 모두 만족할 때 보스 몬스터를 소환하고 보스 몬스터를 소환했다는 플래그를 true로 변경
        if (!bIsBossSpawned
            && StageTimer > 12.0f
            && SceneObjects.size() == 1
            && SceneObjects[0] == Player_)
        {
            bIsBossSpawned = true;
            Instantiate(new Monster(MonsterType::Boss), Vector2(0.0f, 2.0f), { 1.0f, 0.f }, 0.0f);
        }

        BaseScene::Update();
    }
    else
    {
        // 스테이지가 종료되고 메뉴 창이 팝업되면 플레이어 캐릭터를 조작 불가능한 상태로 변경
        Player_->ChangePlayerState(PlayerState::Uncontrollable);

        for (auto& Key : Keys)
        {
            bool CurrentPressed = (GetAsyncKeyState(Key.KeyCode) & 0x8000) != 0;

            if (CurrentPressed && !Key.bIsKeyPressed)
            {
                Key.bIsKeyPressed = true;
            }
            else if (!CurrentPressed && Key.bIsKeyPressed)
            {
                Key.bIsKeyPressed = false;
                ProcessKeyPress(Key.KeyCode);
            }
        }
    }
}

void Stage1Scene::Render()
{
    BaseScene::InitializeScreen();
    BaseScene::RenderSceneObjects();
    RenderStatus();
    if (bIsStageFinished)
    {
        RenderStageClearMenu();
    }
    BaseScene::PrintScreen();
}

void Stage1Scene::RenderStatus()
{
    const size_t StatusUIStartX = 61;

    // 위, 아래 테두리
    for (size_t i = 0; i < Width_; i++)
    {
        Screen[0][i] = L'█';
        Screen[Height_ - 2][i] = L'█';
    }

    // 왼쪽 테두리
    for (size_t i = 1; i < Height_ - 2; i++)
    {
        Screen[i][0] = L'█';
    }

    // 가운데 테두리
    for (size_t i = 1; i < Height_ - 2; i++)
    {
        Screen[i][GameAreaWidth] = L'█';
    }

    // 오른쪽 테두리
    for (size_t i = 1; i < Height_ - 2; i++)
    {
        Screen[i][Width_ - 1] = L'█';
    }

    // 스테이지
    std::wstring StageText = L"S T A G E";
    Screen[3].replace(GetTextStartX(StatusUIStartX, StageText.length()), StageText.length(), StageText);

    std::wstring StageNumberText = L"1";
    Screen[5].replace(GetTextStartX(StatusUIStartX, StageNumberText.length()), StageNumberText.length(), StageNumberText);

    for (size_t i = StatusUIStartX; i < Width_; i++)
    {
        Screen[8][i] = L'█';
    }

    // 점수
    std::wstring ScoreText = L"S C O R E";
    Screen[11].replace(GetTextStartX(StatusUIStartX, ScoreText.length()), ScoreText.length(), ScoreText);

    std::wstring ScoreNumberText = std::to_wstring(Score);
    Screen[13].replace(GetTextStartX(StatusUIStartX, ScoreNumberText.length()), ScoreNumberText.length(), ScoreNumberText);

    for (size_t i = StatusUIStartX; i < Width_; i++)
    {
        Screen[16][i] = L'█';
    }

    // 플레이어 체력
    std::wstring PlayerText = L"P L A Y E R";
    Screen[19].replace(GetTextStartX(StatusUIStartX, PlayerText.length()), PlayerText.length(), PlayerText);

    std::wstring PlayerHpText = L"";
    for (int i = 0; i < Player_->GetHp() - 1; i++)
    {
        PlayerHpText += L"♡ ";
    }

    if (Player_->GetHp() > 0)
    {
        PlayerHpText += L"♡";
    }

    Screen[21].replace(GetTextStartX(StatusUIStartX, PlayerHpText.length()), PlayerHpText.length(), PlayerHpText);

    for (size_t i = StatusUIStartX; i < Width_; i++)
    {
        Screen[24][i] = L'█';
    }
}

void Stage1Scene::ProcessKeyPress(int InKeyCode)
{
    switch (InKeyCode)
    {
        case VK_UP:
            CurrentMenuIndex--;
            if (CurrentMenuIndex < 0)
            {
                CurrentMenuIndex = MenuCount - 1;
            }
            break;
        case VK_DOWN:
            CurrentMenuIndex++;
            if (CurrentMenuIndex > MenuCount - 1)
            {
                CurrentMenuIndex = 0;
            }
            break;
        case VK_RETURN:
            if (CurrentMenuIndex == 0)
            {
                GameEngine::Instance().ChangeScene(SceneType::MenuScene);
            }
            else if (CurrentMenuIndex == 1)
            {
                exit(0);
            }
            break;
        default:
            break;
    }
}

void Stage1Scene::RenderStageClearMenu()
{
    // Screen에서 팝업 메뉴를 렌더링할 부분만 공백으로 초기화
    for (size_t i = 10; i < Height_ - 16; i++)
    {
        for (size_t j = 5; j < Width_ - 5; j++)
        {
            Screen[i][j] = L' ';
        }
    }

    // 위, 아래 테두리
    for (size_t i = 5; i < Width_ - 5; i++)
    {
        Screen[10][i] = L'█';
        Screen[Height_ - 16][i] = L'█';
    }

    // 왼쪽 테두리
    for (size_t i = 11; i < Height_ - 16; i++)
    {
        Screen[i][5] = L'█';
    }

    // 오른쪽 테두리
    for (size_t i = 11; i < Height_ - 16; i++)
    {
        Screen[i][Width_ - 6] = L'█';
    }

    std::vector<std::wstring> Message;

    if (IsPlayerAlive())
    {
        Message = {
            L" ██████╗    ██╗         ███████╗     █████╗     ██████╗ ",
            L"██╔════╝    ██║         ██╔════╝    ██╔══██╗    ██╔══██╗",
            L"██║         ██║         █████╗      ███████║    ██████╔╝",
            L"██║         ██║         ██╔══╝      ██╔══██║    ██╔══██╗",
            L"╚██████╗    ███████╗    ███████╗    ██║  ██║    ██║  ██║",
            L" ╚═════╝    ╚══════╝    ╚══════╝    ╚═╝  ╚═╝    ╚═╝  ╚═╝"
        };
    }
    else
    {
        Message =  {
            L"███████╗     █████╗     ██╗    ██╗                  ",
            L"██╔════╝    ██╔══██╗    ██║    ██║                  ",
            L"█████╗      ███████║    ██║    ██║                  ",
            L"██╔══╝      ██╔══██║    ██║    ██║                  ",
            L"██║         ██║  ██║    ██║    ███████╗    ██╗██╗██╗",
            L"╚═╝         ╚═╝  ╚═╝    ╚═╝    ╚══════╝    ╚═╝╚═╝╚═╝"
        };
    }

    for (size_t i = 0; i < 6; i++)
    {
        Screen[14 + i].replace(GetTextStartX(0, Message[i].length()), Message[i].length(), Message[i]);
    }

    std::wstring MainSceneMenu = CurrentMenuIndex == 0 ? L"▶  처음으로    " : L"   처음으로    ";
    Screen[25].replace(BaseScene::GetTextStartX(0, MainSceneMenu.length() + 4), MainSceneMenu.length() + 4, MainSceneMenu);

    std::wstring QuitProgramMenu = CurrentMenuIndex == 1 ? L"▶      종료    " : L"       종료    ";
    Screen[27].replace(BaseScene::GetTextStartX(0, QuitProgramMenu.length() + 2), QuitProgramMenu.length() + 2, QuitProgramMenu);
}
