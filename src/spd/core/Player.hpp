/**
 * Player.hpp
 *
 * @date 2012/08/29
 * @author katsumata
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "OriginalType.hpp"
#include "NeighborhoodType.hpp"
#include "Action.hpp"
#include "Converter.hpp"
#include "Property.hpp"

namespace spd {
namespace core {
class Strategy;

/**
 * プレイヤを表すクラス
 */
class Player {
public:

	/**
	 * 行動と戦略を指定するコンストラクタ
	 * @param[in] playerId シミュレーション空間におけるプレイヤID
	 */
	Player(int playerId = -1);

	/**
	 * プレイヤの初期化
	 *
	 * preAction と preStrategy 引数で与えたもので同様に初期化
	 * @param[in] action 行動
	 * @param[in] strategy 戦略
	 */
	void init(Action action, const std::shared_ptr<Strategy>& strategy);

	/**
	 * 相手のプレイヤに一方的に接続
	 *
	 * @note
	 * 双方向接続ではない、また多重接続はしない
	 * @param[in] player 接続相手
	 */
	void linkTo(const std::weak_ptr<Player>& player);

	/**
	 * プレイヤの状態を進める
	 */
	void storePreviousStates() {

		this->preAction = this->action;
		this->preScore = this->score;
		this->preStrategy = this->strategy;
	}

	/**
	 * このプレイヤからの接続をなくす。
	 */
	void resetLink() {
		this->linkedPlayers = std::make_shared<std::vector<std::weak_ptr<Player>>>();
	}

	/**
	 * 接続リストを削除する。
	 */
	void deleteLink() {
		this->linkedPlayers = nullptr;
	}

	/**
	 * プレイヤのIDを取得
	 * @return プレイヤのID
	 */
	int getId() const {
		return id;
	}

	/**
	 * 一つ前のステップの行動を取得Properties
	 * @return 一つ前の行動
	 */
	Action getPreAction() const {
		return preAction;
	}

	/**
	 * 一つ前のステップの行動を設定
	 * @param[in] preAction 一つ前の行動
	 */
	void setPreAction(Action preAction) {
		this->preAction = preAction;
	}

	/**
	 * 現在のステップの行動を取得
	 * @return 現在の行動
	 */
	Action getAction() const {
		return action;
	}

	/**
	 * 現在のステップの行動を設定
	 * @param[in] action 行動
	 */
	void setAction(Action action) {
		this->action = action;
	}

	/**
	 * 一つ前のステップのスコアを取得
	 * @return 一つ前のスコア
	 */
	double getPreScore() const {
		return preScore;
	}

	/**
	 * 一つ前のステップのスコアを設定
	 * @param[in] preScore 一つ前のスコア
	 */
	void setPreScore(double preScore) {
		this->preScore = preScore;
	}

	/**
	 * 現在のステップのスコアを取得
	 * @return 現在のスコア
	 */
	double getScore() const {
		return score;
	}

	/**
	 * 現在のステップのスコアを設定
	 * @param[in] score 現在のスコア
	 */
	void setScore(double score) {
		this->score = score;
	}

	/**
	 * 現在のステップのスコアに対戦の利得を加算
	 * @param[in] score 加算するスコア
	 */
	void addScore(double score) {
		this->score += score;
	}

	/**
	 * 一つ前のステップの戦略を取得
	 * @return 一つ前の戦略
	 */
	const std::shared_ptr<Strategy>& getPreStrategy() const {
		return preStrategy;
	}

	/**
	 * 一つ前のステップの戦略を設定
	 * @param[in] preStrategy 一つ前の戦略
	 */
	void setPreStrategy(const std::shared_ptr<Strategy>& preStrategy) {
		this->preStrategy = preStrategy;
	}

	/**
	 * 現在のステップの戦略を取得
	 * @return 現在の戦略
	 */
	const std::shared_ptr<Strategy>& getStrategy() const {
		return strategy;
	}

	/**
	 * 現在のステップの戦略を設定
	 * @param[in] strategy 現在の戦略
	 */
	void setStrategy(const std::shared_ptr<Strategy>& strategy) {
		this->strategy = strategy;
	}

	/**
	 * 構造的に直接接続しているプレイヤを取得
	 * @return 直接接続しているプレイヤ
	 */
	const std::shared_ptr<std::vector<std::weak_ptr<Player>>>& getLinkedPlayers() const {
		return linkedPlayers;
	}

	/**
	 * 指定した近傍タイプにて関わるプレイヤを取得
	 * @param[in] type 近傍タイプ
	 * @return 関わるプレイヤ
	 * @throw std::invalid_argument type が範囲外の場合
	 */
	Neighbors& getNeighbors(const NeighborhoodType& type) {

		if (type >= NeighborhoodType::TYPE_NUM) {
			throw std::invalid_argument("No neighbors for a neighborhood type of " + type);
		}
		return this->neighborPlayers[type];
	}

	/**
	 * プレイヤのプロパティを取得
	 * @return プロパティ
	 */
	const std::vector<Property>& getProperties() const {
		return properties;
	}

	/**
	 * プロパティを追加
	 * @param prop 追加するプロパティ
	 */
	void addProperty(Property prop) {
		this->properties.push_back(prop);
	}

	/**
	 * プロパティを空にする
	 */
	void clearProperty () {
		this->properties.clear();
	}

	/**
	 * 指定した名前の初めに見つかったプロパティの位置を取得する
	 * @param name 検索するプロパティの名前
	 * @param pos 開始する位置
	 * @return 位置
	 * @retval プロパティが見つからない場合 -1
	 */
	int find(const std::string& name, int pos = 0) {

		if (name.empty()) {
			return -1;
		}

		for (int i = pos, propSize = this->properties.size(); i < propSize; ++i) {
			if (name == properties[i].getName()) {
				return i;
			}
		}
		return -1;
	}


private:

	// プレイヤの ID
	const int id;

	// 一つ前のステップの行動
	Action preAction;

	// 現在の行動
	Action action;

	// 一つ前のステップのスコア
	double preScore;

	// 現在のスコア
	double score;

	// 一つ前のステップの戦略
	std::shared_ptr<Strategy> preStrategy;

	// 現在の戦略
	std::shared_ptr<Strategy> strategy;

	// 行動更新、ゲーム対戦、戦略更新でのプレイヤ
	Neighbors neighborPlayers[NeighborhoodType::TYPE_NUM];

	// 直接接続しているプレイヤ
	std::shared_ptr<std::vector<std::weak_ptr<Player>>> linkedPlayers;

	// プロパティ
	std::vector<Property> properties;
};


} /* namespace core */
} /* namespace spd */

#endif /* PLAYER_H_ */
