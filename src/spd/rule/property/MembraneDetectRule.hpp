/**
 * MembraneDetectRule.hpp
 *
 * @date 2014/01/24
 * @author katsumata
 */
#ifndef MEMBRANEDETECTRULE_HPP_
#define MEMBRANEDETECTRULE_HPP_

#include "../Rule.hpp"
#include "../../core/NeighborhoodType.hpp"

namespace spd {
namespace rule {

/**
 * 膜の検知ルールを表すクラス
 *
 */
class MembraneDetectRule : public spd::rule::Rule {

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
	 * 膜を検知する
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
		return "MemDetect";
	}

private:

	/**
	 * 膜グループ
	 */
	enum class Group {
		BLANK, /** 情報がない */
		INNER, /**< 内側に接触 */
		OUTER, /**< 外側に接触 */
		COMBINE, /**< 内外が */
		BOTH_SIDE, /**< 内外に接触 */
		DIRECT, /**< 共通の、戦略または行動を持たない相手と接触 */
		IGNORE, /**< 膜にならならないので無視 */
	};

	/**
	 * プロパティ名
	 */
	const std::vector<std::string> PROP_NAMES = {"MemGroup", "_NextMemGroup", "_MovePoint", "_NextMovePoint"};

	/**
	 * プロパティの初期値
	 */
	const std::vector<int> INIT_VALS = {
			static_cast<int>(Group::BLANK),
			static_cast<int>(Group::BLANK),
			0, 0};

	/**
	 * プロパティの確認と初期化
	 *
	 * プロパティが無い、若しくはあっても数え上げが設定されていない場合(先頭プレイヤに限る)
	 * プロパティを初期化する
	 * @param player プレイヤ
	 */
	void initProp(const std::shared_ptr<Player>& player);



	/**
	 * 初期のグルーピングを行う
	 *
	 * プレイヤにEnum Groupをつける に分ける
	 * IGNORE -> 膜にはなりえないプレイヤ
	 * DIRECT -> 極の相手と対戦している
	 * BLANK -> 同戦略同行動のプレイヤとしか接続していないプレイヤ
	 * INNER -> 同戦略同行動 + 同戦略"異"行動 と接続しているプレイヤ
	 * OUTER -> 同戦略同行動 + "異"戦略同行動 + ["異"行動"異"戦略] と接しているプレイヤ
	 * BOTH_SIDE -> 同戦略"異"行動 + "異"戦略同行動 + [同戦略同行動 | "異"戦略"異"行動] と接しているプレイヤ
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 * @param filter 膜になり得るかどうかのフィルタ
	 */
	void grouping(const std::shared_ptr<Player> player,
			NeighborhoodType type,
			std::vector<bool>& filter);

	/**
	 * 膜判定が広がる
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 */
	void spreadMembraneDetect(const std::shared_ptr<Player> player,
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
	 * 事後処理
	 *
	 * 膜で無いものについて上書きで、消していく
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 */
	void postHandling(const std::shared_ptr<Player> player,
			NeighborhoodType type);

	/**
	 * 空グループに分類されたプレイヤの動き
	 * inner と outer がくっついたら、combine となる。
	 * このcombineは、innerとouterへ広がる
	 *
	 * @param player プレイヤ
	 * @param neighbors 近傍
	 */
	void blankGroupBehavior(const std::shared_ptr<Player> player,
			Neighbors& neighbors);

	/**
	 * InnerグループとOuterグループの動き
	 * @param player プレイヤ
	 * @param neighbors 近傍
	 */
	void inOutGroupBehavior(const std::shared_ptr<Player> player,
			Neighbors& neighbors);


	/**
	 * 現在の状態をみて膜になるのかどうかのフィルタを作る
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	std::vector<bool> filtering(const AllPlayer& allPlayers, const spd::param::Parameter& param);


};

} /* namespace rule */
} /* namespace spd */

#endif /* MEMBRANEDETECTRULE_HPP_ */
