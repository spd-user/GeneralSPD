/**
 * Ring.hpp
 *
 * @date 2014/01/31
 * @author katsumata
 */
#ifndef RING_HPP_
#define RING_HPP_

#include "Network.hpp"

namespace spd {
namespace topology {

/**
 * 一次元ネットワークを表すクラス
 */
class Ring: public spd::topology::Network {

public:
	/**
	 * コンストラクタ
	 */
	Ring() {};

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
			const spd::param::Parameter& param) {
		return;
	}

	/**
	 * 空間において必須の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍距離
	 * @return 最低限必要な長さ
	 * @retval 最低長はないので0
	 */
	int minStrategyLength(int actionRadius) {
		return calcNeighborsNum(actionRadius);
	}

	/**
	 * 空間において最大の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍の長さ
	 * @return 最大の戦略長
	 * @retval 最大の戦略長はないので0
	 */
	int maxStrategyLength(int actionRadius) {
		return calcNeighborsNum(actionRadius);
	}

	/**
	 * なにもしない
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<std::string> properties) {
		return;
	}

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名(Ring)
	 */
	std::string toString() const {
		std::string result;

		result = "Ring";
		return result;
	}

private:

	/** 隣接プレイヤ数 */
	static const int ADJACENCE = 2;

	/**
	 * 指定した近傍半径の自身を含めたプレイヤ数を計算する
	 * @param[in] radius 近傍半径
	 * @return 自身を含めた近傍プレイヤ数
	 */
	int calcNeighborsNum(int radius) const {
		return (2 * radius) + 1;
	}

};

} /* namespace topology */
} /* namespace spd */

#endif /* RING_HPP_ */
