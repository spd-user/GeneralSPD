/**
 * InverseSquareDiscountDistance.hpp
 *
 * @date 2016/9/1
 * @author katsumata
 */

#ifndef InverseSquareDiscountDistance_H_
#define InverseSquareDiscountDistance_H_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * 距離の二乗の比例する割引率（獲得利得が逆二乗）を扱う対戦ルールを表すクラス
 *
 */
class InverseSquareDiscountDistance: public spd::rule::Rule {
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
	 * 距離の二乗に比例する割引率（獲得利得が逆二乗）の対戦を行う
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
	 * @return "InverseSquareDiscountGame"
	 */
	std::string toString() const {
		return "InverseSquareDiscountGame";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* InverseSquareDiscountDistance_H_ */
