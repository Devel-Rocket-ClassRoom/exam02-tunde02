#include "BaseScene.h"
#include "GameEngine.h"
#include "Monster.h"
#include "Bullet.h"

#define USE_COLLISION_EXIT 0

BaseScene::BaseScene(int Width, int Height) : Width_(Width), Height_(Height) {}

BaseScene::~BaseScene()
{
    printf("신 오브젝트 메모리 해제\n");
    for (GameObject* SceneObject : SceneObjects)
    {
        delete SceneObject;
        SceneObject = nullptr;
    }
    SceneObjects.clear();
}

GameObject* BaseScene::Instantiate(GameObject* InGameObject, const Transform& InTransform, const Vector2& InDelta, float InTimer)
{
    GameObject* NewGameObject = InGameObject;
    NewGameObject->Initialize(InTransform, InDelta);

    InstantiateRequests.push_back(InstantiateRequest{ NewGameObject, InTransform, InDelta, InTimer });

    return NewGameObject;
}

GameObject* BaseScene::Instantiate(GameObjectType InGameObjectType, const Transform& InTransform, const Vector2& InDelta, float InTimer)
{
    GameObject* NewGameObject = nullptr;
    switch (InGameObjectType)
    {
        case GameObjectType::Monster:
            NewGameObject = new Monster;
            NewGameObject->Initialize(InTransform, InDelta);
            break;
        case GameObjectType::Bullet:
            NewGameObject = new Bullet;
            NewGameObject->Initialize(InTransform, InDelta);
            break;
        case GameObjectType::Player:
        case GameObjectType::None:
        default:
            break;
    }

    if (NewGameObject == nullptr)
    {
        return nullptr;
    }

    InstantiateRequests.push_back(InstantiateRequest{ NewGameObject, InTransform, InDelta, InTimer });
    //SceneObjects.push_back(NewGameObject);

    return NewGameObject;
}

void BaseScene::Update()
{
    for (auto& Request : InstantiateRequests)
    {
        Request.Timer -= GameEngine::Instance().GetFixedDeltaTime();

        if (Request.Timer <= 0.0f)
        {
            SceneObjects.push_back(Request.Object);
        }
    }

    InstantiateRequests.erase(
        std::remove_if(
            InstantiateRequests.begin(),
            InstantiateRequests.end(),
            [](const InstantiateRequest& request) {
                return request.Timer <= 0.0f;
            }),
        InstantiateRequests.end()
    );

    // 1. 모든 오브젝트 로직 업데이트
    for (auto& Obj : SceneObjects)
    {
        if (!Obj->IsDestroyed())
        {
            Obj->Update();
        }
    }

    // 2. 충돌 검사
#if USE_COLLISION_EXIT == 1
    size_t SceneObjectsCount = SceneObjects.size();
    for (size_t i = 0; i < SceneObjectsCount; i++)
    {
        for (size_t j = i + 1; j < SceneObjectsCount; j++)
        {
            GameObject* ObjA = SceneObjects[i];
            GameObject* ObjB = SceneObjects[j];

            if (ObjA->IsDestroyed() || ObjB->IsDestroyed())
            {
                continue;
            }

            if (CheckAABBCollision(ObjA, ObjB))
            {
                ObjA->AddCurrentCollision(ObjB);
                ObjB->AddCurrentCollision(ObjA);
            }
        }
    }

    for (auto& Obj : SceneObjects)
    {
        if (Obj->IsDestroyed())
        {
            continue;
        }

        for (auto& CurrentCollider : Obj->GetCurrentCollisions())
        {
            if (CurrentCollider == nullptr || CurrentCollider->IsDestroyed())
            {
                continue;
            }

            if (Obj->WasCollidedWith(CurrentCollider))
            {
                // 현재 충돌한 오브젝트가 PrevCollisions에 있다면
                // 계속 충돌 중이라는 뜻이므로 OnCollisionStay()
                Obj->OnCollisionStay(CurrentCollider);
            }
            else
            {
                // 현재 충돌한 오브젝트가 PrevCollisions에 없다면
                // 이제 충돌하기 시작했다는 뜻이므로 OnCollisionEnter()
                Obj->OnCollisionEnter(CurrentCollider);
            }
        }

        // 이전 프레임에 파괴된 오브젝트가 PrevCollisions에 남아있을 수 있어
        // 에러가 발생하는 중
        for (auto& PrevCollider : Obj->GetPrevCollisions())
        {
            if (PrevCollider == nullptr || PrevCollider->IsDestroyed())
            {
                continue;
            }

            if (!Obj->IsCollidedWith(PrevCollider))
            {
                // 이전에 충돌했던 오브젝트가 CurrentCollisions에 없다면
                // 충돌을 벗어났다는 뜻이므로 OnCollisionExit()
                Obj->OnCollisionExit(PrevCollider);
            }
        }

        // 오브젝트의 CurrentCollisions를 PrevCollisions로 갱신
        Obj->UpdateCollisions();
    }
#else
    size_t SceneObjectsCount = SceneObjects.size();
    for (size_t i = 0; i < SceneObjectsCount; i++)
    {
        for (size_t j = i + 1; j < SceneObjectsCount; j++)
        {
            GameObject* ObjA = SceneObjects[i];
            GameObject* ObjB = SceneObjects[j];

            if (ObjA->IsDestroyed() || ObjB->IsDestroyed())
            {
                continue;
            }

            if (CheckAABBCollision(ObjA, ObjB))
            {
                ObjA->OnCollisionEnter(ObjB);
                ObjB->OnCollisionEnter(ObjA);
            }
        }
    }
#endif

    // 3. 외곽 검사
    for (auto& Obj : SceneObjects)
    {
        if (Obj->IsDestroyed())
        {
            continue;
        }

        //if (!(0 <= Obj->GetNextMinX() && Obj->GetNextMaxX() < Width_
        //      && 0 <= Obj->GetNextMinY() && Obj->GetNextMaxY() < Height_))
        //{
        //    Obj->CancelMove();
        //    Obj->OnCollisionEnter(nullptr);
        //}

        if (!(0 <= Obj->GetNextMinX() && Obj->GetNextMaxX() < Width_))
        {
            Obj->Destroy();
        }
        if (!(0 <= Obj->GetNextMinY() && Obj->GetNextMaxY() < Height_))
        {
            Obj->Destroy();
        }

        //if (!(0 <= Obj->GetNextMinX() && Obj->GetNextMaxX() < Width_))
        //{
        //    Obj->CancelXMove();
        //    //Obj->OnCollisionEnter(nullptr);
        //}
    }

    // 4. 이동 적용
    for (auto& obj : SceneObjects)
    {
        if (!obj->IsDestroyed())
        {
            obj->ApplyMove();
        }
    }

    // 5. 지연 삭제 (Update 루프가 완전히 끝난 후 플래그가 켜진 오브젝트 일괄 제거)
    for (auto& Obj : SceneObjects)
    {
        if (Obj != nullptr && Obj->IsDestroyed())
        {
            delete Obj;
            Obj = nullptr;
        }
    }

    SceneObjects.erase(
        std::remove_if(
            SceneObjects.begin(),
            SceneObjects.end(),
            [](const GameObject* obj) {
                return obj == nullptr || obj->IsDestroyed();
            }),
        SceneObjects.end()
    );
}

void BaseScene::Render()
{
    InitializeScreen();
    RenderSceneObjects();
    RenderStatus();
    PrintScreen();
}

bool BaseScene::CheckAABBCollision(const GameObject* ObjA, const GameObject* ObjB)
{
    if (ObjA->GetNextMaxX() <= ObjB->GetNextMinX())
    {
        return false;
    }

    if (ObjB->GetNextMaxX() <= ObjA->GetNextMinX())
    {
        return false;
    }

    if (ObjA->GetNextMaxY() <= ObjB->GetNextMinY())
    {
        return false;
    }

    if (ObjB->GetNextMaxY() <= ObjA->GetNextMinY())
    {
        return false;
    }

    return true;
}

void BaseScene::InitializeScreen(const std::wstring C)
{
    Screen.clear();
    Screen.assign(Height_, L"");
    for (size_t i = 0; i < Height_; i++)
    {
        for (size_t j = 0; j < Width_; j++)
        {
            Screen[i] += C;
        }
    }
}

void BaseScene::RenderSceneObjects()
{
    for (auto& obj : SceneObjects)
    {
        for (size_t i = 0; i < obj->GetHeight(); i++)
        {
            for (size_t j = 0; j < obj->GetWidth(); j++)
            {
                if (Screen[obj->GetPosition().Y + i][obj->GetPosition().X + j] == L' '
                    && obj->GetRenderingVector()[i][j] != L' ')
                {
                    Screen[obj->GetPosition().Y + i][obj->GetPosition().X + j] = obj->GetRenderingVector()[i][j];
                }
            }
        }
    }
}

void BaseScene::RenderStatus()
{
    const size_t StatusStartX = 61;

    // 위, 아래 테두리
    for (size_t i = StatusStartX; i < Width_; i++)
    {
        Screen[0][i] = L'█';
        Screen[Height_ - 2][i] = L'█';
    }

    // 오른쪽 테두리
    for (size_t i = 1; i < Height_ - 2; i++)
    {
        Screen[i][Width_ - 1] = L'█';
    }

    // 스테이지
    std::wstring StageTextStr = L"S T A G E";
    Screen[3].replace(GetTextStartX(StatusStartX, StageTextStr.length()), StageTextStr.length(), StageTextStr);

    std::wstring StageNumberStr = L"1";
    Screen[5].replace(GetTextStartX(StatusStartX, StageNumberStr.length()), StageNumberStr.length(), StageNumberStr);

    for (size_t i = StatusStartX; i < Width_; i++)
    {
        Screen[8][i] = L'█';
    }

    // 점수
    std::wstring ScoreTextStr = L"S C O R E";
    Screen[11].replace(GetTextStartX(StatusStartX, ScoreTextStr.length()), ScoreTextStr.length(), ScoreTextStr);

    std::wstring ScoreNumberStr = L"000,000,000";
    Screen[13].replace(GetTextStartX(StatusStartX, ScoreNumberStr.length()), ScoreNumberStr.length(), ScoreNumberStr);

    for (size_t i = StatusStartX; i < Width_; i++)
    {
        Screen[16][i] = L'█';
    }

    // 플레이어 체력
    std::wstring PlayerTextStr = L"P L A Y E R";
    Screen[19].replace(GetTextStartX(StatusStartX, PlayerTextStr.length()), PlayerTextStr.length(), PlayerTextStr);

    //std::wstring PlayerHpStr = L"♡ ♡ ♡";
    std::wstring PlayerHpStr = L"";
    for (int i = 0; i < Player_->GetHp() - 1; i++)
    {
        PlayerHpStr += L"♡ ";
    }
    PlayerHpStr += L"♡";
    Screen[21].replace(GetTextStartX(StatusStartX, PlayerHpStr.length()), PlayerHpStr.length(), PlayerHpStr);

    for (size_t i = StatusStartX; i < Width_; i++)
    {
        Screen[24][i] = L'█';
    }
}

size_t BaseScene::GetTextStartX(const size_t UiStartX, const size_t Length) const
{
    return ((Width_ - UiStartX) - Length) / 2 + UiStartX;
}

void BaseScene::PrintScreen()
{
    std::wstring Buffer{};
    Buffer.reserve(Width_ + 1 * Height_);
    for (int i = 0; i < Height_; i++)
    {
        Buffer += Screen[i];
        Buffer += L"\n";
    }

    wprintf(L"%s", Buffer.c_str());
}
