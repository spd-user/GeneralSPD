/**
 * Cube.cpp
 *
 * @date 2013/12/03
 * @author katsumata
 */
#include "Cube.hpp"

#include <cmath> // 三乗根(cbrt)
#include <stdexcept>

#include "CubeNeighborType.hpp"
#include "MooreCube.hpp"
#include "NeumannCube.hpp"

#include "../../core/OriginalType.hpp"
#include "../../core/Player.hpp"
#include "../../core/Space.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"

#include "../../output/OutputVisitor.hpp"

namespace spd {
namespace topology {

using spd::core::Player;

Cube::Cube() : cubeNeighbor(new cube::MooreCube()), plateNum(0), sideNum(0) {}


/*
 * すべてのプレイヤの接続を作成する。
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Cube::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {

	int playerNum = players.size();

	// 確認
	int side = static_cast<int>(std::cbrt(playerNum));
	if (side * side * side != playerNum) {
		std::string errMsg = "";
		errMsg += "Please input a number that is able to compose a cube.";
		errMsg += "Ex. " + std::to_string(static_cast<int>(std::pow(side - 1, 3))) +
				", " + std::to_string(static_cast<int>(std::pow(side, 3))) + ", or " +
				std::to_string(static_cast<int>(std::pow(side + 1, 3))) + ".";
		throw std::runtime_error(errMsg);
	}
	auto iniParam = param.getInitialParameter();

	this->sideNum = side;
	this->plateNum = side * side;

	// 使用可能メモリ
	auto availableMemory = iniParam->getMemory();

	// 接続近傍の設定
	// 1プレイヤの接続近接に関わるメモリ量
	auto connectionSize = sizeof(std::weak_ptr<Player>) * cubeNeighbor->calcNeigorsNum();

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

	// 接続
	for (int i = 0; i < playerNum; ++i) {
		int z = i / plateNum;
		int y = (i - z * plateNum) / sideNum;
		int x = (i - z * plateNum) % sideNum;

		cubeNeighbor->connect(x, y, z, players, *this);
	}
}

/*
 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
 * @param[in] players すべてのプレイヤ
 * @param[in] target 対象プレイヤの空間位置
 * @param[in] radius 近傍半径
 * @throw std::invalid_argument 近傍半径が0未満の場合
 */
spd::core::Neighbors Cube::getNeighbors(
		const spd::core::AllPlayer& players,
		int target,
		int radius) {

	if (radius < 0) {
		throw std::invalid_argument("Could not set a minus radius for getting neighbors.\nYou set "
				+ std::to_string(radius) + ".");
	}

	auto result = std::make_shared<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>();
	result->reserve(radius + 1);

	for (int r = 0; r <= radius; ++r) {
		std::shared_ptr<std::vector<std::weak_ptr<Player>>> rVec (new std::vector<std::weak_ptr<Player>>());
		result->push_back(rVec);
	}

	int z = target / plateNum;
	int y = (target - z * plateNum) / sideNum;
	int x = (target - z * plateNum) % sideNum;

	cubeNeighbor->getNeighbors(x, y, z, players, radius, result, *this);

	return result;
}

/*
 * 空間において必須の戦略の長さを求める
 * @param[in] actionRadius 行動更新での近傍距離
 * @return 最低限必要な戦略長
 * @retval 近傍数+1
 */
int Cube::minStrategyLength(int actionRadius) {
	return cubeNeighbor->calcNeigorsNum(actionRadius) + 1;
}

/*
 * 空間において最大の戦略の長さを求める
 * @param[in] actionRadius 行動更新での近傍の長さ
 * @return 最大の戦略長
 * @retval 近傍数+1
 */
int Cube::maxStrategyLength(int actionRadius) {
	return cubeNeighbor->calcNeigorsNum(actionRadius) + 1;
}

/*
 * 立方体用の出力を行う
 * @param[in] visitor 構造に対する出力の処理方法
 * @param[in] space 空間
 */
void Cube::accept(spd::output::OutputVisitor& visitor, spd::core::Space& space) {
	visitor.output(*this, space);
}

/*
 * トポロジのプロパティを設定する
 * @param[in] properties プロパティ
 */
void Cube::setProp(std::vector<std::string> properties) {

	if (properties.size() < 1) {
		return;
	}

	std::string prop = properties[0];
	std::transform(prop.begin(), prop.end(), prop.begin(), ::tolower);


	if (prop == "moore") {
		this->cubeNeighbor.reset(new cube::MooreCube());
	} else if (prop == "neumann") {
		this->cubeNeighbor.reset(new cube::NeumannCube());
	}
}


/*
 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
 *
 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
 * @param[in] neighborType 近傍タイプ
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Cube::setNeighborsOf(
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

	// 近傍の設定
	// 1プレイヤの接続近接に関わるメモリ量
	auto neighborsSize = sizeof(std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>>)
							+ sizeof(std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>)
							+ sizeof(std::shared_ptr<std::vector<std::weak_ptr<Player>>>) * (radius + 1)
							+ sizeof(std::vector<std::weak_ptr<Player>>) * (radius + 1)
							+ sizeof(std::weak_ptr<Player>) * (cubeNeighbor->calcNeigorsNum(radius) + 1);

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



/**
 * 空間構図構造名の出力
 * @return 空間構図構造名(Moore)
 */
std::string Cube::toString() const {
	return "cube:" + cubeNeighbor->toString();
};



} /* namespace topology */
} /* namespace spd */
