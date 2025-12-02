#include <Novice.h>
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include <vector>

const char kWindowTitle[] = "";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player;
	player.Initialize();

	Map map;
	map.Initialize();

	// 道具列表
	std::vector<Item> items;
	for (int i = 0; i < 10; i++) {
		Item item;
		item.Initialize(100 + i * 100, 300, i);
		items.push_back(item);
	}

	int scrollX = 0;

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// -------------------
		// 更新
		// -------------------
		player.Update(keys);

		// 道具更新（传入玩家，用于碰撞检测）
		for (auto& item : items) {
			item.Update(&player);
		}

		// -------------------
		// 绘制
		// -------------------
		// 背景卷轴
		scrollX = (int)player.GetX() - 640;
		if (scrollX < 0) scrollX = 0;

		Novice::DrawBox(0, 300, 1280, 250, 0.0f, WHITE, kFillModeSolid);

		map.Draw(scrollX);

		// 绘制道具
		for (auto& item : items) {
			item.Draw(scrollX);
		}

		// 绘制玩家
		player.Draw(scrollX);


		PlayerStats& s = player.GetStats();

		Novice::ScreenPrintf(20, 20, "HP: %d / %d", s.hp, s.maxHP);
		Novice::ScreenPrintf(20, 40, "ATK: %d", s.attack);
		Novice::ScreenPrintf(20, 60, "DEF: %.1f", s.defense);
		Novice::ScreenPrintf(20, 80, "SPD: %.1f", s.moveSpeed);


		// （如果你有能量 技能冷却 也能显示）
		Novice::ScreenPrintf(20, 100, "スキルCD: %d / %d",
			s.skillCooldownTimer, s.skillCooldown);


		Novice::EndFrame();

		// 退出
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
