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
//  [ ] 플레이어가 총알을 발사하면 총구에서 생성되도록 위치 조정
//  [ ] 총알은 enum BulletSize에 따라 프리셋이 있어야 할 듯
//  [ ] 오브젝트들 생성자 정리
//  [ ] 플레이어에 발사 딜레이 추가
//  [ ] BaseScene.RenderSceneObjects()에서 플레이어를 제일 나중에 그리도록 변경
//  [ ] 플레이어가 탄막에 맞으면 일정시간 깜빡이면서 무적시간이 있어야됨
//  [ ] 플레이어의 실제 피격 범위 조절
// 
//  [x] 스페이스 누르면 플레이어로부터 총알 발사
//  [x] Wall을 그냥 두지 말고, 테두리 렌더링만 한 다음 범위 체크만 해도 되지 않을까?
//      - 총알은 테두리 벗어나면 없어짐
//      - 몬스터는 가장 아래가 아니면 테두리 맞고 X축 운동 방향 반대로
//  [ ] 총알은 enum BulletSize에 따라 프리셋이 있어야 할 듯
//  [ ] 총알에도 체력을 달거임?
//      - 진영이 다른 오브젝트에 맞으면 총알과 오브젝트의 체력이 깎이는 방식
//      - 그러면 일정 개수의 오브젝트를 관통하는 총알을 구현할 수 있다
//      - 기본 체력 1로 구현해보자
//  [x] 충돌은 간단하게 Enter만 있어도 되지 않을까?
//  [ ] 현재 BaseScene에 Player_ 포인터가 있는데, 메뉴같은데에선 없...으면 nullptr니까 딱히 상관 없나?
//      - BaseScene -> MenuScene / BaseScene -> StageScene -> Stage1Scene 이런 식의 상속으로 바꿀까?
//  [ ] ESC 누르면 게임 일시 정지 기능 및 메뉴
//  [x] 플레이어 비행기 모양 바꾸기
//  [ ] BaseScene.RenderSceneObjects()에서 플레이어를 제일 나중에 그리도록 변경
//  [ ] 플레이어의 실제 피격 범위 조절
//      - 1x1? 2x2?
//  [ ] 좌우 방향키 입력에 따라 다른 모양으로 렌더링되는 기능 추가
//  [ ] 플레이어가 탄막에 맞으면 일정시간 깜빡이면서 무적시간이 있어야됨
//  [ ] 탄막 오브젝트 풀링
//  [ ] 몬스터
//      - 그냥 튀어나와서 일정 방향으로 나아가는 몬스터. 테두리를 벗어나면 없어짐
//      - 튀어나와서 일정 방향으로 이동하며 탄막도 무작위로 흩뿌리는 몬스터
//      - 일정 위치를 고수하며 일정 방향으로 탄막을 쏘는 몬스터
//  [ ] 아이템
//      - 무기 강화
//      - 체력
//      - (폭탄)
//  [ ] 시작 메뉴
//  [ ] 보스 스테이지
//  [ ] 플레이어 죽는 연출, 스테이지 클리어 연출
//      - 잔해가 아래로 떨어지는 연출
//      - 플레이어가 위로 쭉 올라가는 연출
//  [ ] 글자 색 변경 추가
//      - Unreal10th_ConsoleGameProject.cpp 내용 참고
// 
//  [ ] 문서 작성
//      - 클래스 목록, 기능들 설명
// 

/* 게임 타이틀
    "   _____ ____  ___   ______ ______  _  __"
    "  / ___// __ \/   | / ____// ____/ | |/ /"
    "  \__ \/ /_/ / /| |/ /    / __/    |   / "
    " ___/ / ____/ ___ / /___ / /___   /   |  "
    "/____/_/   /_/  |_\____//_____/  /_/|_|  "
*/

/* 탄막 모양
    "░▒▓█"
    ▒▒
    ▒█
*/

/* 플레이어 모양

const std::vector<std::vector<std::wstring>> safeExplosion = {
    // Frame 0
    { L"   ▲   ",
      L"  /░\\  ",
      L" /░█░\\ ",
      L" ░▀░▀░ ",
      L"/ / \\ \\",
      L"▀     ▀" },

    // Frame 1
    { L"  ▄█▄  ",
      L" ◢▓▓▓◣ ",
      L"██▓█▓██",
      L" ◥▓▓▓◤ ",
      L"  ▒ ▒  " },

    // Frame 2
    { L" * ░ * ",
      L"▒ ▓▓▓ ▒",
      L" ░▓█▓░ ",
      L"▒ ▓▓▓ ▒",
      L" * ░ * " },

    // Frame 3
    { L" .   . ",
      L"   ░   ",
      L" . ▒ . ",
      L"   ░   ",
      L" .   . " }
};

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
    GameEngine::Instance().ChangeScene(new Stage1Scene(83, 48));
    GameEngine::Instance().Run();

    return 0;
}
