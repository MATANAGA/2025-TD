#include "Map.h"
#include <cstdlib>

void Map::Initialize() {
    tileImage_ = Novice::LoadTexture("white1x1.png");

    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            mapData_[y][x] = (y == kMapHeight - 1) ? 1 : 0;
        }
    }

    
}

void Map::Update() {
    for (auto& item : items_) {
        item.Update();
    }
}

void Map::Draw(int scrollX) {
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            if (mapData_[y][x] == 1) {
                Novice::DrawSprite(
                    x * kTileSize - scrollX,
                    y * kTileSize,
                    tileImage_,
                    1.0f, 1.0f, 0.0f, WHITE
                );
            }
        }
    }

    for (auto& item : items_) {
        item.Draw(scrollX);
    }
}
