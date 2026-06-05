#include "Stage1Scene.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>

Stage1Scene::Stage1Scene(int Width, int Height) : BaseScene(Width, Height) {}

void Stage1Scene::Enter()
{
    std::cout << "▶ [Stage1Scene] 진입: 1스테이지 맵 및 몬스터 생성" << std::endl;

    // Stage 1 전용 오브젝트 배치
    SceneObjects.push_back(new Monster());
    SceneObjects.push_back(new Player());
}

void Stage1Scene::Exit()
{
    std::cout << "◀ [Stage1Scene] 탈출: 1스테이지 데이터 메모리 자동 소멸" << std::endl;
}
