/**
 * Hexagon.cpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#include "Hexagon.hpp"

#include <cmath> // sqrt と absのため
#include <algorithm> // max
#include <iostream>
#include <string>

#include "../core/OriginalType.hpp"
#include "../core/Player.hpp"
#include "../core/Space.hpp"

#include "../param/Parameter.hpp"
#include "../param/InitParameter.hpp"
#include "../param/NeighborhoodParameter.hpp"

#include "../output/OutputVisitor.hpp"

namespace spd {
namespace topology {

using spd::core::Player;


Hexagon::Hexagon() {
	this->side = 0;
}


/*
 * すべてのプレイヤに隣接接続を設定する
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Hexagon::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	auto iniParam = param.getInitialParameter();

	// 適合した格子かどうか
	std::string errMsg;
	if (!isCompliant(iniParam, errMsg)) {
		std::cerr << errMsg << std::endl;
		exit(EXIT_FAILURE);
	}

	// プレイヤ数
	auto playerNum = iniParam->getPlayerNum();

	// 格子サイズ
	int side = static_cast<int>(std::sqrt(playerNum));

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// 接続近傍の設定
	// 1プレイヤの接続近接に関わるメモリ量
	auto connectionSize = sizeof(std::weak_ptr<Player>) * ADJACENCE;

	availableMemory = availableMemory - static_cast<long int>(connectionSize * playerNum);
	if (availableMemory < 0) {
		// 接続近傍が設定できないなら、終了
		std::cerr << "This program could not construct a spatial structure due to insufficient memory.\n"
				<< "Please input a sufficient available memory size or run on other machines.\n\n"
				<< "To simulate this setting, add more than "
				<< std::abs(availableMemory) << " byte(s) of memory.\n";
		std::exit(EXIT_FAILURE);
	}
	iniParam->setMemory(availableMemory);

	// 空間構造に従い、接続近傍の設定を行う
	for (int i = 0; i < playerNum; ++i) {
		auto player = players.at(i);

		// 奇数行目(index は偶数) ならば、上下のxは -1, 0 になる
		// 偶数行目(index は奇数) ならば、上下のxは 0, +1 になる
		int xBias = ((i / side) % 2) == 0 ? 0 : 1;

		// 上側
		player->linkTo(players.at(((i % side) - 1 + xBias +side)%side+((i / side) - 1 +side)%side*side));
		player->linkTo(players.at(((i % side)     + xBias +side)%side+((i / side) - 1 +side)%side*side));
		// 左右
		player->linkTo(players.at(((i % side) - 1 +        side)%side+((i / side)     +side)%side*side));
		player->linkTo(players.at(((i % side) + 1 +        side)%side+((i / side)     +side)%side*side));
		// 下側
		player->linkTo(players.at(((i % side) - 1 + xBias +side)%side+((i / side) + 1 +side)%side*side));
		player->linkTo(players.at(((i % side)     + xBias +side)%side+((i / side) + 1 +side)%side*side));
	}
}

/*
 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
 * @param[in] players すべてのプレイヤ
 * @param[in] target 対象プレイヤの空間位置
 * @param[in] radius 近傍半径
 * @throw std::invalid_argument 近傍半径が0未満の場合
 */
spd::core::Neighbors Hexagon::getNeighbors(const spd::core::AllPlayer& players,
		int target,
		int radius) {

	if (radius < 0) {
		throw std::invalid_argument("Could not set a minus radius for this topology.\nYou set "
				+ std::to_string(radius) + ".");
	}

	auto result = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();
	result->reserve(radius + 1);

	for (int r = 0; r <= radius; ++r) {
		std::shared_ptr<std::vector<std::weak_ptr<Player>>> rVec (new std::vector<std::weak_ptr<Player>>());
		result->push_back(rVec);
	}

	int x0 = target % side;
	int y0 = target / side;

	for (int y = -1 * radius; y <= radius; ++y) {

		int len = radius * 2 - std::abs(y) + 1;

		int startX = 0 - static_cast<int> (len / 2);
		if (((y0 + y) % 2 == 0) &&
				(y0 % 2 == 1) && y != 0) {
			++startX;
		}
		int endX = startX + len - 1;

		for (int x = startX; x <= endX; ++x) {

			int xBias = 0;
			if (y0 % 2 != 0) {
				xBias = (y % 2 == 0)? std::abs(x) + std::abs(y / 2) : std::abs(std::round(x - 0.5)) + std::abs(y / 2);
			} else {
				xBias = (y % 2 == 0)? std::abs(x) + std::abs(y / 2) : std::abs(std::round(x + 0.5)) + std::abs(y / 2);
			}

			int nSite = (x0 + x +side)%side+(y0 + y +side)%side*side;

			result->at(std::max(std::abs(y), std::abs(xBias)))->push_back(players.at(nSite));
		}
	}

	return result;
}

/**
 * 空間において必須の戦略の長さを求める
 * @param actionRadius 行動更新での近傍の長さ
 */
int Hexagon::minStrategyLength(int actionRadius) {
	return calcNeighborsNum(actionRadius);
}

/*
 * 空間において最大の戦略の長さを求める
 * @param actionRadius 行動更新での近傍の長さ
 * @return 最大の戦略長
 * @retval 0 できるかぎり使う場合
 */
int Hexagon::maxStrategyLength(int actionRadius) {
	return calcNeighborsNum(actionRadius);
}

/*
 * 再設定はしない
 */
void Hexagon::reSetting(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {
	return;
}

/*
 * 空間条件が適合しているか調べる
 * @param[in] iniParam パラメタ
 * @param[out] エラーメッセージ
 * @return 各格子に適切なパラメタかどうか
 * @retval true 適切に扱える格子の場合
 * @retval false 適切に扱うことができない格子の場合、エラーメッセージは第2引数から取得
 */
bool Hexagon::isCompliant(const std::shared_ptr<spd::param::InitParameter>& iniParam, std::string& errMsg) {

	// 結果
	bool result = true;

	// 辺の長さ
	auto side = static_cast<int>(std::sqrt(iniParam->getPlayerNum()));
	if (side * side != iniParam->getPlayerNum()) {
		// 辺の長さがプレイヤ数の平方根でなければ終了
		errMsg = "Please input a number that is a square value of integer,";
		errMsg += " when you simulate on a Hexagon.";
		result = false;
	}

	// 適応した空間条件なら、辺の長さを設定する
	this->side = side;
	return result;
}

/*
 * 指定した近傍半径の自身を含めたプレイヤ数を計算する
 * @param radius 近傍半径
 * @return 自身を含めた近傍プレイヤ数
 */
inline int Hexagon::calcNeighborsNum(int radius) {
	return (3 * radius * (1 + radius)) + 1;
}

/*
 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
 *
 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
 * @param[in] neighborType 近傍タイプ
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Hexagon::setNeighborsOf(
		NeighborhoodType neighborType,
		const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	// 近傍パラメタ
	auto& neighborParam = param.getNeighborhoodParameter();
	// 初期値パラメタ
	auto& initParam = param.getInitialParameter();

	// プレイヤ数
	auto playerNum = initParam->getPlayerNum();

	// 設定する近傍半径
	auto radius = neighborParam->getNeiborhoodRadius(neighborType);
	// 0未満は、nullptr埋め
	if (radius < 0) {
		for (std::shared_ptr<Player> player : players) {
			player->getNeighbors(neighborType) = nullptr;
		}
		return;
	}

	// 使用可能メモリ
	auto availableMemory = initParam->getMemory();

	// 接続近傍の設定
	// 1プレイヤの接続近接に関わるメモリ量
	auto neighborsSize = sizeof(std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>)
							+ sizeof(std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>)
							+ sizeof(std::shared_ptr<std::vector<std::weak_ptr<Player>>>) * (radius + 1)
							+ sizeof(std::vector<std::weak_ptr<Player>>) * (radius + 1)
							+ sizeof(std::weak_ptr<Player>) * calcNeighborsNum(radius);

	// 何プレイヤに近傍を設定するか
	int setPlayerNum = static_cast<int>(availableMemory / neighborsSize);
	if (setPlayerNum > playerNum) {
		setPlayerNum = playerNum;
	}

	// 設定可能な近傍の登録
	for (int i = 0; i < setPlayerNum; ++i) {
		players.at(i)->getNeighbors(neighborType) =
				getNeighbors(players,
						i,
						neighborParam->getNeiborhoodRadius(neighborType));
	}
	// メモリ不足により設定できない部分は null 埋める
	for (int i = setPlayerNum; i < playerNum; ++i) {
		players.at(i)->getNeighbors(neighborType) = nullptr;
	}

	// 使ったメモリを引いとく
	initParam->setMemory(availableMemory - (setPlayerNum * neighborsSize));
}

/*
 * 文字出力
 */
std::string Hexagon::toString() const {
	return "Hexagon";
}


} /* namespace topology */
} /* namespace spd */
