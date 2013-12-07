/**
 * NeumannCube.cpp
 *
 * @date 2013/12/07
 * @author katsumata
 */
#include "NeumannCube.hpp"

#include "Cube.hpp"

#include "../../core/OriginalType.hpp"
#include "../../core/Player.hpp"

namespace spd {
namespace topology {
namespace cube {

/*
 * 立方体中の指定プレイヤの隣接を設定する
 * @param[in] x プレイヤのx座標
 * @param[in] y プレイヤのy座標
 * @param[in] z プレイヤのz座標
 * @param[in] allPlayers すべてのプレイヤ
 * @param[in] cube 呼び出し元の立体クラス
 */
void NeumannCube::connect(int x0, int y0, int z0,
		const spd::core::AllPlayer& allPlayers,
		const spd::topology::Cube& cube) const {

	int i = cube.xyz2i(x0, y0, z0);

	auto& player = allPlayers.at(i);

	player->linkTo(allPlayers.at(cube.getTarget(i,  0,  0, -1)));
	player->linkTo(allPlayers.at(cube.getTarget(i,  0, -1,  0)));
	player->linkTo(allPlayers.at(cube.getTarget(i, -1,  0,  0)));
	player->linkTo(allPlayers.at(cube.getTarget(i,  1,  0,  0)));
	player->linkTo(allPlayers.at(cube.getTarget(i,  0,  1,  0)));
	player->linkTo(allPlayers.at(cube.getTarget(i,  0,  0,  1)));
}

/**
 * 立方体の指定プレイヤの近傍となるプレイヤの取得
 * @param[in] x プレイヤのx座標
 * @param[in] y プレイヤのy座標
 * @param[in] z プレイヤのz座標
 * @param[in] allPlayers すべてのプレイヤ
 * @param[in] radius 近傍半径
 * @param[in, out] result 指定プレイヤの近傍
 * @param[in] cube 呼び出し元の立体クラス
 */
void NeumannCube::getNeighbors(
		int x0, int y0, int z0,
		const spd::core::AllPlayer& allPlayers,
		int radius,
		spd::core::Neighbors& result,
		const spd::topology::Cube& cube) const {


	int i = cube.xyz2i(x0, y0, z0);


	for (int z = -1 * radius; z <= radius; ++z) {
		for (int y = -1 * radius; y <= radius; ++y) {
			for (int x = -1 * radius; x <= radius; ++x) {

				int distance = std::abs(z) + std::abs(y) + std::abs(x);
				if (distance <= radius) {
					int nSite = cube.getTarget(i, x, y, z);
					result->at(distance)->push_back(allPlayers.at(nSite));
				}
			}
		}
	}
}


} /* namespace cube */
} /* namespace topology */
} /* namespace spd */
