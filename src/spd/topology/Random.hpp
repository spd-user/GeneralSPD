/**
 * Random.hpp
 *
 * @date 2013/09/05
 * @author katsumata
 */

#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <memory>
#include <string>
#include "Network.hpp"

namespace spd {
namespace topology {

/**
 * ランダムグラフを表すクラス
 */
class Random: public spd::topology::Network {

public :

	/**
	 * デフォルトの接続確率でクラス生成
	 */
	Random();

	/**
	 * すべてのプレイヤの接続を作成する。
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void connectPlayers(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

//	/**
//	 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
//	 * @param[in] players すべてのプレイヤ
//	 * @param[in] target 対象プレイヤの空間位置
//	 * @param[in] radius 近傍半径
//	 */
//	spd::core::Neighbors getNeighbors(
//			const spd::core::AllPlayer& players,
//			int target,
//			int radius);
//
//	/**
//	 * 接続と近傍の再設定
//	 * @param[in] players すべてのプレイヤ
//	 * @param[in] param パラメタ
//	 */
//	void reSetting(const spd::core::AllPlayer& players,
//			const spd::param::Parameter& param);
//
//	/**
//	 * 空間において必須の戦略の長さを求める
//	 * @param[in] actionRadius 行動更新での近傍距離
//	 * @return 最低限必要な長さ
//	 * @retval 最低長はないので0
//	 */
//	int minStrategyLength(int actionRadius);
//
//	/**
//	 * 空間において最大の戦略の長さを求める
//	 * @param[in] actionRadius 行動更新での近傍の長さ
//	 * @return 最大の戦略長
//	 * @return 最大の戦略長はないので0
//	 */
//	int maxStrategyLength(int actionRadius);

	/**
	 * トポロジのプロパティ(接続確率)を設定する
	 *
	 * もし複数のプロパティが渡されても、一つ目しか使用しない<br>
	 * また、無い場合は変更しないまま
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<double> properties);

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名(Random)
	 */
	std::string toString() const {
		std::string result;

		result = "Random:" + std::to_string(this->connectionProbability);
		return result;
	}

private :

	/**
	 * デフォルトの接続確率
	 */
	static constexpr double DEFAULT_CONNECTION_PROBABILITY = 0.01;

	/**
	 * 接続確率
	 */
	double connectionProbability;

//
//	/**
//	 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
//	 *
//	 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
//	 * @param[in] neighborType 近傍タイプ
//	 * @param[in] players すべてのプレイヤ
//	 * @param[in] param パラメタ
//	 */
//	void setNeighborsOf(
//			NeighborhoodType neighborType,
//			const spd::core::AllPlayer& players,
//			const spd::param::Parameter& param);
};

} /* namespace topology */
} /* namespace spd */
#endif /* RANDOM_HPP_ */
