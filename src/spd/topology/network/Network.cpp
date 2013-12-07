/**
 * Network.cpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#include "Network.hpp"

#include <stdexcept>

#include "../../core/OriginalType.hpp"
#include "../../core/Player.hpp"
#include "../../core/Space.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"

#include "../../output/OutputVisitor.hpp"


namespace spd {
namespace topology {

using spd::core::Player;

/*
 * グラフ用の出力を行う
 *
 * @param[in] visitor 構造に対する出力の処理方法
 * @param[in] space 空間
 */
void Network::accept(spd::output::OutputVisitor& visitor, spd::core::Space& space) {

	visitor.output((*this), space);
}


/*
 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
 * @param[in] players すべてのプレイヤ
 * @param[in] target 対象プレイヤの空間位置
 * @param[in] radius 近傍半径
 * @throw std::invalid_argument 近傍半径が0未満の場合
 */
spd::core::Neighbors Network::getNeighbors(
		const spd::core::AllPlayer& players,
		int target,
		int radius) {

	if (radius < 0) {
		throw std::invalid_argument("Could not set a minus radius for this topology.\nYou set "
				+ std::to_string(radius) + ".");
	}

	// 自分だけをいれた、初期の近傍リストを作成
	auto result = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();
	result->reserve(radius + 1);
	for (int r = 0; r <= radius; ++r) {
		std::shared_ptr<std::vector<std::weak_ptr<Player>>> rVec (new std::vector<std::weak_ptr<Player>>());
		result->push_back(rVec);
	}
	result->at(0)->push_back(players.at(target));

	// 幅優先で探索
	for (int previousR = 0; previousR < radius; ++previousR) {

		int r = previousR + 1;

		for (auto& opponentWP : *(result->at(previousR))) {

			auto opponent = opponentWP.lock();

			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			// 対戦者のリンクをたどる
			auto linkedPlayer = opponent->getLinkedPlayers();
			for (auto opponentLink : *(linkedPlayer)) {
				auto opponentLinkId = opponentLink.lock()->getId();
				// 今までにはない場合、追加
				if (!existOnNeighbors(result, opponentLinkId)) {
					result->at(r)->push_back(players.at(opponentLinkId));
				}
			}
		}
	}

	return result;
}

/*
 * 対象のプレイヤが近傍内にあるかidを使って調べる
 * @param[in] neighbors 入っているか調べられる近傍
 * @param[in] id 対象のプレイヤID
 * @return すでにあるかどうか
 * @retval true すでにある場合
 * @retval false まだない場合
 */
bool Network::existOnNeighbors(spd::core::Neighbors& neighbors, int id) {

	for (int r = 0, rMax = neighbors->size(); r < rMax; ++r) {

		for (auto& opponentWP : *(neighbors->at(r))) {
			auto neighbor = opponentWP.lock();
			if (neighbor == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			// 既にある場合
			if (neighbor->getId() == id) {
				return true;
			}
		}
	}
	return false;
}


/*
 * 接続と近傍の再設定
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Network::reSetting(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// リンクの削除
	for (auto& player : players) {
		player->resetLink();
	}

	// 接続
	connectPlayers(players, param);
}

/*
 * 空間において必須の戦略の長さを求める
 * @param[in] actionRadius 行動更新での近傍距離
 * @return 最低限はないので 0
 */
int Network::minStrategyLength(int actionRadius) {
	return 0;
}

/*
 * 空間において最大の戦略の長さを求める
 * @param[in] actionRadius 行動更新での近傍の長さ
 * @return 最長はないので0
 */
int Network::maxStrategyLength(int actionRadius) {
	return 0;
}


/*
 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
 *
 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
 * @param[in] neighborType 近傍タイプ
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Network::setNeighborsOf(
		NeighborhoodType neighborType,
		const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// 近傍パラメタ
	auto& neighborParam = param.getNeighborhoodParameter();

	// 設定する近傍半径
	auto radius = neighborParam->getNeiborhoodRadius(neighborType);
	// 0未満は、nullptr埋め
	if (radius < 0) {
		for (std::shared_ptr<Player> player : players) {
			player->getNeighbors(neighborType) = nullptr;
		}
		return;
	}

	// 初期値パラメタ
	auto& initParam = param.getInitialParameter();

	// プレイヤ数
	auto playerNum = initParam->getPlayerNum();

	// 使用可能メモリ
	auto availableMemory = initParam->getMemory();


	// 1プレイヤの接続近接に関わるメモリ量の基礎値
	// (ここでは、近傍数によらない数を算出)
	// プレイヤ数が決まったら、 weak_ptr * <近傍数> を加算
	auto neighborsBaseMemory = sizeof(std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>)
							+ sizeof(std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>)
							+ sizeof(std::shared_ptr<std::vector<std::weak_ptr<Player>>>) * (radius + 1)
							+ sizeof(std::vector<std::weak_ptr<Player>>) * (radius + 1);

	for (int i = 0; i < playerNum; ++i) {
		// 次数
		int degree = 0;

		// 近傍半径による近傍を取得
		auto neighbors = getNeighbors(players, i, radius);

		degree = countNeighbors(neighbors);

		// このプレイヤが使う近傍メモリ量を確定
		auto playerMemory = neighborsBaseMemory + sizeof(std::weak_ptr<Player>) * degree;

		if (availableMemory - playerMemory > 0) {
			// 残るなら設定する
			players.at(i)->getNeighbors(neighborType) = neighbors;
			availableMemory -= playerMemory;
		} else {
			// 残らないなら null
			players.at(i)->getNeighbors(neighborType) = nullptr;
		}
	}
	initParam->setMemory(availableMemory);
}

/*
 * 近傍数を数える
 * @param[in] neighbors
 * @return 近傍数
 */
int Network::countNeighbors(spd::core::Neighbors& neighbors) {

	int result = 0;

	for (int r = 0, rMax = neighbors->size(); r < rMax; ++r) {

		for (auto& opponentWP : *(neighbors->at(r))) {
			auto neighbor = opponentWP.lock();
			if (neighbor == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player.");
			}

			++result;
		}
	}
	return result;
}

} /* namespace topology */
} /* namespace spd */
