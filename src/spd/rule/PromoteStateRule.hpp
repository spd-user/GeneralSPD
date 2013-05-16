/**
 * PromoteStateRule.hpp
 *
 * @date 2013/05/16
 * @author katsumata
 */

#ifndef PROMOTESTATERULE_HPP_
#define PROMOTESTATERULE_HPP_

#include "Rule.hpp"

#include <memory>
#include <string>

#include "../core/Player.hpp"
#include "../param/Parameter.hpp"

namespace spd {
namespace rule {

/**
 * プレイヤの状態を進めるルールを表すクラス
 */
class PromoteStateRule: public spd::rule::Rule {
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
			const spd::param::Parameter& param) {}

	/**
	 * プレイヤの状態を進める
	 * @par
	 * 進める状態は行動、利得、戦略のみ
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
			int step) {

		player->storePreviousStates();
	}

	/**
	 * ルール情報の文字出力
	 */
	std::string toString() const {
		return "Store";
	}
};

}
}

#endif /* PROMOTESTATERULE_HPP_ */
