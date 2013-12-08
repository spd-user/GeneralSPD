/**
 * ConsoleOutput.cpp
 *
 * @date 2012/12/18
 * @author katsumata
 */

#include "ConsoleOutput.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

#include "../core/Space.hpp"
#include "../core/Player.hpp"
#include "../core/Strategy.hpp"
#include "../core/Converter.hpp"

#include "../topology/AllTopology.hpp"

namespace spd {
namespace output {

/*
 * 初期化で行うことはない
 */
void ConsoleOutput::init(spd::core::Space& space, spd::param::Parameter& param) {
	return;
}

/*
 * 格子空間への処理
 */
void ConsoleOutput::output(const spd::topology::Lattice& lattice, spd::core::Space& space) {

	auto side = lattice.getSide();

	auto allPlayers = space.getPlayers();

	std::cout << "step:" << space.getStep() << "\n";
	for (int y = 0; y < side; ++y) {
		for (int x = 0; x < side; ++x) {

			std::shared_ptr<spd::core::Player> player = allPlayers.at(y*side + x);
			printPlayer(player);
		}
		std::cout << "\n";
	}

}


/*
 * 立体格子に即した出力
 * @param[in] cube 空間構造
 * @param[in] space 空間状態
 */
void ConsoleOutput::output(const spd::topology::Cube& cube, spd::core::Space& space) {

	auto& allPlayers = space.getPlayers();
	int playerNum = allPlayers.size();
	auto side = std::cbrt(playerNum);
	side = (2.0 * side + playerNum / side / side) / 3.0; // ニュートン法の漸化式


	std::cout << "step:" << space.getStep() << "\n";

	for (int z = 0; z < side; ++z) {
		for (int y = 0; y < side; ++y) {
			for (int x = 0; x < side; ++x) {
				std::shared_ptr<spd::core::Player> player = allPlayers.at(z*side*side+y*side + x);
				printPlayer(player);
			}
			std::cout << "\n";
		}
		std::cout << "-------------------------------\n";
	}
	std::cout << std::endl;

}


/*
 * 1プレイヤを出力する
 * @param[in] player プレイヤ
 */
void ConsoleOutput::printPlayer(const std::shared_ptr<spd::core::Player> player) {

	auto playerAction = player->getAction();

#ifdef DEBUG
	std::cout << "<";
	try {
		std::cout << spd::core::converter::actionToChar(playerAction) <<
				"-" << spd::core::converter::actionToChar(player->getPreAction()) << ": ";
	} catch (std::invalid_argument& e) {
		std::cout << "\n[id: " << player->getId() << "]'s action is undefined.\n";
	}

	std::cout << player->getStrategy()->getShortStrategy() << "-" <<
			player->getPreStrategy()->getShortStrategy() << ":" <<
			std::setw(6) << player->getScore() << "-" <<
			std::setw(6) << player->getPreScore() << ">, ";
#else // Relese 環境
	if (playerAction == Action::ACTION_C) {
			std::cout << "_,";
		} else if (playerAction == Action::ACTION_D) {
			if (player->getStrategy()->isAll(Action::ACTION_D)) {
				std::cout << "■<";
			} else {
				std::cout << "□<";
			}
		}
#endif // DEBUG
}

/*
 * ネットワークでは出力しない
 */
void ConsoleOutput::output(const spd::topology::Network& topology,
	spd::core::Space& space) {

#ifdef DEBUG
	// Debug では詳しく全て表示
	auto& allPlayers = space.getPlayers();

	std::cout << "step:" << space.getStep() << "\n";
	for (auto& player : allPlayers) {
		try {
			// id の表示
			std::cout << "ID: " << player->getId() << " [";

			// 接続の表示
			auto& linkedPlayers = player->getLinkedPlayers();
			for (auto& linkedPlayer : *linkedPlayers) {
				std::cout << " " << linkedPlayer.lock()->getId() << ",";
			}

			// 状態の表示
			std::cout << "preAct(" << spd::core::converter::actionToChar(player->getPreAction())
			<< "), act(" << spd::core::converter::actionToChar(player->getAction())
			<< "), preSocre(" << player->getPreScore() << "), score(" << player->getScore()
			<< "), preStrategy(" << player->getPreStrategy()->getShortStrategy()
			<< "), strategy(" << player->getStrategy()->getShortStrategy()
			<< ")\n";

		} catch (std::invalid_argument& e) {
			std::cout << "\n[id: " << player->getId() << "]'s err. Maybe related action.\n";
		}
	}
	std::cout << "--------------------------------" << std::endl;
#else
	// Relese ではなにも表示しない
	std::cerr << "Could not output on a console without a lattice.\n";
#endif
}

} /* namespace output */
} /* namespace spd */
