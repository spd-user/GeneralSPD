/**
 * SimpleActionRule.hpp
 *
 * @date 2012/12/05
 * @author katsumata
 */

#ifndef SIMPLEACTIONRULE_H_
#define SIMPLEACTIONRULE_H_

#include "../Rule.hpp"

namespace spd {
namespace core {
	class Strategy;
}
namespace rule {

/**
 * デフォルトの行動更新ルールを表すクラス
 *
 * 行動更新はDの数に従い、決定的に更新を行う
 */
class SimpleActionRule : public spd::rule::Rule {
public:

	/**
	 * プレイヤの初期化ルール
	 * @note なにもしない
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	void initialize(
			const std::shared_ptr<Player>& player,
			const AllPlayer& allPlayers,
			const spd::param::Parameter& param);

	/**
	 * 行動更新を行う
	 *　@par
	 *　Dの数によって決定的に行動を決定する。<br>
	 *　戦略の長さと近傍数が異なる場合は、対応する戦略へ伸縮させる。<br>
	 *　複数の戦略対象範囲に係るDの数では、正規乱数に従い対応する戦略の位置を求める。
	 *
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 * @param[in] step 実行ステップ
	 */
	void runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step);

	/**
	 * ルール情報の文字出力
	 */
	std::string toString() const {
		return "SimpleActionUpdate";
	}

private:

	/**
	 * Dの数を数える
	 * @param[in] player プレイヤ
	 * @param[in] neighbors 対象プレイヤの近傍
	 * @param[out] dMax Dの取り得る最大値
	 * @return Dの数
	 * @throw std::runtime_error 近傍が見つからない場合や、近傍の行動が未定義の場合
	 */
	int countDNum(const std::shared_ptr<Player>& player,
			const Neighbors& neighbors,
			int* dMax);

	/**
	 * Dの数をプレイヤの戦略の長さに調整する
	 * @par
	 * 調整はその値+0.5 を中心とした、二項分布(正規乱数による近似)に従う
	 *
	 * @param[in] dNum Dの数
	 * @param[in] dMax 近傍プレイヤ数
	 * @param[in] playerStrategy プレイヤの戦略
	 * @param[in] param パラメタ
	 */
	int adjustToStrategyLength(
			int dNum,
			int dMax,
			const std::shared_ptr<spd::core::Strategy>& playerStrategy,
			const spd::param::Parameter& param);

};

} /* namespace rule */
} /* namespace spd */
#endif /* SIMPLEACTIONRULE_H_ */
