/**
 * Random.cpp
 *
 * @date 2013/09/05
 * @author katsumata
 */

#include "Random.hpp"

#include <random>
#include <stdexcept>

#include <thread>

#include "../core/OriginalType.hpp"
#include "../core/Player.hpp"
#include "../core/Space.hpp"

#include "../param/Parameter.hpp"
#include "../param/InitParameter.hpp"
#include "../param/NeighborhoodParameter.hpp"
#include "../param/RandomParameter.hpp"

#include "../output/OutputVisitor.hpp"

#include <iostream>

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

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// 0以上1未満の一様分布
	std::uniform_real_distribution<> dist(0.0, 1.0);

	auto randParam = param.getRandomParameter();

	int core = param.getCore();
	std::vector<std::thread> thr(core + 1);

	// 全プレイヤ数
	int allPlayerNum = players.size();
	// 半分のプレイヤ数(総プレイヤ数が奇数の場合は半分に満たない数になる)
	int halfPlayerNum = allPlayerNum / 2;
	//(allPlayerNum % 2 == 0) ? allPlayerNum / 2 : (allPlayerNum + 1) / 2;
	// 1コアが担当するプレイヤ数
	int breadth = halfPlayerNum / core;

	// プレイヤの設定状況の進捗
	int playerProgress = 0;

	for (int i = 0, size = thr.size(); i < size; ++i) {
		thr[i] = std::thread(
				[&, i]{

			// 乱数生成器の作成
			std::mt19937_64 engine (randParam->getSeed());
			engine.discard(randParam->getDiscardNum());

			// 中央以外の計算部分
			if (i != core) {

				int from = breadth * i;
				int to = (i + 1 < core) ? breadth * (i + 1) : halfPlayerNum;

				// 乱数の調整
				unsigned long long discardNum =
						(((allPlayerNum - 1) + (allPlayerNum - from)) * from) / 2;
				engine.discard(discardNum);

				// 前半部分
				for (int id = from; id < to; ++id) {
					for (int destPlayer = id + 1; destPlayer < allPlayerNum; ++destPlayer) {

						// 接続
						if (dist(engine) < this->connectionProbability) {
							// メモリの計算
							availableMemory -= static_cast<long int>(sizeof(std::weak_ptr<Player>));
							if (availableMemory < 0) {
								std::runtime_error("Could not construct a spatial structure due to insufficient memory.");
							}

							players.at(id)->linkTo(players.at(destPlayer));
							players.at(destPlayer)->linkTo(players.at(id));
						}

					}
				}

				playerProgress += to - from;
				int progress = (playerProgress * 100) / allPlayerNum;
				std::string msg = "connetion ";
				msg += std::to_string(progress) + "% finished (thr " + std::to_string(i) + ")";
				std::cout << msg << std::endl;

				discardNum = ((allPlayerNum - 1) * ((halfPlayerNum - to) * 2 + (allPlayerNum % 2))) / 2;
				engine.discard(discardNum);
				// 後半部分
				for (int id = allPlayerNum - to; id < allPlayerNum - from; ++id) {
					for (int destPlayer = id + 1; destPlayer < allPlayerNum; ++destPlayer) {

						// 接続
						if (dist(engine) < this->connectionProbability) {
							// メモリの計算
							availableMemory -= static_cast<long int>(sizeof(std::weak_ptr<Player>));
							if (availableMemory < 0) {
								std::runtime_error("Could not construct a spatial structure due to insufficient memory.");
							}

							players.at(id)->linkTo(players.at(destPlayer));
							players.at(destPlayer)->linkTo(players.at(id));
						}
					}
				}
			} else {
				// 最後のスレッドは真ん中だけ専用
				if (allPlayerNum % 2 == 1) {
					// プレイヤ数が奇数であるとき、最後のスレッドで真ん中の分を行う
					int centerPlayer = (allPlayerNum - 1) / 2;

					// 乱数の調整
					unsigned long long discardNum =
							(((allPlayerNum - 1) + (allPlayerNum - centerPlayer)) * centerPlayer) / 2;
					engine.discard(discardNum);

					for (int destPlayer = centerPlayer + 1; destPlayer < allPlayerNum; ++destPlayer) {

						// 接続
						if (dist(engine) < this->connectionProbability) {
							// メモリの計算
							availableMemory -= static_cast<long int>(sizeof(std::weak_ptr<Player>));
							if (availableMemory < 0) {
								std::runtime_error("Could not construct a spatial structure due to insufficient memory.");
							}

							players.at(centerPlayer)->linkTo(players.at(destPlayer));
							players.at(destPlayer)->linkTo(players.at(centerPlayer));
						}

					}
				}
			}
		}
		);
	}

	// ネットワークの構築完了
	for (std::thread& t : thr) {
		t.join();
	}

	// 生成に必要な乱数の数
	unsigned long long genRand = ((allPlayerNum * allPlayerNum) - allPlayerNum) / 2;

	randParam->addGenerated(genRand);
	iniParam->setMemory(availableMemory);
}

///**
// * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
// * @param[in] players すべてのプレイヤ
// * @param[in] target 対象プレイヤの空間位置
// * @param[in] radius 近傍半径
// * @throw std::invalid_argument 近傍半径が0未満の場合
// */
//spd::core::Neighbors getNeighbors(
//		const spd::core::AllPlayer& players,
//		int target,
//		int radius) {
//
//	if (radius < 0) {
//		throw std::invalid_argument("Could not set a minus radius for this topology.\nYou set "
//				+ std::to_string(radius) + ".");
//	}
//
//	// 自分だけをいれた、初期の近傍リストを作成
//	auto result = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();
//	result->reserve(radius + 1);
//	for (int r = 0; r <= radius; ++r) {
//		std::shared_ptr<std::vector<std::weak_ptr<Player>>> rVec (new std::vector<std::weak_ptr<Player>>());
//		result->push_back(rVec);
//	}
//	result->at(0)->push_back(players.at(target));
//
//	// 幅優先で探索
//	for (int previousR = 0; previousR < radius; ++previousR) {
//
//		int r = previousR + 1;
//
//		if () {
//
//		}
//
//
//	}
//}
//
///**
// * 対象のプレイヤが近傍内にあるかidを使って調べる
// * @param[in] neighbors 入っているか調べられる近傍
// * @param[in] id 対象のプレイヤID
// * @return すでにあるかどうか
// * @retval true すでにある場合
// * @retval false まだない場合
// */
//bool existOnNeighbors(spd::core::Neighbors& neighbors, int id) {
//
//	for (int r = 0, rMax = neighbors->size(); r < rMax; ++r) {
//
//		for (auto& opponentWP : *(neighbors->at(r))) {
//			auto neighbor = opponentWP.lock();
//			if (neighbor == nullptr) {
//				// 近傍がいない場合終了
//				throw std::runtime_error("Could not find a neighbor of a player.");
//			}
//
//			// 既にある場合
//			if (neighbor->getId() == id) {
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//
///*
// * 接続と近傍の再設定
// * @param[in] players すべてのプレイヤ
// * @param[in] param パラメタ
// */
//void Random::reSetting(const spd::core::AllPlayer& players,
//		const spd::param::Parameter& param) {
//
//	// リンクの削除
//	for (auto& player : players) {
//		player->resetLink();
//	}
//
//	// 接続
//	connectPlayers(players, param);
//}
//
///*
// * 空間において必須の戦略の長さを求める
// * @param[in] actionRadius 行動更新での近傍距離
// * @return 最低限はないので 0
// */
//int Random::minStrategyLength(int actionRadius) {
//	return 0;
//}
//
///*
// * 空間において最大の戦略の長さを求める
// * @param[in] actionRadius 行動更新での近傍の長さ
// * @return 最長はないので0
// */
//int Random::maxStrategyLength(int actionRadius) {
//	return 0;
//}

/*
 * ランダム用の出力を行う
 * @param[in] visitor 構造に対する出力の処理方法
 * @param[in] space 空間
 */
// TODO ?
//void accept(spd::output::OutputVisitor& visitor, spd::core::Space& space);

//
//
//
///**
// * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
// *
// * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
// * @param[in] neighborType 近傍タイプ
// * @param[in] players すべてのプレイヤ
// * @param[in] param パラメタ
// */
//void setNeighborsOf(
//		NeighborhoodType neighborType,
//		const spd::core::AllPlayer& players,
//		const spd::param::Parameter& param);

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
