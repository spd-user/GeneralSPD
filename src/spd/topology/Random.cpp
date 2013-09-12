/**
 * Random.cpp
 *
 * @date 2013/09/05
 * @author katsumata
 */

#include "Random.hpp"

#include <random>
#include <stdexcept>

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

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// プレイヤ数
	auto playerNum = iniParam->getPlayerNum();

	// 0以上1未満の一様分布
	std::uniform_real_distribution<> dist(0.0, 1.0);

	auto randParam = param.getRandomParameter();
	auto engine = randParam->getEngine();

	// 全ての組み合わせをみる
	for (int i = 0; i < playerNum - 1; ++i) {
		for (int j = i + 1; j < playerNum; ++j) {

			// 接続
			if (dist(engine) < this->connectionProbability) {
				// メモリの計算
				availableMemory -= static_cast<long int>(sizeof(std::weak_ptr<Player>));
				if (availableMemory < 0) {
					std::runtime_error("Could not construct a spatial structure due to insufficient memory.");
				}

				players.at(i)->linkTo(players.at(j));
				players.at(j)->linkTo(players.at(i));
			}
			randParam->addGenerated(1);
		}
	}

	iniParam->setMemory(availableMemory);
}
//
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
		throw std::runtime_error("The propery is nothing");
	}

	this->connectionProbability = properties[0];
};

} /* namespace topology */
} /* namespace spd */
