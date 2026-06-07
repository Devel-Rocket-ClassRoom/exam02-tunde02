#include "BaseScene.h"

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

void BaseScene::Update()
{
    // 1. 모든 오브젝트 로직 업데이트
    for (auto& Obj : SceneObjects)
    {
        if (!Obj->IsDestroyed())
        {
            Obj->Update();
        }
    }

    // 2. 충돌 검사
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
                ObjA->CancelMove();
                ObjB->CancelMove();

                ObjA->OnCollisionEnter(ObjB);
                ObjB->OnCollisionEnter(ObjA);
            }
        }
    }

    // 3. 외곽 검사
    for (auto& Obj : SceneObjects)
    {
        if (Obj->IsDestroyed())
        {
            continue;
        }

        if (!(0 <= Obj->GetNextMinX() && Obj->GetNextMaxX() < Width_
            && 0 <= Obj->GetNextMinY() && Obj->GetNextMaxY() < Height_))
        {
            Obj->CancelMove();
            Obj->OnCollisionEnter(nullptr);
        }
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
    // TODO: 별도 배열에 파괴된 오브젝트들 추가하고, for문 밖에서 이것들 delete
    SceneObjects.erase(
        std::remove_if(SceneObjects.begin(), SceneObjects.end(),
        [](const GameObject* obj) {
        return obj->IsDestroyed();
    }),
        SceneObjects.end()
    );
}

void BaseScene::Render()
{
    std::vector<std::wstring> Screen(Height_, L"");
    for (size_t i = 0; i < Height_; i++)
    {
        for (size_t j = 0; j < Width_; j++)
        {
            Screen[i] += L"█";
        }
    }

    for (auto& obj : SceneObjects)
    {
        for (size_t i = 0; i < obj->GetHeight(); i++)
        {
            for (size_t j = 0; j < obj->GetWidth(); j++)
            {
                Screen[obj->GetPosition().Y + i][obj->GetPosition().X + j] = obj->GetRenderingVector()[i][j];
            }
        }
    }

    std::wstring Buffer{};
    Buffer.reserve(Width_ + 1 * Height_);
    for (int i = 0; i < Height_; i++)
    {
        Buffer += Screen[i];
        Buffer += L"\n";
    }

    wprintf(L"%s", Buffer.c_str());
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
