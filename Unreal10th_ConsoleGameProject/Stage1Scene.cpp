#include "Stage1Scene.h"
#include "Player.h"
#include "Monster.h"
#include "Wall.h"
#include "Bullet.h"

#include <iostream>

Stage1Scene::Stage1Scene(int Width, int Height) : BaseScene(Width, Height) {}

void Stage1Scene::Enter()
{
    Player_ = new Player();
    SceneObjects.push_back(Player_);

    float Timer = 0.25f;
    for (int i = 0; i < 10; i++)
    {
        Instantiate(new Monster(MonsterType::Default), Transform(5.0f, 2.0f, 0, 0), { -2.5f, 0.2f }, Timer);
        Timer += 0.25f;
    }

    Timer += 0.4f;

    for (int i = 0; i < 3; i++)
    {
        Instantiate(new Monster(MonsterType::TripleShot), Transform(2.0f, 2.0f, 0, 0), { 1.0f, 0.2f }, Timer);
        Instantiate(new Monster(MonsterType::TripleShot), Transform(50.0f, 2.0f, 0, 0), { -1.0f, 0.2f }, Timer);
        Timer += 0.4f;
    }

    Timer += 0.4f;

    Instantiate(new Monster(MonsterType::Settled), Transform(2.0f, 2.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Transform(26.0f, 9.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Transform(51.0f, 2.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Timer += 0.4f;
    Instantiate(new Monster(MonsterType::Settled), Transform(2.0f, 9.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Transform(26.0f, 2.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Transform(51.0f, 9.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Timer += 2.0f;

    Instantiate(new Monster(MonsterType::TripleShot), Transform(2.0f, 2.0f, 0, 0), { 1.0f, 1.2f }, Timer);
    Instantiate(new Monster(MonsterType::TripleShot), Transform(50.0f, 2.0f, 0, 0), { -1.0f, 0.2f }, Timer);
    Instantiate(new Monster(MonsterType::TripleShot), Transform(2.0f, 2.0f, 0, 0), { 1.0f, 0.2f }, Timer + 0.2f);
    Instantiate(new Monster(MonsterType::TripleShot), Transform(50.0f, 2.0f, 0, 0), { -1.0f, 0.2f }, Timer + 0.2f);
    Instantiate(new Monster(MonsterType::Settled), Transform(13.0f, 9.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    Instantiate(new Monster(MonsterType::Settled), Transform(38.0f, 9.0f, 0, 0), { 0.0f, 0.0f }, Timer);
    for (int i = 0; i < 20; i++)
    {
        Instantiate(new Monster(MonsterType::Default), Transform(5.0f, 2.0f, 0, 0), { 1.0f, 0.0f }, Timer);
        Instantiate(new Monster(MonsterType::Default), Transform(56.0f, 2.0f, 0, 0), { -1.0f, 0.0f }, Timer);
        Timer += 0.07f;
    }

    Timer += 3.0f;

    // BOSS
}

void Stage1Scene::Exit()
{
    std::cout << "◀ [Stage1Scene] 탈출: 1스테이지 데이터 메모리 자동 소멸" << std::endl;
}
