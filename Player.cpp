#include "Player.h"
#include <algorithm>
#include <cmath>

void Player::Initialize() {
    spriteSheet_ = Novice::LoadTexture("player/player_1.png");
    frameWidth_ = 128;
    frameHeight_ = 128;
    frameCount_ = 4;
    currentFrame_ = 0;
    animationTimer_ = 0;

    x_ = 100;
    y_ = 300;
    speed_ = 5.0f;

    isAttacking_ = false;
    attackTimer_ = 0;
    attackPhase_ = 0;
    comboTimer_ = 0;

    isDashing_ = false;
    dashTimer_ = 0;
    dashCooldownTimer_ = 0;
    afterImages_.clear();
    afterImageTimer_ = 0;

    isUltimateActive_ = false;
    ultimateTimer_ = 0;
    ultimateCooldown_ = 0;
    slashRays_.clear();
}

void Player::Update(char* keys) {
    bool moving = false;

    // --- 移动 ---
    if (keys[DIK_D]) {
        x_ += speed_; moving = true; left = false;
    }
    if (keys[DIK_A]) {
        x_ -= speed_; moving = true; left = true;
    }
    if (keys[DIK_W]) {
        y_ -= speed_; moving = true;
    }
    if (keys[DIK_S]) {
        y_ += speed_; moving = true;
    }

    // 限制上下边界
    if (y_ < kLaneTop) y_ = (float)kLaneTop;
    if (y_ > kLaneBottom - frameHeight_) y_ = (float)(kLaneBottom - frameHeight_);

    // --- 动画更新 ---
    if (moving) {
        animationTimer_++;
        if (animationTimer_ > 10) {
            currentFrame_ = (currentFrame_ + 1) % frameCount_;
            animationTimer_ = 0;
        }
    }

    // --- 攻击输入检测（三连击逻辑） ---
    if (Novice::IsPressMouse(0)) {
        if (!isAttacking_ && comboTimer_ <= 0) {
            // 第一击
            isAttacking_ = true;
            attackPhase_ = 1;
            attackTimer_ = attackDuration_;
        }
        else if (!isAttacking_ && comboTimer_ > 0 && attackPhase_ < 3) {
            // 后续连击
            isAttacking_ = true;
            attackPhase_++;
            attackTimer_ = attackDuration_;
        }
    }

    // --- 攻击计时 ---
    if (isAttacking_) {
        attackTimer_--;
        if (attackTimer_ <= 0) {
            isAttacking_ = false;
            comboTimer_ = comboWindow_; // 开启连击窗口
        }
    }
    else {
        // 连击输入窗口计时
        if (comboTimer_ > 0) {
            comboTimer_--;
            if (comboTimer_ <= 0) {
                attackPhase_ = 0; // 超时重置连段
            }
        }
    }

    // --- 冲刺触发（Shift键） ---
    if (!isDashing_ && dashCooldownTimer_ <= 0 && keys[DIK_LSHIFT]) {
        int mouseX, mouseY;
        Novice::GetMousePosition(&mouseX, &mouseY);

        float px = x_ + frameWidth_ / 2.0f;
        float py = y_ + frameHeight_ / 2.0f;
        float dx = (float)mouseX - px;
        float dy = (float)mouseY - py;
        float len = sqrtf(dx * dx + dy * dy);

        if (len > 0.01f) {
            dx /= len;
            dy /= len;
        }

        dashDirX_ = dx;
        dashDirY_ = dy;
        isDashing_ = true;
        dashTimer_ = dashDuration_;
        dashCooldownTimer_ = dashCooldown_;
    }

    // --- 冲刺中 ---
    if (isDashing_) {
        x_ += dashDirX_ * dashSpeed_;
        y_ += dashDirY_ * dashSpeed_;
        dashTimer_--;

        // 生成残影
        afterImageTimer_++;
        if (afterImageTimer_ >= afterImageInterval_) {
            AfterImage img{ x_, y_, currentFrame_, 255.0f };
            afterImages_.push_back(img);
            if (afterImages_.size() > 8) {
                afterImages_.erase(afterImages_.begin());
            }
            afterImageTimer_ = 0;
        }

        if (dashTimer_ <= 0) {
            isDashing_ = false;
            afterImages_.clear();
        }
    }

    // --- 冲刺冷却 ---
    if (dashCooldownTimer_ > 0) {
        dashCooldownTimer_--;
    }

    // ==================== 必杀技逻辑 ====================
    if (!isUltimateActive_) {
        if (ultimateCooldown_ > 0) ultimateCooldown_--;
        if (keys[DIK_R] && ultimateCooldown_ <= 0) {
            isUltimateActive_ = true;
            ultimateTimer_ = 0;
            ultimateCooldown_ = ultimateCooldownMax_;
            slashRays_.clear();
        }
    }
    else {
        ultimateTimer_++;

        // 每帧生成随机光线
        for (int i = 0; i < 1; i++) {
            float x = static_cast<float>(rand() % 1280);
            float y = static_cast<float>(rand() % 720);
            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
            float speed = 15.0f + rand() % 25;
            float length = 40.0f + rand() % 60;

            slashRays_.push_back({ x, y, cosf(angle), sinf(angle), speed, length });
        }

        // 更新光线位置
        for (auto& r : slashRays_) {
            r.x += r.dx * r.speed * 2;
            r.y += r.dy * r.speed * 2;
        }

        // 必杀技结束
        if (ultimateTimer_ >= ultimateDuration_) {
            isUltimateActive_ = false;
            slashRays_.clear();
        }
    }
}

void Player::Draw(int scrollX) {
    // --- 攻击范围显示 ---
    if (isAttacking_) {
        int mouseX, mouseY;
        Novice::GetMousePosition(&mouseX, &mouseY);

        float px = x_ + frameWidth_ / 2.0f;
        float py = y_ + frameHeight_ / 2.0f;
        float dx = (float)mouseX - px;
        float dy = (float)mouseY - py;
        float angle = atan2f(dy, dx);

        switch (attackPhase_) {
        case 1: // 月牙斩
            Novice::DrawEllipse(
                (int)(px + cosf(angle) * 80 - scrollX),
                (int)(py + sinf(angle) * 80),
                20, 60, angle, 0xFF00FFFF, kFillModeWireFrame
            );
            break;
        case 2: // 横扫斩
            Novice::DrawEllipse(
                (int)(px + cosf(angle) * 120 - scrollX),
                (int)(py + sinf(angle) * 120),
                30, 80, angle, 0xFFFF00FF, kFillModeWireFrame
            );
            break;
        case 3: // 旋风斩
            Novice::DrawEllipse(
                (int)(px - scrollX),
                (int)(py),
                120, 120, 0.0f, RED, kFillModeWireFrame
            );
            break;
        }
    }

    // --- 残影绘制 ---
    for (size_t i = 0; i < afterImages_.size(); i++) {
        float alpha = afterImages_[i].alpha - (i * 30.0f);
        if (alpha < 0) alpha = 0;
        unsigned int color = (int(alpha) << 24) | 0xFFFFFF;

        if (!left) {
            Novice::DrawSpriteRect(
                (int)afterImages_[i].x - scrollX,
                (int)afterImages_[i].y,
                afterImages_[i].frame * frameWidth_, 0,
                frameWidth_, frameHeight_,
                spriteSheet_, 0.25f, 1.0f, 0.0f, color
            );
        }
        else {
            Novice::DrawSpriteRect(
                (int)(afterImages_[i].x - scrollX + frameWidth_),
                (int)afterImages_[i].y,
                afterImages_[i].frame * frameWidth_, 0,
                frameWidth_, frameHeight_,
                spriteSheet_, -0.25f, 1.0f, 0.0f, color
            );
        }
    }

    // --- 绘制玩家 ---
    if (!left) {
        Novice::DrawSpriteRect(
            (int)x_ - scrollX, (int)y_,
            currentFrame_ * frameWidth_, 0,
            frameWidth_, frameHeight_,
            spriteSheet_, 0.25f, 1.0f, 0.0f, WHITE
        );
    }
    else {
        Novice::DrawSpriteRect(
            (int)(x_ - scrollX + frameWidth_), (int)y_,
            currentFrame_ * frameWidth_, 0,
            frameWidth_, frameHeight_,
            spriteSheet_, -0.25f, 1.0f, 0.0f, WHITE
        );
    }

    // ==================== 必杀技演出 ====================
    if (isUltimateActive_) {
        // 黑屏
        Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x000000DF, kFillModeSolid);

        // 绘制光线
        for (const auto& r : slashRays_) {
            Novice::DrawLine(
                (int)r.x, (int)r.y,
                (int)(r.x + r.dx * r.length * 4),
                (int)(r.y + r.dy * r.length * 4),
                WHITE
            );
        }
    }
}


///属性---道具
void Player::AddMaxHP(int value) {
    stats_.maxHP += value;
    if (stats_.maxHP < 1) stats_.maxHP = 1;
}

void Player::AddHP(int value) {
    stats_.hp += value;
    if (stats_.hp > stats_.maxHP) stats_.hp = stats_.maxHP;
    if (stats_.hp < 0) stats_.hp = 0;
}

void Player::AddAttack(int value) {
    stats_.attack += value;
}

void Player::AddMoveSpeed(float value) {
    stats_.moveSpeed += value;
}

void Player::AddDefense(float value) {
    stats_.defense += value;
}
///道具


