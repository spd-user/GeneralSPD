/**
 * SimpleSumGameRule.cpp
 *
 * @date 2012/12/12
 * @author katsumata
 */

#include <stdexcept>
#include "SimpleSumGameRule.hpp"

#include "../../core/Player.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RuntimeParameter.hpp"

#include "../../topology/Topology.hpp"

namespace spd {
namespace rule {

void SimpleSumGameRule::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {
	return;
}

/*
 * 総和対戦を行う
 */
void SimpleSumGameRule::runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) {

	// 近傍の設定
	auto phase = NeighborhoodType::GAME;
	auto neighbors = player->getNeighbors(phase);
	if (neighbors == nullptr) {
		neighbors = std::move(param.getNeighborhoodParameter()->getTopology()->getNeighbors(
				allPlayers,
				player->getId(),
				param.getNeighborhoodParameter()->getNeiborhoodRadius(phase)));
	}

	double payoffSum = 0.0;

	// 自身の利得行を取得
	auto payoffRow = param.getRuntimeParameter()->getPayoffRow(player->getAction());

	int startRadius = 0;
	// 自己対戦がないなら、半径1から
	if (!param.getRuntimeParameter()->isSelfInteraction()) {
		startRadius++;
	}
	// 近傍対戦
	for (int r = startRadius, rMax = neighbors->size(); r < rMax; ++r) {
		for (auto& opponentWP : *(neighbors->at(r))) {

			auto opponent = opponentWP.lock();

			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			payoffSum += payoffRow[static_cast<int>(opponent->getAction())];
		}
	}

	// 利得を加える
	player->addScore(payoffSum);
}


} /* namespace rule */
} /* namespace spd */
