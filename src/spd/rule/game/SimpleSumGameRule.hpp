/**
 * DefaultGameRule.hpp
 *
 * @date 2012/12/12
 * @author katsumata
 */

#ifndef SIMPLESUMGAMERULE_H_
#define SIMPLESUMGAMERULE_H_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * デフォルトの対戦ルールを表すクラス
 *
 * 対戦は純粋な総和で行う
 */
class SimpleSumGameRule: public spd::rule::Rule {
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
	 * 総和対戦を行う
	 *　@par
	 *　ハブとなるプレイヤの利得は、一般プレイヤより基本的には大きい分散となる。
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
	 * @return "SimpleSumGame";
	 */
	std::string toString() const {
		return "SimpleSumGame";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* SIMPLESUMGAMERULE_H_ */
