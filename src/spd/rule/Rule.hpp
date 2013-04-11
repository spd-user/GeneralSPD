/**
 * Rule.hpp
 *
 * @date 2012/12/26
 * @author katsumata
 */

#ifndef RULE_H_
#define RULE_H_

#include <memory.h>
#include "../IToString.hpp"
#include "../core/OriginalType.hpp"

namespace spd {
namespace core {
	class Player;
}
namespace param {
	class Parameter;
}
namespace rule {

using spd::core::AllPlayer;
using spd::core::Neighbors;
using spd::core::Player;

/**
 *  ルールを表す抽象クラス
 */
class Rule : public IToString {
public:

	/**
	 * デストラクタ
	 */
	virtual ~Rule(){};


	/**
	 * ルール特有のプレイヤの初期化処理
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	virtual void initialize(
			const std::shared_ptr<Player>& player,
			const AllPlayer& allPlayers,
			const spd::param::Parameter& param) = 0;

	/**
	 * 更新ルール
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 * @param[in] step 実行ステップ
	 */
	virtual void runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) = 0;


};

} /* namespace rule */
} /* namespace spd */
#endif /* RULE_H_ */
