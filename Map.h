#include "Item.h"
#include <vector>

class Map {
public:
    void Initialize();
    void Update();
    void Draw(int scrollX);

private:
    static const int kMapWidth = 50;
    static const int kMapHeight = 15;
    static const int kTileSize = 32;

    int mapData_[kMapHeight][kMapWidth];
    int tileImage_ = 0;

    std::vector<Item> items_;
};
