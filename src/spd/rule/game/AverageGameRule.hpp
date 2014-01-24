/**
 * AverageGameRule.hpp
 *
 * @date 2013/9/8
 * @author katsumata
 */

#ifndef AVERAGEGAMERULE_H_
#define AVERAGEGAMERULE_H_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * 平均利得を扱う対戦ルールを表すクラス
 *
 */
class AverageGameRule: public spd::rule::Rule {
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
	 * 平均利得の対戦を行う
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
	 * @return "AverageGame"
	 */
	std::string toString() const {
		return "AverageGame";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* AVERAGEGAMERULE_H_ */
