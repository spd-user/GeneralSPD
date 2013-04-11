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


void ConsoleOutput::output(const spd::topology::Moore& topology,
	spd::core::Space& space) {
	// たらい回し
	outputLattice(topology, space);
}
void ConsoleOutput::output(const spd::topology::Neumann& topology,
	spd::core::Space& space) {
	// たらい回し
	outputLattice(topology, space);
}
void ConsoleOutput::output(const spd::topology::Hexagon& topology,
	spd::core::Space& space) {
	// たらい回し
	outputLattice(topology, space);
}

/*
 * 格子空間への処理
 */
void ConsoleOutput::outputLattice(const spd::topology::Lattice& lattice,
	spd::core::Space& space) {

	auto side = lattice.getSide();

	auto allPlayers = space.getPlayers();

	for (int y = 0; y < side; ++y) {
		for (int x = 0; x < side; ++x) {

			std::shared_ptr<spd::core::Player> player = allPlayers.at(y*side + x);
			printPlayer(player);
		}
		std::cout << "\n";
	}

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

	std::cerr << "Could not output on a console without a lattice.\n";
}

} /* namespace output */
} /* namespace spd */
