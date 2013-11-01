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

/*
 * コンストラクタ
 */
Random::Random() : connectionProbability(DEFAULT_CONNECTION_PROBABILITY),
		usedMemory(0) {};

/*
 * すべてのプレイヤの接続を作成する。
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Random::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// 前回使ったメモリを戻しておく
	// 初回時は 0
	auto memory = param.getInitialParameter()->getMemory() + usedMemory;
	param.getInitialParameter()->setMemory(memory);

	if (this->connectionProbability > DIVIDE_POINT) {
		// 減少
		decrementCreate(players, param);
	} else {
		// 増加
		incrementCreate(players, param);
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

	double cp = properties[0];
	if (cp < 0) {
		cp = 0;
	} else if (cp > 1) {
		cp = 1;
	}
	this->connectionProbability = cp;
};

/*
 * ノードの追加型接続生成
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Random::incrementCreate(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// プレイヤ数
	int allPlayerNum = players.size();

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	long long int availableMemory = iniParam->getMemory();

	// 総当たりでの最大エッジ数
	unsigned long long maxEdge = (
			static_cast<unsigned long long>(allPlayerNum) *
			(static_cast<unsigned long long>(allPlayerNum) - 1)
				) / 2;

	// 生成するエッジ数
	unsigned long long generateEdge = maxEdge * (this->connectionProbability * 100) / 100;

	// 生成する乱数の数(最低数はエッジの数 * 2)
	unsigned long long genRnd = generateEdge * 2;

	// 接続したら表示する単位(5%ごと)
	auto persent = 5;
	auto displayUnit = generateEdge / (100 / persent);
	auto displayTiming = displayUnit;

	// メモリの確認(双方向分)
	unsigned long int requiredMemory = sizeof(std::weak_ptr<Player>) * generateEdge * 2;


	availableMemory -= requiredMemory;

	if (availableMemory < 0) {

		std::cerr << "maxEdge : " << maxEdge << "\nconnection probability : " << this->connectionProbability <<
				"\ngenerateEdge : " <<  generateEdge << "\nrequired memory : " << requiredMemory << std::endl;


		std::cerr << "\nThis program could not construct a spatial structure due to insufficient memory.\n"
				<< "Please input a sufficient available memory size or run on other machines.\n\n"
				<< "To simulate this setting, add more than "
				<< std::abs(availableMemory) << " byte(s) of memory.\n";
		std::exit(EXIT_FAILURE);
	}
	iniParam->setMemory(availableMemory);
	// 使用メモリの記憶
	this->usedMemory = requiredMemory;

	// プレイヤ数の一様分布
	std::uniform_int_distribution<> dist(0, allPlayerNum - 1);

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

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
 * ノードの減少生成型接続生成
 *
 * メモリ容量が足りない場合、追加型生成に切り替える
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Random::decrementCreate(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// プレイヤ数
	int allPlayerNum = players.size();

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	unsigned long long int availableMemory = iniParam->getMemory();

	// 総当たりでの最大エッジ数
	unsigned long long maxEdge = (
			static_cast<unsigned long long>(allPlayerNum) *
			(static_cast<unsigned long long>(allPlayerNum) - 1)
	) / 2;

	// 最大のメモリ(双方向分)
	unsigned long long int maxMemory = sizeof(std::weak_ptr<Player>) * maxEdge * 2;

	// 減少生成出来ないので、増加作成を行う
	if (maxMemory > availableMemory) {
		std::cerr << "The network is structured by incremental generation, "
				"due to insufficient memory. It might take a long time to construction." << std::endl;
		incrementCreate(players, param);
	}

	// 削除するエッジ数
	unsigned long long deleteEdge = maxEdge * (100 - (this->connectionProbability * 100)) / 100;

	// 生成する乱数の数(最低数はエッジの数 * 2)
	unsigned long long genRnd = deleteEdge * 2;

	// 削除したら表示する単位(5%ごと)
	auto persent = 5;
	auto displayUnit = deleteEdge / (100 / persent);
	auto displayTiming = displayUnit;

	// 使用するメモリ(双方向分)
	unsigned long long int requiredMemory = sizeof(std::weak_ptr<Player>) * (maxEdge - deleteEdge) * 2;

	availableMemory -= requiredMemory;


	iniParam->setMemory(availableMemory);
	// 使用メモリの記憶
	this->usedMemory = requiredMemory;

	// プレイヤ数の一様分布
	std::uniform_int_distribution<> dist(0, allPlayerNum - 1);

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

	// 完全グラフの生成
	for (int i = 0; i < allPlayerNum - 1; ++i) {
		for (int j = i + 1; j < allPlayerNum; ++j) {
			// 接続
			players.at(i)->linkTo(players.at(j));
			players.at(j)->linkTo(players.at(i));
		}
	}

	unsigned long long edge = 0;
	while(edge < deleteEdge) {

		int src = dist(engine);
		int dest = dist(engine);

		if (players.at(src)->deleteLinkTo(players.at(dest))) {
			players.at(dest)->deleteLinkTo(players.at(src));
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



} /* namespace topology */
} /* namespace spd */
