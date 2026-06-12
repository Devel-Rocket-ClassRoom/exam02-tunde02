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
//  [x] 플레이어가 총알을 발사하면 총구에서 생성되도록 위치 조정
//      - 기본적으로 오브젝트의 기준 좌표는 좌측 상단, 가로와 세로는 우하단 방향으로 나아간다
//      - 플레이어의 총구 위치는 Player가 가지고 있고, 총알의 중앙 하단 오프셋은 Bullet이 가지고 있을 것
//      - 플레이어가 생성자 호출하여 Bullet 생성 - Instantiate() Initialize(Transform, Delta) 호출 순으로 오브젝트를 스폰한다
//      - OffsetX : Player.Width/2 - Bullet.Width/2
//      - OffsetY : -Bullet.Height
//  [~] 몬스터
//      - 그냥 튀어나와서 일정 방향으로 나아가는 몬스터. 밑 테두리를 벗어나면 없어짐
//      - 튀어나와서 일정 방향으로 이동하며 탄막도 무작위로 흩뿌리는 몬스터
//      - 일정 위치를 고수하며 일정 방향으로 탄막을 쏘는 몬스터
//  [x] 총알이 플레이어보다 느려서 발사하고 위로 올라가면 총알이랑 플레이어랑 겹치고 있음
//      - 총알 속도를 좀 빠르게 해야할 듯
//  [x] 총알은 enum BulletSize에 따라 프리셋이 있어야 할 듯
//  [x] 오브젝트들 생성자 정리
//      - Instantiate() 에서 GameObject->Initialize()해서 생성자에서 하는 일을 굳이 또 하는 느낌. 생성자 정리할 때 확실히 하자
//  [x] 플레이어에 발사 딜레이 추가
//  [ ] BaseScene.RenderSceneObjects()에서 플레이어를 제일 나중에 그리도록 변경
//  [ ] 플레이어가 탄막에 맞으면 일정시간 깜빡이면서 무적시간이 있어야됨
//  [ ] 플레이어의 실제 피격 범위 조절
//  [ ] Delta를 Vector2말고 Direciton 열거체의 조합으로 나타내고, 속도는 UpgradePeriod/Speed 로 표현?
//      - 근데 이러면 이동 방향이 8가지로 제한됨
//      - 그렇다고 Delta와 Position을 float로 바꾸면 속도가 너무 빠를 경우 벽을 뚫는 버그 발생
//  [x] 중력, ground 삭제
//  [ ] 플레이어 이동 벽에 막히도록 수정
//      - 위쪽 테두리보다 살짝 덜 갈 수 있어야 한다
//  [x] 큰 몬스터끼리 겹치면 출력이 이상하게 됨
//      - BaseScene.Render() 에서 이전 스크린 픽셀이 공백이었고, 이번에 그릴 오브젝트의 픽셀이 공백이 아닐때만 그리도록 해서 해결
//  [ ] 플레이어 체력 렌더링 수정
//      - 0개도 1개로 출력 중. 그냥 다 그리고 맨끝 공백 지우는 식으로 수정하자
//  [ ] 총알 이동 방식 Direction 으로 변경
// 
// 
//  [x] 스페이스 누르면 플레이어로부터 총알 발사
//  [x] Wall을 그냥 두지 말고, 테두리 렌더링만 한 다음 범위 체크만 해도 되지 않을까?
//      - 총알은 테두리 벗어나면 없어짐
//      - 몬스터는 가장 아래가 아니면 테두리 맞고 X축 운동 방향 반대로
//  [x] 총알에도 체력을 달거임?
//      - 진영이 다른 오브젝트에 맞으면 총알과 오브젝트의 체력이 깎이는 방식
//      - 그러면 일정 개수의 오브젝트를 관통하는 총알을 구현할 수 있다
//      - 기본 체력 1로 구현해보자
//  [x] 충돌은 간단하게 Enter만 있어도 되지 않을까?
//  [ ] 현재 BaseScene에 Player_ 포인터가 있는데, 메뉴같은데에선 없...으면 nullptr니까 딱히 상관 없나?
//      - BaseScene -> MenuScene / BaseScene -> StageScene -> Stage1Scene 이런 식의 상속으로 바꿀까?
//  [ ] ESC 누르면 게임 일시 정지 기능 및 메뉴
//  [ ] 좌우 방향키 입력에 따라 다른 모양으로 렌더링되는 기능 추가
//  [ ] 탄막 오브젝트 풀링
//  [ ] 아이템
//      - 무기 강화
//      - 체력
//      - 슈퍼마리오 별처럼 일정시간 폭격기 총알을 발사하는 모드가 되는 아이템
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
//      - Bullet.h 의 inline static 변수 선언 때문에 프로젝트 언어 설정 C++20으로 변경한 것 명시
// 

/* 게임 타이틀
* https://www.asciiart.eu/text-to-ascii-art
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


/*

   ,
__/ \__
\     /
/_   _\
  \ /
   '


*/

/* 플레이어 모양

const std::vector<std::vector<std::wstring>> safeExplosion = {
    // Frame 0
    { L"   ▲   ",
      L"  /░\  ",
      L" /░█░\ ",
      L" ░▀░▀░ ",
      L"/ / \ \",
      L"▀▀   ▀▀" },

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
