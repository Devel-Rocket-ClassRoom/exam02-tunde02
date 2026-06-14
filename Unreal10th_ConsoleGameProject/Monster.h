#pragma once
#include "GameObject.h"
#include <unordered_map>

class Monster : public GameObject
{
public:
    Monster();
    Monster(int InX, int InY);
    Monster(MonsterType InMonsterType);
    Monster(MonsterType InMonsterType, Vector2 InDelta);

    virtual void Initialize(const Transform& InTransform, const Vector2& InDelta) override;
    virtual void Update() override;
    virtual void OnCollisionEnter(GameObject* Other) override;

    void TurnAround();
    void FireBullet() const;

private:
    const float MonsterShotDelay = 0.1f;

    MonsterType MonsterType_ = MonsterType::Default;
    BulletType BulletType_ = BulletType::Default;
    float ShotDelay = 0.0f;

    struct MonsterSpec
    {
        MonsterType Type;
        size_t Width;
        size_t Height;
        int Hp;
        int Damage;
        float Speed;
        BulletType BulletType_;
        std::vector<std::wstring> RenderString;
    };

    inline static const std::unordered_map<MonsterType, MonsterSpec> MonsterSpecs = {
        {
            MonsterType::Default,
            {
                MonsterType::Default,
                5,
                3,
                3,
                1,
                1.0f,
                BulletType::Default,
                {
                    L"█ ░ █",
                    L"▓███▓",
                    L" ░▓░ "
                }
            }
        },
        {
            MonsterType::TripleShot,
            {
                MonsterType::TripleShot,
                7,
                5,
                5,
                1,
                0.5f,
                BulletType::Default,
                {
                    L"  ░█░  ",
                    L" ░▓█▓░ ",
                    L"▒▓███▓▒",
                    L" ░▓▓▓░ ",
                    L" █ █ █ "
                }
            }
        },
        {
            MonsterType::Settled,
            {
                MonsterType::Settled,
                7,
                5,
                10,
                2,
                0.0f,
                BulletType::Upgrade_2,
                {
                    L"   ░   ",
                    L"▒ ▓▓▓ ▒",
                    L" ░▓█▓░ ",
                    L"▒ ▓▓▓ ▒",
                    L"   ░   "
                }
            }
        },
        {
            MonsterType::Boss,
            {
                MonsterType::Boss,
                8,
                4,
                50,
                5,
                0.2f,
                BulletType::Upgrade_1,
                {
                    L"⢀⣠⣴⣿⣿⣦⣄⡀",
                    L"⢺⣿⣿BO⣿⣿⡗",
                    L"⠙⢿⣿SS⣿⡿⠋",
                    L" ⠘⠻⠿⠿⠟⠃ "
                }
            }
        },
    };
};

/*

L"█ ░ █",
L"▓███▓",
L" ░▓░ "


L" █ ░ █ ",
L" ▓▒ ▒▓ ",
L"▒▓███▓▒",
L" ░▓▓▓░ ",
L"   ░   "

L"  ░░░  ",
L" ▒▓█▓▒ ",
L"█▒▓█▓▒█",
L" ░▒▓▒░ ",
L"   ░   "

L"  ░█░  ",
L" ░▓█▓░ ",
L"▒▓███▓▒",
L" ░▓▓▓░ ",
L" █ █ █ "

L"           ▄▄█████▄▄           ", // 최상단 주 컴퓨터 타워
L"        ▄█████████████▄        ",
L" ░░   ▄████▓▓▒░█░▒▓▓████▄   ░░ ", // 보조 유도 안테나 라인
L"▒▒▒▓██████▓▒ ▄▀█▀▄ ▒▓██████▓▒▒▒", // 중앙 메인 제어실 보호 격벽
L"████████▓▒░ █ █ █ █ ░▒▓████████", // 상부 메인 에너지 서클
L"██████▓▒░  █████████  ░▒▓██████", // 내부 격납고 진입로 시각화
L" ▀███▓▒░ ▄███▀▀█▀▀███▄ ░▒▓███▀ ", // 메인 빔포 충전 에너지 가이드
L"   ▀█▓░ ███▀   █   ▀███ ░▓█▀   ",
L"    █  ██▀    ▄█▄    ▀██  █    ", // 좌우 세컨더리 레이저 포대 활성화
L"       █     █████     █       ", // 중앙 하이퍼 메가 입자포 포신
L"             ▀▀▀▀▀             "  // 포구 종단면 배출구

L"        ■■■█████████■■■        ", // 상단 중장갑 헤드
L"       ■███████████████■       ",
L" ◢■◣  ███████▀▀█▀▀███████  ◢■◣ ", // 상부 부포탑 라인
L"■███■██████▀   █   ▀██████■███■", // 좌우 고출력 제트 부스터
L"██████████▀ ▄█████▄ ▀██████████", // 중앙 초고밀도 입자 방출구
L"◥████████  █████████  ████████◤",
L"  ▀██████▄ ▀███████▀ ▄██████▀  ", // 전면 미사일 격납고 해치
L"   ░██████▄   ▀█▀   ▄██████░   ",
L"   ▒▓███████▄     ▄███████▓▒   ", // 하단 중력 제어 날개
L"   ░▒▓███████     ███████▓▒░   ",
L"     ▀▀▀▀▀▀▀       ▀▀▀▀▀▀▀     " // 듀얼 플라즈마 분사구

L"            ⢰█████⡆           ",
L"         ░ ⢺███████⡗ ░         ", // 상단 수호 룬 문자
L" ✧ ✦ ✧  ░▒▓█████████▓▒░  ✧ ✦ ✧ ",
L"███████▄█████▓▒♦▒▓█████▄███████", // 좌우 독립 부유 마법 구체
L"◥██████████▓▒ ░█░ ▒▓██████████◤",
L" ░▒▓██████▓▒ ✧███✧ ▒▓██████▓▒░ ", // 중앙 마나 핵 (Runic Core)
L"   ▀██████▒░ ▀███▀ ░▒██████▀   ",
L"  ▄██████▓░   ▄█▄   ░▓██████▄  ", // 마력 유도 가이드 라인
L" ███████▀    █████    ▀███████ ",
L" ◥█████◤     ▀▀▀▀▀     ◥█████◤ ", // 하부 충격파 마법진
L"   ▀▀▀                 ▀▀▀     "

*/
