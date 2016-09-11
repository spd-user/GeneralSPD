/**
 * InverseSquareDiscountDistance.cpp
 *
 * @date 2016/9/11
 * @author katsumata
 */

#include <stdexcept>
#include "InverseSquareDiscountDistance.hpp"

#include "../../core/Player.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RuntimeParameter.hpp"

#include "../../topology/Topology.hpp"

namespace spd {
namespace rule {

void InverseSquareDiscountDistance::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {
	return;
}

/*
 * 距離の二乗に比例する割引率（獲得利得が逆二乗）の対戦を行う
 */
void InverseSquareDiscountDistance::runRule(
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

	int neighborsCount = 0;

	// 自身の利得行を取得
	auto payoffRow = param.getRuntimeParameter()->getPayoffRow(player->getAction());

	int startRadius = 0;
	// 自己対戦がないなら、半径1から
	if (!param.getRuntimeParameter()->isSelfInteraction()) {
		startRadius++;
	}
	// 近傍対戦
	for (int r = startRadius, rMax = neighbors->size(); r < rMax; ++r) {

		// 割引率
		double discoutRatio = 1.0 / ((r + static_cast<double>(1.0)) * (r + static_cast<double>(1.0)));

		for (auto& opponentWP : *(neighbors->at(r))) {

			auto opponent = opponentWP.lock();

			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			// 割引加算
			payoffSum += payoffRow[static_cast<int>(opponent->getAction())] * discoutRatio;
		}
	}

	// 利得を加える
	player->addScore(payoffSum);
}


} /* namespace rule */
} /* namespace spd */
