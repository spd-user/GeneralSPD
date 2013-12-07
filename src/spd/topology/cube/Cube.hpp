/**
 * Cube.hpp
 *
 * @date 2013/12/03
 * @author katsumata
 */
#ifndef CUBE_HPP_
#define CUBE_HPP_

#include "../Topology.hpp"
#include "CubeNeighborType.hpp"

namespace spd {
namespace topology {

/**
 * 立方体を表すクラス
 */
class Cube : public spd::topology::Topology {

public:

	/**
	 * コンストラクタ
	 */
	Cube();

	/**
	 * すべてのプレイヤの接続を作成する。
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void connectPlayers(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
	 * @param[in] players すべてのプレイヤ
	 * @param[in] target 対象プレイヤの空間位置
	 * @param[in] radius 近傍半径
	 * @throw std::invalid_argument 近傍半径が0未満の場合
	 */
	spd::core::Neighbors getNeighbors(
			const spd::core::AllPlayer& players,
			int target,
			int radius);

	/**
	 * 接続と近傍の再設定
	 *
	 * @note なにもしない
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void reSetting(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param) {};

	/**
	 * 空間において必須の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍距離
	 * @return 最低限必要な戦略長
	 * @retval 近傍数+1
	 */
	int minStrategyLength(int actionRadius);

	/**
	 * 空間において最大の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍の長さ
	 * @return 最大の戦略長
	 * @retval 近傍数+1
	 */
	int maxStrategyLength(int actionRadius);

	/**
	 * 立方体用の出力を行う
	 * @param[in] visitor 構造に対する出力の処理方法
	 * @param[in] space 空間
	 */
	void accept(spd::output::OutputVisitor& visitor, spd::core::Space& space);

	/**
	 * トポロジのプロパティを設定する
	 * @note 一つ目のみ有効。また、"moore", "neumann" でなければ無効
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<std::string> properties);

	/**
	 * 対象プレイヤに対する、x, y, zの相対値から該当するプレイヤ位置座標を取得する
	 * @param[in] i ベースのプレイヤ位置座標
	 * @param[in] x xの相対値
	 * @param[in] y yの相対値
	 * @param[in] z zの相対値
	 */
	int getTarget(int i, int x, int y, int z) const {
		int z0 = i / plateNum;
		int y0 = (i - z0 * plateNum) / sideNum;
		int x0 = (i - z0 * plateNum) % sideNum;

		return ((z0 + z +sideNum) % sideNum) * plateNum +
				((y0 + y +sideNum) % sideNum) * sideNum +
				((x0 + x +sideNum) % sideNum);
	};

	/**
	 * x, y, z座標を一意のプレイヤ位置座標へ変換する
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標

	 */
	int xyz2i(int x, int y, int z) const {
		return z * plateNum + y * sideNum + x;
	}

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名
	 */
	std::string toString() const;

private:

	/**
	 * 立方体で考慮する近傍
	 */
	std::unique_ptr<cube::CubeNeighborType> cubeNeighbor;

	/**
	 * 一層のプレイヤ数
	 */
	int plateNum;

	/**
	 * 一辺のプレイヤ数
	 */
	int sideNum;

	/**
	 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
	 *
	 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
	 * @param[in] neighborType 近傍タイプ
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void setNeighborsOf(
			NeighborhoodType neighborType,
			const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

};

} /* namespace topology */
} /* namespace spd */

#endif /* CUBE_HPP_ */
