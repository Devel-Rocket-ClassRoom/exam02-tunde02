#include "GameEngine.h"
#include "GameObject.h"
#include "BaseScene.h"
#include "Player.h"
#include "Monster.h"
#include "MenuScene.h"
#include "Stage1Scene.h"

#include <iostream>
//#include <string>
//#include <vector>
//#include <memory>
//#include <algorithm>
//#include <thread>
//#include <chrono>
//#include <Windows.h>
//#include <cstdlib>

//  TODO:
//  [x] 클래스 나누기 및 정리, main 브랜치에 커밋
//  [ ] 오브젝트들 Update()에서 끝자리 판정하는 걸 어떻게 못바꾸나?
//      - 씬 만들때 넘겨주는 건 충분히 가능
//      - 근데 씬 안의 오브젝트들도 씬의 가로세로를 꼭 알아야 할까?
//  [ ] 씬의 가로 세로를 맵으로 구현 -> 카메라가 플레이어를 따라가도록
//  [ ] 충돌 감지
//  [ ] 중력 적용
//  [ ] 점프 구현
//  [ ] 몬스터 AI
//      - 낭떠러지에서 떨어지면 파괴된다
//  [ ] 버섯
// 

/*
transform.width, height size_t로
update(frame)로 변경
GetRenderingVector()로 변경
씬에서 오브젝트 삭제할 때 메모리 해제 처리 추가
씬에서 씬으로 전환은 엔진.run()에서 씬의 멤버 속성 하나 최상단에서 확인하다가 바꼈으면 해당 enum 씬으로 전환
game engine to staic? 이미 staic임..
엔진은 카메라 가로세로 / 씬은 자기 가로세로
engine.update(frame) -> scene.update(frame, scenewidth, sceneheight)
위치 변경을 오브젝트가 하지 말고 자기 update에서 델타값만 변경 - 씬에서 델타값이 0이 아닌 애들만 뱉음 - 엔진(또는 씬)이 충돌감지한 다음 실제로 위치 변경
그럼 update도 파라미터 받지 말고 엔진에서 직접 가져다 쓰도록 하는 게
*/

int main()
{
    // Need to use std::wstring
    setlocale(LC_ALL, "");

    GameEngine::Instance().ChangeScene(new Stage1Scene(120, 25));
    GameEngine::Instance().Run();

    return 0;
}
