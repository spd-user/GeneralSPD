/**
 * CubeNeighborType.hpp
 *
 * @date 2013/12/07
 * @author katsumata
 */
#ifndef CUBENEIGHBORTYPE_HPP_
#define CUBENEIGHBORTYPE_HPP_

#include "../../core/OriginalType.hpp"
#include "../../IToString.hpp"

namespace spd {
namespace core {
class Player;
}
namespace topology {
class Cube;
namespace cube {


/**
 * 立方体の近傍タイプによる動作を表すクラス
 */
class CubeNeighborType : public spd::IToString {

public:


	/**
	 * デストラクタ
	 */
	virtual ~CubeNeighborType(){};

	/**
	 * 近傍数を計算する
	 * @param[in] radius 近傍数(デフォルト値は1)
	 */
	virtual int calcNeigorsNum(int radius = 1) const = 0;

	/**
	 * 立方体中の指定プレイヤの隣接を設定する
	 * @param[in] x0 プレイヤのx座標
	 * @param[in] y0 プレイヤのy座標
	 * @param[in] z0 プレイヤのz座標
	 * @param[in] allPlayers すべてのプレイヤ
	 * @param[in] cube 呼び出し元の立体クラス
	 */
	virtual void connect(
			int x0, int y0, int z0,
			const spd::core::AllPlayer& allPlayers,
			const spd::topology::Cube& cube) const = 0;

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
	virtual void getNeighbors(
			int x0, int y0, int z0,
			const spd::core::AllPlayer& allPlayers,
			int radius,
			spd::core::Neighbors& result,
			const spd::topology::Cube& cube) const = 0;

};

} /* namespace cube */
} /* namespace topology */
} /* namespace spd */

#endif /* CUBENEIGHBORTYPE_HPP_ */
