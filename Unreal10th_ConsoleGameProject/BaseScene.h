#pragma once
#include "GameObject.h"
#include <vector>

// ==========================================
// 2. 씬 / 스테이지 관리를 위한 부모 추상 클래스
// ==========================================
class BaseScene
{
protected:
    // 각 씬은 자신만의 독립된 오브젝트 리스트를 관리함
    std::vector<GameObject*> SceneObjects;
    size_t Width_ = 0;
    size_t Height_ = 0;

public:
    BaseScene(int Width, int Height);
    virtual ~BaseScene();

    virtual void Enter() = 0;  // 씬 시작 시 (데이터 로드, 배치)
    virtual void Exit() = 0;   // 씬 종료 시 (자원 해제)

    // 현재 씬에 속한 오브젝트들 업데이트 및 지연 삭제 처리
    virtual void Update();

    // 현재 씬의 오브젝트들 렌더링
    virtual void Render();
};
