#include "MenuScene.h"
#include "GameEngine.h"
#include <Windows.h>
#include <iostream>
#include <string>

void MenuScene::Enter()
{
    CurrentMenuIndex = 0;
    for (auto& Key : Keys)
    {
        Key.bIsKeyPressed = false;
    }
}

void MenuScene::Exit() {}

void MenuScene::Update()
{
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

void MenuScene::Render()
{
    BaseScene::InitializeScreen();

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

    // 오른쪽 테두리
    for (size_t i = 1; i < Height_ - 2; i++)
    {
        Screen[i][Width_ - 1] = L'█';
    }

    std::vector<std::wstring> TitleText = {
        L"███████╗    ██████╗      █████╗      ██████╗    ███████╗        ██╗  ██╗",
        L"██╔════╝    ██╔══██╗    ██╔══██╗    ██╔════╝    ██╔════╝        ╚██╗██╔╝",
        L"███████╗    ██████╔╝    ███████║    ██║         █████╗           ╚███╔╝ ",
        L"╚════██║    ██╔═══╝     ██╔══██║    ██║         ██╔══╝           ██╔██╗ ",
        L"███████║    ██║         ██║  ██║    ╚██████╗    ███████╗        ██╔╝ ██╗",
        L"╚══════╝    ╚═╝         ╚═╝  ╚═╝     ╚═════╝    ╚══════╝        ╚═╝  ╚═╝"
    };

    for (size_t i = 0; i < 6; i++)
    {
        Screen[10 + i].replace(BaseScene::GetTextStartX(0, TitleText[i].length()), TitleText[i].length(), TitleText[i]);
    }

    std::wstring Stage1SceneMenu = CurrentMenuIndex == 0 ? L"▶  상장    " : L"   상장    ";
    Screen[22].replace(BaseScene::GetTextStartX(0, Stage1SceneMenu.length() + 2), Stage1SceneMenu.length() + 2, Stage1SceneMenu);

    std::wstring QuitProgramMenu = CurrentMenuIndex == 1 ? L"▶  종료    " : L"   종료    ";
    Screen[24].replace(BaseScene::GetTextStartX(0, QuitProgramMenu.length() + 2), QuitProgramMenu.length() + 2, QuitProgramMenu);

    BaseScene::PrintScreen();
}

void MenuScene::ProcessKeyPress(int InKeyCode)
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
                GameEngine::Instance().ChangeScene(SceneType::Stage1Scene);
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
