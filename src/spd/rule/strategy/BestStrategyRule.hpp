/**
 * DefaultStrategyRule.hpp
 *
 * @date 2012/12/12
 * @author katsumata
 */

#ifndef BESTSTRATEGYRULE_H_
#define BESTSTRATEGYRULE_H_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * デフォルトの戦略更新を表すクラス
 *
 * 最も利得を得ているプレイヤの戦略に、決定的に決まる
 */
class BestStrategyRule: public spd::rule::Rule {
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
	 * 戦略更新を行う
	 * @par
	 * 戦略は、最大値利得戦略へ決定的に更新される
	 *
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] param パラメタ
	 * @param[in] allPlayers 全てのプレイヤ
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
		return "BestStrategyUpdate";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* BESTSTRATEGYRULE_H_ */
