/**
 * SimpleActionRule.cpp
 *
 * @date 2012/12/05
 * @author katsumata
 */


#include "SimpleActionRule.hpp"

#include <stdexcept>
#include <random>

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RandomParameter.hpp"
#include "../../topology/Topology.hpp"

namespace spd {
namespace rule {

void SimpleActionRule::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {
	return;
}

/*
 * 行動更新
 *
 * Dの数に従い、決定的に行動を決定する。
 * 戦略の長さと近傍数が異なる場合、伸縮させる。
 */
void SimpleActionRule::runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) {

	// 近傍の設定
	auto phase = NeighborhoodType::ACTION;
	auto neighbors = player->getNeighbors(phase);
	if (neighbors == nullptr) {
		neighbors = std::move(param.getNeighborhoodParameter()->getTopology()->getNeighbors(
				allPlayers,
				player->getId(),
				param.getNeighborhoodParameter()->getNeiborhoodRadius(phase)));
	}

	int dMax = 0;
	int playersDNum = countDNum(player, neighbors, &dMax);

	// dの最大値+1 と、戦略の長さが異なる場合は調整
	if ((dMax + 1) != player->getStrategy()->getLength()) {
		playersDNum = adjustToStrategyLength(playersDNum, dMax, player->getStrategy(), param);
	}

	// 行動を設定
	player->setAction(player->getStrategy()->actionAt(playersDNum));
}

/*
 * Dの数
 */
inline int SimpleActionRule::countDNum(
		const std::shared_ptr<Player>& player,
		const Neighbors& neighbors,
		int* dMax) {

	int dNum = 0;
	*dMax = 0;

	// 自分は数えない
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {

		// 同じ近傍距離のプレイヤ数
		auto nNeighbors = neighbors->at(r);

		// 近傍距離に応じた近傍プレイヤ数を増やす
		*dMax += nNeighbors->size();

		for (auto& opponentWP : *(nNeighbors)) {
			auto neighbor = opponentWP.lock();
			if (neighbor == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			// Dの数をカウント
			if (neighbor->getPreAction() == Action::ACTION_D) {
				++dNum;
			} else if (neighbor->getPreAction() == Action::ACTION_UN) {
				// 未定義の行動があった場合終了
				throw std::runtime_error("The neighbor's action is undefined.");
			}
		}
	}

	return dNum;
}

/*
 * Dの数をプレイヤの戦略の長さに調整する
 */
inline int SimpleActionRule::adjustToStrategyLength(
		int dNum,
		int dMax,
		const std::shared_ptr<spd::core::Strategy>& playerStrategy,
		const spd::param::Parameter& param) {

	double normalRandom;

	auto engine = param.getRandomParameter()->getEngine();

	// 正規乱数の生成
	// 中央値(dNum + 0.5), 標準偏差(0.25) とする
	// 二項分布の0未満、試行回数以上ならば、やりなおす。
	std::normal_distribution<double> normalDistribution (0.5 + dNum, 0.25);
	do {
		normalRandom = normalDistribution(engine);
		param.getRandomParameter()->addGenerated(1);
	} while ((normalRandom < dNum) || (normalRandom >= dNum + 1));

	int result = static_cast<int>(normalRandom * playerStrategy->getLength() / (dMax + 1));

	return result;
}


} /* namespace rule */
} /* namespace spd */
