#include "MenuScene.h"
#include <Windows.h>
#include <iostream>

void MenuScene1::Enter() { std::cout << "▶ [MenuScene] 진입: 타이틀 메뉴 생성" << std::endl; }

void MenuScene1::Exit() { std::cout << "◀ [MenuScene] 탈출: 메뉴 리소스 해제" << std::endl; }

void MenuScene1::Update()
{
    if (GetAsyncKeyState(VK_UP)) // ↑
    {
        CurrentMenuIndex--;
        if (CurrentMenuIndex < 0)
        {
            CurrentMenuIndex = 2;
        }
    }
    else if (GetAsyncKeyState(VK_DOWN)) // ↓
    {
        CurrentMenuIndex++;
        if (CurrentMenuIndex > 2)
        {
            CurrentMenuIndex = 0;
        }
    }
}

void MenuScene1::Render()
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
