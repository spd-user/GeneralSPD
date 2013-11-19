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

	// プレイヤ数-1 より少ないエッジでは、接続グラフを作れないので修了
	if (generateEdge < allPlayerNum - 1) {
		std::cerr << "Could not generate a connected graph by a probability " << this->connectionProbability
				<< "\nPlease set larger probability." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// 生成する乱数の数(最低数はエッジの数 * 2)
	unsigned long long genRnd = 0;

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


	// ノードの接続を管理する配列
	// 中央のグラフに接続されているノード
	std::vector<int> connectedNodes;
	connectedNodes.reserve(allPlayerNum);
	// 中央グラフに入っていないノード
	std::vector<int> unconnectedNodes;
	unconnectedNodes.reserve(allPlayerNum - 1);

	// あるノードを選択し、接続グラフへ入れる
	int firstNode = dist(engine);
	genRnd++;
	connectedNodes.push_back(firstNode);
	// 残りを未接続にする
	for (int i = 0; i < allPlayerNum; ++i) {
		if (i != firstNode) {
			unconnectedNodes.push_back(i);
		}
	}


	// 最初に必ず一つのランダムグラフを作る
	while (unconnectedNodes.size() != 0) {

		std::uniform_int_distribution<> connectDist(0, connectedNodes.size() - 1);
		std::uniform_int_distribution<> unconnectDist(0, unconnectedNodes.size() - 1);

		int src = connectDist(engine);
		int dest = unconnectDist(engine);

		int srcPlayer = connectedNodes.at(src);
		int destPlayer = unconnectedNodes.at(dest);

		players.at(srcPlayer)->linkTo(players.at(destPlayer));
		players.at(destPlayer)->linkTo(players.at(srcPlayer));

		// 接続したのを接続済みへ設定し、未接続から削除
		connectedNodes.push_back(unconnectedNodes.at(dest));
		unconnectedNodes.erase(std::begin(connectedNodes) + dest);
	}

	// 接続エッジ数
	// この時点、プレイヤ数-1 のエッジが作成済み
	unsigned long long edge = allPlayerNum - 1;

	genRnd += 2 * (allPlayerNum - 1);


	// あとは適当に接続
	while(edge < generateEdge) {

		int src = dist(engine);
		int dest = dist(engine);
		genRnd += 2;

		if (players.at(src)->linkTo(players.at(dest))) {
			players.at(dest)->linkTo(players.at(src));
			++edge;

			if (edge > displayTiming) {
				std::cout << "Stage 1/1: connect " << displayTiming / displayUnit * persent << "% finish" << "   \r" << std::flush;
				displayTiming += displayUnit;
			}
		}
	}

	randParam->addGenerated(genRnd);
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

	// 生成する乱数の数
	unsigned long long genRnd = 0;

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
	std::cout << "Stage 1/2: Generating a perfect graph\r" << std::flush;
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
		genRnd += 2;

		// 削除したときに、未連結グラフにならないようにする
		if ((players.at(src)->getLinkedPlayers()->size() <= 1)
				|| (players.at(dest)->getLinkedPlayers()->size() <= 1)) {

			if (players.at(src)->deleteLinkTo(players.at(dest))) {
				players.at(dest)->deleteLinkTo(players.at(src));
				++edge;

				if (edge > displayTiming) {
					std::cout << "Stage 2/2: disconnect " << displayTiming / displayUnit * persent << "% finish" << "   \r" << std::flush;
					displayTiming += displayUnit;
				}
				continue;
			}
		}

	}

	randParam->addGenerated(genRnd);
}



} /* namespace topology */
} /* namespace spd */
