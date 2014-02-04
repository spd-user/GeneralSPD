/**
 * AffectedPlayerRule.hpp
 *
 * @date 2014/02/04
 * @author katsumata
 */
#ifndef AFFECTEDPLAYERRULE_HPP_
#define AFFECTEDPLAYERRULE_HPP_

#include "../Rule.hpp"
#include "../../core/NeighborhoodType.hpp"

namespace spd {
namespace rule {

/**
 * 膜の影響を受けているプレイヤを検知するルールを表すクラス
 *
 * 膜検知ルールを実行した後でないとエラーが発生
 */
class AffectedPlayerRule : public spd::rule::Rule {
public:

	/**
	 * 影響状態
	 */
	enum class Affect {
		BLANK, /**< まだ分からない */
		MEMBRANE, /**< 膜の影響を受けている */
		ENEMY, /**< 敵の影響 */
		IGNORE, /**< 考えないでよいプレイヤ */
	};

	/**
	 * プロパティを設定する
	 * @param[in, out] player 対象プレイヤ
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	void initialize(
			const std::shared_ptr<Player>& player,
			const AllPlayer& allPlayers,
			const spd::param::Parameter& param);

	/**
	 * 影響検知
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
	 * @return "MemDetect"
	 */
	std::string toString() const {
		return "Affected";
	}

private:

	/**
	 * プロパティ名
	 */
	const std::vector<std::string> PROP_NAMES = {"Affect", "_NextAffect"};

	/**
	 * プロパティの初期値
	 */
	const std::vector<int> INIT_VALS = {
			static_cast<int>(Affect::BLANK),
			static_cast<int>(Affect::BLANK)};

	/**
	 * プロパティの確認と初期化
	 *
	 * プロパティが無い、若しくはあっても数え上げが設定されていない場合(先頭プレイヤに限る)
	 * プロパティを初期化する
	 * @param player プレイヤ
	 */
	void initProp(const std::shared_ptr<Player>& player);

	/**
	 * 直接的な影響を調べる
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 * @param filter 膜になり得るかどうかのフィルタ
	 */
	void directAffect(const std::shared_ptr<Player> player,
				NeighborhoodType type,
				std::vector<bool>& filter);
	/**
	 * 影響が広がる
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 */
	void spreadAffect(const std::shared_ptr<Player> player,
			NeighborhoodType type);

	/**
	 * next を current に更新する
	 *
	 * その際状態が変われば、true, 変わらなければ false とする
	 * @param player プレイヤ
	 * @return 状態が変わったかどうか
	 * @retval true 状態が変わった
	 * @retval false 状態が変わらない
	 */
	bool changesStatus(const std::shared_ptr<Player> player);

	/**
	 * 現在の状態をみて膜になるのかどうかのフィルタを作る
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	std::vector<bool> filtering(const AllPlayer& allPlayers, const spd::param::Parameter& param);


};

} /* namespace rule */
} /* namespace spd */

#endif /* AFFECTEDPLAYERRULE_HPP_ */
