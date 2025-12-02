#include "Item.h"
#include "Player.h"
#include <cmath>

void Item::Initialize(int x, int y, int type) {
    position_.x = x;
    position_.y = y;
    type_ = type;
    active_ = true;

    switch (type_) {
    case 0: texture_ = Novice::LoadTexture("Item/item_0.png"); break;
    case 1: texture_ = Novice::LoadTexture("Item/item_1.png"); break;
    case 2: texture_ = Novice::LoadTexture("Item/item_2.png"); break;
    case 3: texture_ = Novice::LoadTexture("Item/item_3.png"); break;
    case 4: texture_ = Novice::LoadTexture("Item/item_4.png"); break;
    case 5: texture_ = Novice::LoadTexture("Item/item_5.png"); break;
    case 6: texture_ = Novice::LoadTexture("Item/item_6.png"); break;
    case 7: texture_ = Novice::LoadTexture("Item/item_7.png"); break;
    case 8: texture_ = Novice::LoadTexture("Item/item_8.png"); break;
    case 9: texture_ = Novice::LoadTexture("Item/item_9.png"); break;
    default: texture_ = Novice::LoadTexture("Item/item_0.png"); break;
    }
}

void Item::Update(Player* player) {
    if (!active_ || player == nullptr) return;

    // 玩家底部矩形
    float px = player->GetX();         // 玩家左上角 X
    float py = player->GetY();         // 玩家左上角 Y
    float pw = 32.0f;                  // 脚部宽度（可调小于玩家实际宽度）
    float ph = 10.0f;                  // 脚部高度（脚步厚度）

    float playerLeft = px + 30;        // 偏移到脚部中心
    float playerTop = py + 118;        // y + frameHeight_ - 脚部高度
    float playerRight = playerLeft + pw;
    float playerBottom = playerTop + ph;

    // 道具矩形
    float itemLeft = (float)position_.x;
    float itemTop = (float)position_.y;
    float itemRight = itemLeft + (float)width_;
    float itemBottom = itemTop + (float)height_;

    // AABB 碰撞检测
    if (playerRight > itemLeft && playerLeft < itemRight &&
        playerBottom > itemTop && playerTop < itemBottom) {
        active_ = false;

        // 可触发道具效果
 // ★★★★★ 触发道具效果 ★★★★★
        switch (type_) {

        case 0:
            // 血量 +20
            player->AddHP(20);
            break;

        case 1:
            // 最大 HP +10
            player->AddMaxHP(10);
            break;

        case 2:
            // 攻击力 +5
            player->AddAttack(5);
            break;

        case 3:
            // 防御力 +1
            player->AddDefense(1.0f);
            break;

        case 4:
            // 移动速度 +0.5
            player->AddMoveSpeed(0.5f);
            break;

        case 5:
            // 全恢复
            player->AddHP(9999);
            break;

        case 6:
            // 小型治疗
            player->AddHP(10);
            break;

        case 7:
            // 强力攻击提升
            player->AddAttack(15);
            break;

        case 8:
            // 强力防御提升
            player->AddDefense(5.0f);
            player->AddMoveSpeed(-1.0f);

            break;

        case 9:
            player->AddMoveSpeed(2.0f);
            break;
        }
    }
}



void Item::Draw(int scrollX) {
    if (!active_) return;

    Novice::DrawSprite(
        position_.x - scrollX,
        position_.y,
        texture_,
        1.0f, 1.0f, 0.0f, WHITE
    );
}
