/**
 * Ring.cpp
 *
 * @date 2014/01/31
 * @author katsumata
 */
#include "Ring.hpp"

#include <iostream>
#include <string>

#include "../../core/OriginalType.hpp"
#include "../../core/Player.hpp"
#include "../../core/Space.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"

namespace spd {
namespace topology {

using spd::core::Player;

/*
 * すべてのプレイヤの接続を作成する。
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Ring::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	auto iniParam = param.getInitialParameter();

	// プレイヤ数
	auto playerNum = iniParam->getPlayerNum();

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// 接続近傍の設定
	// 1プレイヤの接続近接に関わるメモリ量
	auto connectionSize = sizeof(std::weak_ptr<Player>) * ADJACENCE;

		availableMemory = availableMemory - static_cast<long int>(connectionSize * playerNum);
		if (availableMemory < 0) {
			// 接続近傍が設定できないなら、終了
			std::cerr << "This program could not construct a spatial structure due to insufficient memory.\n"
					<< "Please input a sufficient available memory size or run on other machines.\n\n"
					<< "To simulate this setting, add more than "
					<< std::abs(availableMemory) << " byte(s) of memory.\n";
			std::exit(EXIT_FAILURE);
		}
		iniParam->setMemory(availableMemory);

		// 空間構造に従い、接続近傍の設定を行う
		for (int i = 0; i < playerNum; ++i) {
			auto player = players.at(i);

			player->linkTo(players.at((i - 1 +playerNum)%playerNum));
			player->linkTo(players.at((i + 1 +playerNum)%playerNum));
		}
}

/*
 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
 * @param[in] players すべてのプレイヤ
 * @param[in] target 対象プレイヤの空間位置
 * @param[in] radius 近傍半径
 */
spd::core::Neighbors Ring::getNeighbors(
			const spd::core::AllPlayer& players,
			int target,
			int radius) {

	auto allPlayerNum = players.size();

	if (radius < 0) {
		throw std::invalid_argument("Could not set a minus radius for this topology.\nYou set "
				+ std::to_string(radius) + ".");
	}

	auto result = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();
	result->reserve(radius + 1);

	for (int r = 0; r <= radius; ++r) {
		std::shared_ptr<std::vector<std::weak_ptr<Player>>> rVec (new std::vector<std::weak_ptr<Player>>());
		result->push_back(rVec);
	}

	for (int x = -1 * radius; x <= radius; ++x) {

		int nSite = (target + x +allPlayerNum)%allPlayerNum;

		result->at(std::abs(x))->push_back(players.at(nSite));
	}

	return result;


}

} /* namespace topology */
} /* namespace spd */
