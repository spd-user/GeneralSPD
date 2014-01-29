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
 * @note 設計の都合上、マルチスレッドに対応しません。シングルスレッドで実行されます。
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
	 * プレイヤの初期化ルール
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
	 * プロパティ名
	 */
	const std::vector<std::string> PROP_NAMES = {"MemGroup", "_NextMemGroup", "_MovePoint", "_NextMovePoint"};

	/**
	 * プロパティの初期値
	 */
	const std::vector<int> INIT_VALS = {0, 0, 0, 0};

	/**
	 * プロパティの確認と初期化
	 *
	 * プロパティが無い、若しくはあっても数え上げが設定されていない場合(先頭プレイヤに限る)
	 * プロパティを初期化する
	 */
	void initProp(const std::shared_ptr<Player>& player);



	/**
	 * 初期のグルーピングを行う
	 *
	 * 初期のプレイヤを-1, 0, 1, 2, 3 に分ける
	 * -1 -> 膜にはなりえないプレイヤ(0-3 以外)
	 * 0 -> 同戦略同行動のプレイヤとしか接続していないプレイヤ
	 * 1 -> 同戦略同行動 + 同戦略"異"行動 と接続しているプレイヤ
	 * 2 -> 同戦略同行動 + "異"戦略同行動 + ["異"行動"異"戦略] と接しているプレイヤ
	 * 3 -> 同戦略"異"行動 + "異"戦略同行動 + [同戦略同行動 | "異"戦略"異"行動] と接しているプレイヤ
	 * @param player プレイヤ
	 * @param type 考える膜のタイプ
	 */
	void grouping(const std::shared_ptr<Player> player,
			NeighborhoodType type);

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
	 * グループ0に分類されたプレイヤの動き
	 * 1 と 2 がくっついたら、4となる。この4は、1と2へ広がる
	 *
	 * @param player プレイヤ
	 * @param neighbors 近傍
	 */
	void groupZeroBehavior(const std::shared_ptr<Player> player,
			Neighbors& neighbors);

	/**
	 * グループ1, 2に分類されたプレイヤの動き
	 * @param player プレイヤ
	 * @param neighbors 近傍
	 */
	void groupOneTwoBehavior(const std::shared_ptr<Player> player,
			Neighbors& neighbors);


};

} /* namespace rule */
} /* namespace spd */

#endif /* MEMBRANEDETECTRULE_HPP_ */
