/**
 * Topology.cpp
 *
 * @date 2012/10/03
 * @author katsumata
 */

#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>

#include "Topology.hpp"

#include "../core/Player.hpp"

#include "../param/Parameter.hpp"
#include "../param/NeighborhoodParameter.hpp"

using spd::core::Player;

namespace spd {
namespace topology {

/**
 * デストラクタ
 */
Topology::~Topology() {};

/*
 * すべてのプレイヤに近傍を設定する
 *
 * メモリ容量が足りない場合は設定されない。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Topology::setNeighbors(
		const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	auto& neiParam = param.getNeighborhoodParameter();


	// 行動近傍
	std::cout << "setting action neighbors." << std::endl;
	if (neiParam->getNeiborhoodRadius(NeighborhoodType::ACTION) == 1) {
		// 近傍半径が 1 なら、接続近傍を設定
		copyConnectors(NeighborhoodType::ACTION, players);
	} else {
		this->setNeighborsOf(NeighborhoodType::ACTION, players, param);
	}

	// 対戦近傍
	std::cout << "setting game neighbors." << std::endl;
	if (neiParam->getNeiborhoodRadius(NeighborhoodType::GAME) ==
			neiParam->getNeiborhoodRadius(NeighborhoodType::ACTION)) {
		this->copyNeighbors(NeighborhoodType::ACTION, NeighborhoodType::GAME, players);
	} else if (neiParam->getNeiborhoodRadius(NeighborhoodType::GAME) == 1) {
		copyConnectors(NeighborhoodType::GAME, players);
	} else {
		this->setNeighborsOf(NeighborhoodType::ACTION, players, param);
	}

	// 戦略更新近傍
	std::cout << "setting strategy neighbors." << std::endl;
	if (neiParam->getNeiborhoodRadius(NeighborhoodType::STRATEGY) ==
				neiParam->getNeiborhoodRadius(NeighborhoodType::ACTION)) {
			this->copyNeighbors(NeighborhoodType::ACTION, NeighborhoodType::STRATEGY, players);
	} else if (neiParam->getNeiborhoodRadius(NeighborhoodType::STRATEGY) ==
			neiParam->getNeiborhoodRadius(NeighborhoodType::GAME)) {
		this->copyNeighbors(NeighborhoodType::GAME, NeighborhoodType::STRATEGY, players);
	} else if (neiParam->getNeiborhoodRadius(NeighborhoodType::STRATEGY) == 1) {
		copyConnectors(NeighborhoodType::STRATEGY, players);
	} else {
		this->setNeighborsOf(NeighborhoodType::STRATEGY, players, param);
	}

	std::cout << "finish setting neighbors." << std::endl;

}

/*
 * プレイヤの近傍をコピーする
 * @param[in] sourceType コピー元
 * @param[in] destType コピー先
 * @param[in] players すべてのプレイヤ
 */
void Topology::copyNeighbors(
			NeighborhoodType sourceType,
			NeighborhoodType destType,
			const spd::core::AllPlayer& players) const {

	// 同じなら何もしない
	if (sourceType == destType) {
		return;
	}

	for (std::shared_ptr<Player> player : players) {
		player->getNeighbors(destType) = player->getNeighbors(sourceType);
	}
}

/*
 * プレイヤの接続近傍をコピーする
 * @param[in] destType コピー先
 * @param[in] players すべてのプレイヤ
 */
void Topology::copyConnectors(NeighborhoodType destType, const spd::core::AllPlayer& players) const {

	for (std::shared_ptr<Player> player : players) {

		auto& neighbors = player->getNeighbors(destType);
		neighbors = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();

		auto own = std::make_shared<std::vector<std::weak_ptr<Player>>>();
		own->push_back(player);

		auto playersLink = player->getLinkedPlayers();
		if (playersLink == nullptr) {
			// null の場合は自分とのリンクを作ろうと試みることで、null でないようにする
			player->linkTo(player);
			playersLink = player->getLinkedPlayers();
		}

		neighbors->push_back(own);
		neighbors->push_back(playersLink);
	}
}

} /* namespace topology */
} /* namespace spd */
