#include "Common.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "BaseScene.h"
#include "Player.h"
#include "Monster.h"
#include "MenuScene.h"
#include "Stage1Scene.h"

#include <iostream>

//  TODO:
//  [x] 클래스 나누기 및 정리, main 브랜치에 커밋
//  [x] 오브젝트들 Update()에서 끝자리 판정하는 걸 어떻게 못바꾸나?
//      - 씬 만들때 넘겨주는 건 충분히 가능
//      - 근데 씬 안의 오브젝트들도 씬의 가로세로를 꼭 알아야 할까?
//  [x] 충돌 감지
//  [ ] 씬이 맵을 불러오는 부분 추가
//      - 파일에 그린 맵에서? 그러면 좌표나 크기가 모호해짐
//      - 플레이어, 몬스터, 벽, 벽돌에 대한 정보(좌표, 크기, 문자열 등)가 들어 있는 파일
//      - 가로로 긴 맵. 몬스터 여러 개
//  [ ] 씬에서 씬으로 전환은 엔진.run()에서 씬의 멤버 속성 하나 최상단에서 확인하다가 바꼈으면 해당 enum 씬으로 전환
//  [x] 중력 적용
//      - 땅 위에 있으면 중력의 영향을 받으면 안된다
//  [ ] 점프 구현
//      - 일정 시간동안 중력의 영향을 받지 않도록?
//  [ ] 카메라
//      - 플레이어를 따라다니는
//  [ ] 몬스터 AI
//      - 낭떠러지에서 떨어지면 파괴된다
//  [ ] 밑에서 충돌하면 아이템을 뱉는 혹은 깨지는 벽돌
//  [ ] 버섯
//      - 이동하며, 플레이어와 충돌하면 플레이어의 체력이 올라감
//  [ ] 깃발
//      - 가만히 있고 중력의 영향을 받지 않음
//      - 플레이어와 충돌하면 스테이지 클리어 후 메뉴로
//  [ ] 마리오 첫 번째 스테이지 구현
// 

int main()
{
    // Need to use std::wstring
    setlocale(LC_ALL, "");

    // Camera Size : 170x37
    GameEngine::Instance().ChangeScene(new Stage1Scene(170, 37));
    GameEngine::Instance().Run();

    return 0;
}
