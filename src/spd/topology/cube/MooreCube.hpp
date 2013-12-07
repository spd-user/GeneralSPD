/**
 * MooreCube.hpp
 *
 * @date 2013/12/07
 * @author katsumata
 */
#ifndef MOORECUBE_HPP_
#define MOORECUBE_HPP_

#include "CubeNeighborType.hpp"

#include <cmath>

namespace spd {
namespace topology {
namespace cube {

/**
 * 立方体のムーアを表すクラス
 */
class MooreCube : public spd::topology::cube::CubeNeighborType {
public:
	/**
	 * 近傍数を計算する
	 * @param[in] radius 近傍数(デフォルト値は1)
	 */
	int calcNeigorsNum(int radius = 1) const {
		return std::pow((2 * radius + 1), 3) - 1;

	};

	/**
	 * 立方体中の指定プレイヤの隣接を設定する
	 * @param[in] x0 プレイヤのx座標
	 * @param[in] y0 プレイヤのy座標
	 * @param[in] z0 プレイヤのz座標
	 * @param[in] allPlayers すべてのプレイヤ
	 * @param[in] cube 呼び出し元の立体クラス
	 */
	void connect(
			int x0, int y0, int z0,
			const spd::core::AllPlayer& allPlayers,
			const spd::topology::Cube& cube) const;

	/**
	 * 立方体の指定プレイヤの近傍となるプレイヤの取得
	 * @param[in] x0 プレイヤのx座標
	 * @param[in] y0 プレイヤのy座標
	 * @param[in] z0 プレイヤのz座標
	 * @param[in] allPlayers すべてのプレイヤ
	 * @param[in] radius 近傍半径
	 * @param[in, out] result 指定プレイヤの近傍
	 * @param[in] cube 呼び出し元の立体クラス
	 */
	void getNeighbors(
			int x0, int y0, int z0,
			const spd::core::AllPlayer& allPlayers,
			int radius,
			spd::core::Neighbors& result,
			const spd::topology::Cube& cube) const;

	/**
	 * クラス情報の文字出力
	 * @return クラス情報
	 */
	std::string toString() const {
		return "moore";
	};
};

} /* namespace cube */
} /* namespace topology */
} /* namespace spd */

#endif /* MOORECUBE_HPP_ */
