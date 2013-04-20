/**
 * DefaultStrategyRule.cpp
 *
 * @date 2012/12/12
 * @author katsumata
 */

#include "BestStrategyRule.hpp"

#include <stdexcept>

#include "../../core/Strategy.hpp"
#include "../../core/Player.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/RuntimeParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"

#include "../../topology/Topology.hpp"

namespace spd {
namespace rule {

void BestStrategyRule::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {
	return;
}

/*
 * 決定的最大値行動更新ルール
 */
void BestStrategyRule::runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) {

	// 戦略更新周期でなければ、何もしない
	if (step % param.getRuntimeParameter()->getStrategyUpdateCycle() != 0) {
		return;
	}

	// 近傍の設定
	auto phase = NeighborhoodType::STRATEGY;
	auto neighbors = player->getNeighbors(phase);
	if (neighbors == nullptr) {
		neighbors = std::move(param.getNeighborhoodParameter()->getTopology()->getNeighbors(
				allPlayers,
				player->getId(),
				param.getNeighborhoodParameter()->getNeiborhoodRadius(phase)));
	}

	// 最大値は自身ので初期化
	double maxScore = player->getPreScore();
	// 最大戦略のID
	int maxStrategyId = player->getPreStrategy()->getId();

	// 自身と比べる必要はないので1から。
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {
		for (auto& opponentWP : *(neighbors->at(r))) {

			auto opponent = opponentWP.lock();

			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			if (maxScore < opponent->getPreScore()) {
				// 対戦相手の戦略が高い場合、その戦略を記憶
				maxStrategyId = opponent->getPreStrategy()->getId();
				maxScore = opponent->getPreScore();
			} else if ((maxScore == opponent->getPreScore())
					&& (player->getPreStrategy()->getId() == opponent->getPreStrategy()->getId())) {
				// 利得が同じなら、戦略を維持する
				maxStrategyId = opponent->getPreStrategy()->getId();
			}
		}
	}

	// 最大の戦略を設定
	player->setStrategy(param.getStrategyList().at(maxStrategyId).first);

	// 利得を0にする
	player->setScore(0.0);
}


} /* namespace rule */
} /* namespace spd */
