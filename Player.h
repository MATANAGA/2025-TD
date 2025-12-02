#pragma once
#include <Novice.h>
#include <vector>
#include <cmath>

struct AfterImage {
    float x = 0.0f;
    float y = 0.0f;
    int frame = 0;
    float alpha = 0.0f;
};

struct SlashRay {
    float x = 0.0f;
    float y = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;
    float speed = 0.0f;
    float length = 0.0f;
};

struct PlayerStats {
    int maxHP = 100;
    int hp = 100;
    int attack = 10;
    float moveSpeed = 5.0f;
    float defense = 0.0f;

    int skillCooldown = 300;
    int skillCooldownTimer = 0;
};

class Player {
public:
    void Initialize();
    void Update(char* keys);
    void Draw(int scrollX);

    float GetX() const {
        return x_;
    }
    float GetY() const {
        return y_;
    }
    
    void AddMaxHP(int value);
    void AddHP(int value);
    void AddAttack(int value);
    void AddMoveSpeed(float value);
    void AddDefense(float value);

    // 获取 stats（如果你要直接读属性）
    PlayerStats& GetStats() {
        return stats_;
    }



private:
    int spriteSheet_ = 0;
    int frameWidth_ = 0;
    int frameHeight_ = 0;
    int frameCount_ = 0;
    int currentFrame_ = 0;
    int animationTimer_ = 0;

    float x_ = 0.0f;
    float y_ = 0.0f;
    float speed_ = 0.0f;
    bool left = false;
    bool running_ = false;

    const int kLaneTop = 200;
    const int kLaneBottom = 550;

    bool isAttacking_ = false;
    int attackTimer_ = 0;
    const int attackDuration_ = 15;
    int attackPhase_ = 0;
    int comboTimer_ = 0;
    const int comboWindow_ = 20;

    bool isDashing_ = false;
    int dashTimer_ = 0;
    const int dashDuration_ = 20;
    const int dashCooldown_ = 120;
    int dashCooldownTimer_ = 0;
    float dashSpeed_ = 15.0f;
    float dashDirX_ = 0.0f;
    float dashDirY_ = 0.0f;

    std::vector<AfterImage> afterImages_;
    int afterImageInterval_ = 3;
    int afterImageTimer_ = 0;

    bool isUltimateActive_ = false;
    int ultimateTimer_ = 0;
    const int ultimateDuration_ = 220;
    int ultimateCooldown_ = 0;
    const int ultimateCooldownMax_ = 220;
    int ultimateTexture_ = 0;
    float ultimateAlpha_ = 0.0f;
    bool ultimateHitApplied_ = false;

    PlayerStats stats_;

    struct Ray {
        float x = 0.0f;
        float y = 0.0f;
        float dx = 0.0f;
        float dy = 0.0f;
        float speed = 0.0f;
    };
    std::vector<Ray> rays_;
    std::vector<SlashRay> slashRays_;
};
