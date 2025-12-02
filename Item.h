#pragma once
#include <Novice.h>

struct Vector2 {
    int x = 0;
    int y = 0;
};

class Player; // 前置声明

class Item {
public:
    void Initialize(int x, int y, int type); // 指定位置和类型
    void Update(Player* player = nullptr); // 可传入玩家指针检测碰撞
    void Draw(int scrollX);

    bool IsActive() const {
        return active_;
    }
    int GetType() const {
        return type_;
    }

private:
    Vector2 position_;  // 已默认初始化
    int type_ = 0;      // 道具类型 0~9
    bool active_ = true;
    int texture_ = 0;
    int width_ = 32;
    int height_ = 32;
};
