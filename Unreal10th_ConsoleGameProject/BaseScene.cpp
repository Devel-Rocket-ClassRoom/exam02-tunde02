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
    for (auto& obj : SceneObjects)
    {
        if (!obj->IsDestroyed())
        {
            obj->Update();

            if (!obj->ShouldUpdated())
            {
                continue;
            }

            Vector2 PrevPosition = obj->GetPosition();
            Vector2 NextPosition = obj->GetTransform().Position + obj->GetTransform().Delta;

            // Collide with Player?
            bool IsCollidedWithPlayer = false;
            GameObject* player = SceneObjects[1];
            if (!obj->IsPlayer())
            {
                if (obj->GetPosition().X <= player->GetPosition().X)
                {
                    IsCollidedWithPlayer = obj->GetPosition().X + obj->GetWidth() > player->GetPosition().X
                        && obj->GetPosition().Y + obj->GetHeight() > player->GetPosition().Y;
                }
                else
                {
                    IsCollidedWithPlayer = player->GetPosition().X + player->GetWidth() > obj->GetPosition().X
                        && player->GetPosition().Y + player->GetHeight() < obj->GetPosition().Y;
                }
            }

            if (IsCollidedWithPlayer)
            {
                obj->OnCollisionEnter(SceneObjects[1]);
            }
            else if (NextPosition.X == 30 || NextPosition.X == 5)
            {
                obj->OnCollisionEnter(nullptr);
            }
            else if (0 <= NextPosition.X && NextPosition.X < Width_
                     && 0 <= NextPosition.Y && NextPosition.Y < Height_)
            {
                obj->SetPosition(NextPosition);
                obj->SetShouldUpdated(false);
            }
        }
    }

    // 2. 지연 삭제 (Update 루프가 완전히 끝난 후 플래그가 켜진 오브젝트 일괄 제거)
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