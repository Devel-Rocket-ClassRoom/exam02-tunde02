#include "Stage1Scene.h"
#include "Player.h"
#include "Monster.h"
#include "Wall.h"
#include "Bullet.h"

#include <iostream>

Stage1Scene::Stage1Scene(int Width, int Height) : BaseScene(Width, Height) {}

void Stage1Scene::Enter()
{
    std::cout << "▶ [Stage1Scene] 진입: 1스테이지 맵 및 몬스터 생성" << std::endl;

    // Stage 1 전용 오브젝트 배치

    SceneObjects.push_back(new Wall(0.0f, 0.0f, 1, 47)); // Left
    SceneObjects.push_back(new Wall(60.0f, 0.0f, 1, 47)); // Right
    SceneObjects.push_back(new Wall(1.0f, 0.0f, 59, 1)); // Top
    SceneObjects.push_back(new Wall(1.0f, 46.0f, 59, 1)); // Bottom
    //SceneObjects.push_back(new Bullet(Transform{}, Vector2{}, Faction::Monster, 2));
    //SceneObjects.push_back(new Monster(2, 2));
    //SceneObjects.push_back(new Monster(4, 2));
    // 
    //SceneObjects.push_back(new Monster(10, 2));

    Transform t{ 5.0f, 2.0f, 0, 0 };
    for (int i = 0; i < 10; i++)
    {
        //Instantiate(new Monster(10, 2), t, {}, static_cast<float>(i) / 30);
        Instantiate(new Monster(MonsterType::Default), t, { -2.5f, 0.2f }, static_cast<float>(i) / 20);
    }
    Player_ = new Player();
    SceneObjects.push_back(Player_);
}

void Stage1Scene::Exit()
{
    std::cout << "◀ [Stage1Scene] 탈출: 1스테이지 데이터 메모리 자동 소멸" << std::endl;
}
