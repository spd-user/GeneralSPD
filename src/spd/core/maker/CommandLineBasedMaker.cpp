/**
 * CommandLineBasedMaker.cpp
 *
 * @date 2013/01/12
 * @author katsumata
 */

#include "CommandLineBasedMaker.hpp"

#include <cmath>
#include <stdexcept>
#include <iostream>

#include "../Player.hpp"
#include "../Space.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RandomParameter.hpp"

#include "../../topology/Topology.hpp"
#include "../../rule/SpdRule.hpp"

namespace spd {
namespace core {

/*
 * 初期化で使用するパラメタを指定して初期化
 */
CommandLineBasedMaker::CommandLineBasedMaker(spd::param::Parameter& param) : parameter(param) {}

/*
 * プレイヤを作成し、接続と近傍の設定を行う
 */
AllPlayer CommandLineBasedMaker::makePlayers() {

	// プレイヤの作成

	AllPlayer allPlayer = std::vector<std::shared_ptr<Player>>();
	auto playerNum = parameter.getInitialParameter()->getPlayerNum();

	// プレイヤの作成
	auto playersMemory = sizeof(std::vector<std::shared_ptr<Player>>) +
			(sizeof(std::shared_ptr<Player>) + sizeof(Player)) * playerNum;

	auto availableMemory = parameter.getInitialParameter()->getMemory() - static_cast<long int>(playersMemory);
	if (availableMemory < 0) {
		// メモリが少なく、プレイヤを作れない場合
		std::cerr << "This program could not construct players due to insufficient memory.\n"
				"Please input a sufficient available memory size or run on other machines.\n\n"
				"At least more than " << std::abs(availableMemory) <<
				" byte(s) of memory is needed for only construct players.\n";
		std::exit(EXIT_FAILURE);
	}
	parameter.getInitialParameter()->setMemory(availableMemory);

	allPlayer.reserve(playerNum);
	for (int id = 0; id < playerNum; ++id) {
		allPlayer.push_back(std::make_shared<Player>(id));
	}

	// 接続設定
	auto topology = this->parameter.getNeighborhoodParameter()->getTopology();

	std::cout << "connecting players." << std::endl;
	topology->connectPlayers(allPlayer, this->parameter);
	std::cout << "set neighbor players." << std::endl;
	topology->setNeighbors(allPlayer, this->parameter);

	return allPlayer;
}

/*
 * プレイヤの初期化を行う
 */
void CommandLineBasedMaker::initPlayer(const AllPlayer& players, spd::core::Space& space) {

	auto strategyList = parameter.getStrategyList();

	for (int i = 0, playerNum = players.size(); i < playerNum; ++i) {
		auto player = players.at(i);

		int strategyNumber = getStrategyNumber();
		if (strategyNumber == -1) {
			throw std::runtime_error("The strategy number corresponding to random number was not found.");
		}

		Action initAction;
		if (parameter.getInitialParameter()->startsInitialFixedAction()) {
			initAction = parameter.getInitialParameter()->getFixedAction();
		} else {
			// 乱数の生成
			std::uniform_int_distribution<int> distribution(0, 1);
			if (distribution(parameter.getRandomParameter()->getEngine()) == 0) {
				initAction = Action::ACTION_C;
			} else {
				initAction = Action::ACTION_D;
			}
			parameter.getRandomParameter()->addGenerated(1);
		}

		// 初期化
		player->init(initAction, strategyList.at(strategyNumber).first);
	}

	// クラスタがある場合の初期化
	if (parameter.getInitialParameter()->startsCluster()) {
		int side = std::sqrt(parameter.getInitialParameter()->getPlayerNum());

		// 中心座標
		int centerIndex = std::floor(side / 2) * side + std::floor(side / 2);

		std::shared_ptr<spd::topology::Topology> topology = this->parameter.getNeighborhoodParameter()->getTopology();
		auto clusters = topology->getNeighbors(players, centerIndex, parameter.getInitialParameter()->getStartClusterSize());

		for (int r = 0, rMax = clusters->size(); r < rMax; ++r) {
			for (int i = 0, rPlayerNum = clusters->at(r)->size(); i < rPlayerNum; ++i) {
				auto opponent = (clusters->at(r)->at(i)).lock();

				if (opponent == nullptr) {
					// 近傍がいない場合終了
					throw std::runtime_error("Could not get the cluster players.");
				}
				// 初めの戦略で初期化
				opponent->init(opponent->getAction(), strategyList.at(0).first);
			}
		}
	}

	// ルールによるプロパティの初期化
	parameter.getInitialParameter()->getSpdRule()->init(players, this->parameter);
}

int CommandLineBasedMaker::getStrategyNumber() {

	// 乱数の最大値
	int randMax = 0;

	// 戦略表
	auto strategyList = parameter.getStrategyList();

	int startIndex = 0;

	for (auto strategy : strategyList) {
		randMax += strategy.second;
	}
	// 初期クラスタがあるときは、最初の戦略を考えない
	if (parameter.getInitialParameter()->startsCluster()) {
		startIndex++;
		randMax -= strategyList.front().second;
	}

	// 乱数の生成
	std::uniform_int_distribution<int> distribution(0, randMax - 1);
	auto key = distribution(parameter.getRandomParameter()->getEngine());
	parameter.getRandomParameter()->addGenerated(1);

	int level = 0;
	for (int i = startIndex, size = strategyList.size(); i < size; ++i) {

		level += strategyList.at(i).second;
		if (key < level) {
			return i;
		}
	}

	return -1;
}


/*
 * 構造と近傍の再初期化を行う
 */
void CommandLineBasedMaker::resetTopology(AllPlayer& players) {
	std::shared_ptr<spd::topology::Topology> topology = this->parameter.getNeighborhoodParameter()->getTopology();

	topology->reSetting(players, this->parameter);
}


} /* namespace core */
} /* namespace spd */
