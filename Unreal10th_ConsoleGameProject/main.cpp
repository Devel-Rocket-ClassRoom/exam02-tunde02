#include "Common.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "BaseScene.h"
#include "Player.h"
#include "Monster.h"
#include "MenuScene.h"
#include "Stage1Scene.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

//  TODO:
//  [ ] 문서 작성
//      - 클래스 목록, 기능들 설명
//      - Bullet.h 의 inline static 변수 선언 때문에 프로젝트 언어 설정 C++20으로 변경한 것 명시
// 

/* 게임 타이틀 문자열
* https://www.asciiart.eu/text-to-ascii-art

    L"███████╗    ██████╗      █████╗      ██████╗    ███████╗        ██╗  ██╗",
    L"██╔════╝    ██╔══██╗    ██╔══██╗    ██╔════╝    ██╔════╝        ╚██╗██╔╝",
    L"███████╗    ██████╔╝    ███████║    ██║         █████╗           ╚███╔╝ ",
    L"╚════██║    ██╔═══╝     ██╔══██║    ██║         ██╔══╝           ██╔██╗ ",
    L"███████║    ██║         ██║  ██║    ╚██████╗    ███████╗        ██╔╝ ██╗",
    L"╚══════╝    ╚═╝         ╚═╝  ╚═╝     ╚═════╝    ╚══════╝        ╚═╝  ╚═╝"
*/

int main()
{
    // Set random seed
    //unsigned int Seed = time(0);
    unsigned int Seed = 0;
    srand(Seed);

    // Need to use std::wstring
    setlocale(LC_ALL, "");


    // Screen Size : 83, 48
    GameEngine::Instance().AddNewScene(new MenuScene(83, 48));
    GameEngine::Instance().AddNewScene(new Stage1Scene(83, 48));

    GameEngine::Instance().ChangeScene(SceneType::MenuScene);
    GameEngine::Instance().Run();

    return 0;
}
