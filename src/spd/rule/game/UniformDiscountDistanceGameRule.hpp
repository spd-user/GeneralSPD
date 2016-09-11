/**
 * UniformDiscountDistance.hpp
 *
 * @date 2016/9/1
 * @author katsumata
 */

#ifndef UniformDiscountDistance_H_
#define UniformDiscountDistance_H_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * 均一な距離割引を扱う対戦ルールを表すクラス
 *
 */
class UniformDiscountDistance: public spd::rule::Rule {
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
	 * 均一な距離割引の対戦を行う
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
	 * @return "UniformDiscountGame"
	 */
	std::string toString() const {
		return "UniformDiscountGame";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* UniformDiscountDistance_H_ */
