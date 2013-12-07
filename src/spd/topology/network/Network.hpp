/**
 * Network.hpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Topology.hpp"

namespace spd {
namespace topology {

/**
 * ネットワークグラフを表すクラス
 */
class Network: public spd::topology::Topology {
public:

	/**
	 * デフォルトデストラクタ
	 */
	virtual ~Network() {};

	/**
	 * グラフ用の出力を行う
	 *
	 * @param[in] visitor 構造に対する出力の処理方法
	 * @param[in] space 空間
	 */
	void accept(spd::output::OutputVisitor& visitor, spd::core::Space& space);

	/**
	 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
	 * @param[in] players すべてのプレイヤ
	 * @param[in] target 対象プレイヤの空間位置
	 * @param[in] radius 近傍半径
	 */
	spd::core::Neighbors getNeighbors(
			const spd::core::AllPlayer& players,
			int target,
			int radius);

	/**
	 * 接続と近傍の再設定
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void reSetting(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * 空間において必須の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍距離
	 * @return 最低限必要な長さ
	 * @retval 最低長はないので0
	 */
	int minStrategyLength(int actionRadius);

	/**
	 * 空間において最大の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍の長さ
	 * @return 最大の戦略長
	 * @retval 最大の戦略長はないので0
	 */
	int maxStrategyLength(int actionRadius);

	/**
	 * 中心座標を取得する
	 * @param playerNum 全プレイヤ数
	 * @return  全プレイヤの中央
	 */
	int getCenterIndex(int playerNum) const {
		return playerNum / 2;
	};

private:

	/**
	 * 対象のプレイヤが近傍内にあるかidを使って調べる
	 * @param[in] neighbors 入っているか調べられる近傍
	 * @param[in] id 対象のプレイヤID
	 * @return すでにあるかどうか
	 * @retval true すでにある場合
	 * @retval false まだない場合
	 */
	bool existOnNeighbors(spd::core::Neighbors& neighbors, int id);

	/**
	 * 近傍数を数える
	 * @param[in] neighbors
	 * @return 近傍数
	 */
	int countNeighbors(spd::core::Neighbors& neighbors);

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
#endif /* NETWORK_H_ */
