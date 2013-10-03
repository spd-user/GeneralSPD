/**
 * Player.cpp
 *
 * @date 2012/08/29
 * @author katsumata
 */

#include "Player.hpp"

#include <stdexcept>

namespace spd {
namespace core {

/*
 * コンストラクタ
 * @param id シミュレーション空間におけるプレイヤID
 */
Player::Player(int playerID) : id(playerID),
		preAction(Action::ACTION_UN), action(Action::ACTION_UN),
		preScore(0), score(0),
		preStrategy(nullptr), strategy(nullptr), linkedPlayers(nullptr) {

	for (int i = 0; i < NeighborhoodType::TYPE_NUM; ++i) {
		this->neighborPlayers[i] = nullptr;
	}
}

/*
 * 初期化を行う
 */
void Player::init(Action action, const std::shared_ptr<Strategy>& strategy) {

	this->action = action;
	this->preAction = action;

	this->score = 0.0;
	this->preScore = 0.0;

	this->strategy = strategy;
	this->preStrategy = this->strategy;
}

/*
 * 相手のプレイヤに一方的に接続
 */
bool Player::linkTo(const std::weak_ptr<Player>& player) {
	if (linkedPlayers == nullptr) {
		linkedPlayers = std::make_shared<std::vector<std::weak_ptr<Player>>>();
	}

	int opponentId = player.lock()->getId();

	// 相手先が自分の場合は、なにもしない
	if (opponentId == id) {
		return false;
	}

	for (auto& linkedPlayer : *(linkedPlayers)) {
		// すでに接続している場合は、なにもしない
		if (opponentId == linkedPlayer.lock()->getId()) {
			return false;
		}
	}

	linkedPlayers->push_back(player);
	return true;
}


} /* namespace core */
} /* namespace spd */
