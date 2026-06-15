#include "GameEngine.h"
#include "MenuScene.h"
#include "Stage1Scene.h"

#include <iostream>
#include <stdlib.h>

int main()
{
    // Set Seed
    unsigned int Seed = static_cast<unsigned int>(time(0));
    srand(Seed);

    // Need to use std::wstring
    setlocale(LC_ALL, "");

    // 엔진 생성 및 MenuScene, Stage1Scene 추가
    int ScreenWidth = 83;
    int ScreenHeight = 48;
    GameEngine::Instance().AddNewScene(new MenuScene(ScreenWidth, ScreenHeight));
    GameEngine::Instance().AddNewScene(new Stage1Scene(ScreenWidth, ScreenHeight));

    // 엔진의 CurrentScene을 MenuScene로 바꾼 뒤 Run
    GameEngine::Instance().ChangeScene(SceneType::MenuScene);
    GameEngine::Instance().Run();

    return 0;
}
