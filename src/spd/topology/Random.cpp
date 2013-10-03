/**
 * Random.cpp
 *
 * @date 2013/09/05
 * @author katsumata
 */

#include "Random.hpp"

#include <random>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include "../core/OriginalType.hpp"
#include "../core/Player.hpp"
#include "../core/Space.hpp"

#include "../param/Parameter.hpp"
#include "../param/InitParameter.hpp"
#include "../param/NeighborhoodParameter.hpp"
#include "../param/RandomParameter.hpp"

#include "../output/OutputVisitor.hpp"

namespace spd {
namespace topology {

using spd::core::Player;

/**
 * 接続確率を設定するコンストラクタ
 */
Random::Random() : connectionProbability(DEFAULT_CONNECTION_PROBABILITY) {};

/*
 * すべてのプレイヤの接続を作成する。
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Random::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// プレイヤ数
	int allPlayerNum = players.size();

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// プレイヤ数の一様分布
	std::uniform_int_distribution<> dist(0, allPlayerNum - 1);

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

	// 総当たりでの最大エッジ数
	unsigned long long maxEdge = ((allPlayerNum * allPlayerNum) - allPlayerNum) / 2;

	// 生成するエッジ数
	unsigned long long generateEdge = maxEdge * this->connectionProbability;

	// 生成する乱数の数(最低数はエッジの数 * 2)
	unsigned long long genRnd = generateEdge * 2;

	// 接続したら表示する単位(5%ごと)
	auto persent = 5;
	auto displayUnit = generateEdge / (100 / persent);
	auto displayTiming = displayUnit;

	// メモリの確認
	long int requiredMemory = sizeof(std::weak_ptr<Player>) * generateEdge;
	availableMemory -= requiredMemory;
	if (availableMemory < 0) {
		std::cerr << "This program could not construct a spatial structure due to insufficient memory.\n"
				<< "Please input a sufficient available memory size or run on other machines.\n\n"
				<< "To simulate this setting, add more than "
				<< std::abs(availableMemory) << " byte(s) of memory.\n";
		std::exit(EXIT_FAILURE);
	}
	iniParam->setMemory(availableMemory);

	unsigned long long edge = 0;
	while(edge < generateEdge) {

		int src = dist(engine);
		int dest = dist(engine);

		if (players.at(src)->linkTo(players.at(dest))) {
			players.at(dest)->linkTo(players.at(src));
			++edge;

			if (edge > displayTiming) {
				std::cout << "connect: " << displayTiming / displayUnit * persent << "% finish" << "   \r" << std::flush;
				displayTiming += displayUnit;
			}
		} else {
			// 接続出来ないときは、今の分の乱数が増える
			genRnd += 2;
		}
	}
}

/*
 * トポロジのプロパティ(接続確率)を設定する
 * @param[in] properties プロパティ
 */
void Random::setProp(std::vector<double> properties) {

	if (properties.size() < 1) {
		return;
	}

	this->connectionProbability = properties[0];
};

} /* namespace topology */
} /* namespace spd */
