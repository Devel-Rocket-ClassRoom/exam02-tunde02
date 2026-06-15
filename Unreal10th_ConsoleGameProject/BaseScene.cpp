#include "BaseScene.h"
#include "GameEngine.h"
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"

BaseScene::BaseScene(int Width, int Height) : Width_(Width), Height_(Height) {}

BaseScene::~BaseScene()
{
    for (GameObject* SceneObject : SceneObjects)
    {
        delete SceneObject;
        SceneObject = nullptr;
    }
    SceneObjects.clear();
}

GameObject* BaseScene::Instantiate(GameObject* InGameObject, const Vector2& InPosition, const Vector2& InDelta, float InTimer)
{
    GameObject* NewGameObject = InGameObject;
    NewGameObject->Initialize(InPosition, InDelta);

    InstantiateRequests.push_back(InstantiateRequest{ NewGameObject, InPosition, InDelta, InTimer });

    return NewGameObject;
}

void BaseScene::Update()
{
    // 생성 요청의 타이머를 갱신
    // 타이머가 다 됐다면 생성
    for (auto& Request : InstantiateRequests)
    {
        Request.Timer -= GameEngine::Instance().GetFixedDeltaTime();

        if (Request.Timer < 0.0f)
        {
            SceneObjects.push_back(Request.Object);
        }
    }

    InstantiateRequests.erase(
        std::remove_if(
            InstantiateRequests.begin(),
            InstantiateRequests.end(),
            [](const InstantiateRequest& request) {
                return request.Timer < 0.0f;
            }),
        InstantiateRequests.end()
    );

    // 모든 오브젝트 로직 업데이트
    for (auto& Obj : SceneObjects)
    {
        if (!Obj->IsDestroyed())
        {
            Obj->Update();
        }
    }

    // 충돌 검사, 외곽 검사, 1칸씩 이동을 동시에
    size_t SceneObjectsSize = SceneObjects.size();
    for (size_t i = 0; i < SceneObjectsSize; i++)
    {
        GameObject* ObjA = SceneObjects[i];

        if (ObjA->IsDestroyed())
        {
            continue;
        }

        TryXMove(ObjA);
        TryYMove(ObjA);
    }

    // 지연 삭제 (Update 루프가 완전히 끝난 후 플래그가 켜진 오브젝트 일괄 제거)
    for (auto& Obj : SceneObjects)
    {
        if (Obj != nullptr && Obj->IsDestroyed())
        {
            if (Obj->GetCollisionLayer() == CollisionLayer::Monster)
            {
                Score += 1000;
            }
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
    PrintScreen();
}

void BaseScene::TryXMove(GameObject* ObjA)
{
    auto [ObjAPosX, ObjAPosY] = ObjA->GetPosition().ToRoundInt();
    float PredictedX = ObjA->GetDelta().X * ObjA->GetSpeed();

    if (PredictedX == 0.0f)
    {
        return;
    }

    float StepX = ObjA->GetDelta().X;
    float SumX = 0.0f;

    while (std::abs(SumX) < std::abs(PredictedX))
    {
        // 마지막 루프에서 남은 거리가 StepX보다 작다면, 남은 만큼만 이동하도록 보정
        if (std::abs(PredictedX - SumX) < std::abs(StepX))
        {
            StepX = PredictedX - SumX;
        }

        // 이번 루프에서 이동할 누적치 갱신
        SumX += StepX;

        int IntNextX = static_cast<int>(std::floor(ObjAPosX + SumX));

        // 외곽 검사
        if (IntNextX < 1)
        {
            ObjA->SetPosition({ static_cast<float>(1), ObjA->GetPosition().Y });
            ObjA->OnCollisionEnter(nullptr);
            return;
        }
        else if (IntNextX + ObjA->GetWidth() > GameAreaWidth)
        {
            ObjA->SetPosition({ static_cast<float>(GameAreaWidth - ObjA->GetWidth()), ObjA->GetPosition().Y });
            ObjA->OnCollisionEnter(nullptr);
            return;
        }

        // 충돌 검사
        size_t SceneObjectsSize = SceneObjects.size();
        for (size_t j = 0; j < SceneObjectsSize; j++)
        {
            if (SceneObjects[j]->IsDestroyed() || SceneObjects[j] == ObjA)
            {
                continue;
            }

            GameObject* ObjB = SceneObjects[j];

            // 1. 충돌 가능한 상태인가
            // 2. 충돌 레이어가 서로 다른가
            // 3. 오브젝트가 서로 다른 진영인가
            // 세 가지 조건을 만족해야만 충돌 검사 진행
            if (!(ObjA->CanCollide() && ObjB->CanCollide())
                || ObjA->GetCollisionLayer() == ObjB->GetCollisionLayer()
                || ObjA->GetFaction() == ObjB->GetFaction())
            {
                // 플레이어와 아이템의 경우는 예외
                if (!IsPlayerAndItem(ObjA, ObjB))
                {
                    continue;
                }
            }

            auto [ObjBPosX, ObjBPosY] = ObjB->GetPosition().ToRoundInt();
            if (CheckAABBCollision(IntNextX, ObjAPosY, ObjA->GetWidth(), ObjA->GetHeight(),
                                   ObjBPosX, ObjBPosY, ObjB->GetWidth(), ObjB->GetHeight()))
            {
                ObjA->OnCollisionEnter(ObjB);
                ObjB->OnCollisionEnter(ObjA);
                return;
            }
        }

        ObjA->ApplyXMove(StepX);
    }
}

void BaseScene::TryYMove(GameObject* ObjA)
{
    auto [ObjAPosX, ObjAPosY] = ObjA->GetPosition().ToRoundInt();
    float PredictedY = ObjA->GetDelta().Y * ObjA->GetSpeed();

    if (PredictedY == 0.0f)
    {
        return;
    }

    float StepY = ObjA->GetDelta().Y;
    float SumY = 0.0f;

    while (std::abs(SumY) < std::abs(PredictedY))
    {
        // 마지막 루프에서 남은 거리가 StepY보다 작다면, 남은 만큼만 이동하도록 보정
        if (std::abs(PredictedY - SumY) < std::abs(StepY))
        {
            StepY = PredictedY - SumY;
        }

        // 이번 루프에서 이동할 누적치 갱신
        SumY += StepY;

        int IntNextY = static_cast<int>(std::floor(ObjAPosY + SumY));

        // 외곽 검사
        if (IntNextY < 1)
        {
            ObjA->SetPosition({ ObjA->GetPosition().X, static_cast<float>(1) });
            ObjA->OnCollisionEnter(nullptr);
            return;
        }
        else if (IntNextY + ObjA->GetHeight() > GameAreaHeight)
        {
            ObjA->SetPosition({ ObjA->GetPosition().X, static_cast<float>(GameAreaHeight - ObjA->GetHeight()) });
            ObjA->OnCollisionEnter(nullptr);
            return;
        }

        // 충돌 검사
        size_t SceneObjectsSize = SceneObjects.size();
        for (size_t j = 0; j < SceneObjectsSize; j++)
        {
            if (SceneObjects[j]->IsDestroyed() || SceneObjects[j] == ObjA)
            {
                continue;
            }

            GameObject* ObjB = SceneObjects[j];

            // 1. 충돌 가능한 상태인가
            // 2. 충돌 레이어가 서로 다른가
            // 3. 오브젝트가 서로 다른 진영인가
            // 세 가지 조건을 만족해야만 충돌 검사 진행
            if (!(ObjA->CanCollide() && ObjB->CanCollide())
                || ObjA->GetCollisionLayer() == ObjB->GetCollisionLayer()
                || ObjA->GetFaction() == ObjB->GetFaction())
            {
                // 플레이어와 아이템의 경우는 예외
                if (!IsPlayerAndItem(ObjA, ObjB))
                {
                    continue;
                }
            }

            auto [ObjBPosX, ObjBPosY] = ObjB->GetPosition().ToRoundInt();
            if (CheckAABBCollision(ObjAPosX, IntNextY, ObjA->GetWidth(), ObjA->GetHeight(),
                                   ObjBPosX, ObjBPosY, ObjB->GetWidth(), ObjB->GetHeight()))
            {
                ObjA->OnCollisionEnter(ObjB);
                ObjB->OnCollisionEnter(ObjA);
                return;
            }
        }

        ObjA->ApplyYMove(StepY);
    }
}

bool BaseScene::CheckAABBCollision(int ObjAPosX, int ObjAPosY, size_t ObjAWidth, size_t ObjAHeight, int ObjBPosX, int ObjBPosY, size_t ObjBWidth, size_t ObjBHeight)
{
    if (ObjAPosX + ObjAWidth <= ObjBPosX)
    {
        return false;
    }

    if (ObjBPosX + ObjBWidth <= ObjAPosX)
    {
        return false;
    }

    if (ObjAPosY + ObjAHeight <= ObjBPosY)
    {
        return false;
    }

    if (ObjBPosY + ObjBHeight <= ObjAPosY)
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
        if (obj->IsDestroyed())
        {
            continue;
        }

        for (size_t i = 0; i < obj->GetHeight(); i++)
        {
            for (size_t j = 0; j < obj->GetWidth(); j++)
            {
                auto [PositionX, PositionY] = obj->GetPosition().ToRoundInt();
                if (Screen[PositionY + i][PositionX + j] == L' '
                    && obj->GetRenderingVector()[i][j] != L' ')
                {
                    Screen[PositionY + i][PositionX + j] = obj->GetRenderingVector()[i][j];
                }
            }
        }
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
