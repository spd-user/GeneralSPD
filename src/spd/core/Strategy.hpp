/**
 * Strategy.hpp
 *
 * @date 2012/08/25
 * @author: katsumata
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_

#include <vector>
#include <string>
#include "Action.hpp"

namespace spd {
namespace core {

/**
 * 戦略を表すクラス
 */
class Strategy {
public:

	/**
	 * 長さ0, 戦略文字列がすべて空の戦略を作成
	 */
	Strategy();

	/**
	 * 指定された戦略文字列から、該当する戦略長さだけの戦略を作成
	 *
	 * @par
	 * 最大戦略長が0の場合は、与えられた戦略文字列すべてを用いる
	 *
	 * @param[in] strategy 戦略を表す文字列(省略でも冗長でも両方可)
	 * @param[in] minLen 最低限の戦略長
	 * @param[in] maxLen 最大の戦略長
	 */
	Strategy(const char* strategy, int minLen = 0, int maxLen = 0);

	/**
	 * 戦略が指定行動の All 戦略かどうかを調べる
	 * @param[in] action 調べる行動
	 * @return 戦略が All 戦略かの判定結果
	 * @retval true 指定行動のAll戦略の場合
	 * @retval false 指定行動のAll戦略でない場合
	 */
	bool isAll(Action action) const;

	/**
	 * 指定したDの数に該当する行動を返す
	 * @param[in] dNum Dの数
	 * @return 行動
	 */
	Action actionAt(int dNum) const {
		return this->longStrategy.at(dNum);
	}

	/**
	 * 戦略が同じかどうか判定
	 * @param[in] anotherStrategy 判定する戦略
	 * @return 判定結果
	 * @retval true 同じ戦略の場合
	 * @retval false 異なる戦略の場合
	 */
	bool equals(const Strategy& anotherStrategy) const {

		// 短縮形が同じなら一緒
		if (this->shortStrategy == anotherStrategy.getShortStrategy()) {
			return true;
		}
		return false;
	}

	/**
	 * 戦略の長さを取得
	 * @return 戦略の長さ
	 */
	int getLength() const {
		return longStrategy.size();
	}

	/**
	 * 冗長な列挙型戦略を取得
	 * @return 冗長な列挙型戦略
	 */
	const std::vector<Action>& getLongStrategy() const {
		return longStrategy;
	}

	/**
	 * 略記の戦略を取得
	 * @return 略記の戦略
	 */
	const std::string& getShortStrategy() const {
		return shortStrategy;
	}

	/**
	 * 戦略のIDを取得
	 * @return 戦略のID
	 */
	int getId() const {
		return id;
	}

	/**
	 * 戦略のIDを設定
	 * @param[in] id 戦略のID
	 */
	void setId(int id) {
		this->id = id;
	}


private:
	// 戦略の冗長列挙型表記
	std::vector<Action> longStrategy;

	// 戦略の省略表記
	std::string shortStrategy;

	// 戦略のID
	int id;

	/**
	 * 文字配列を冗長な列挙型戦略列に変換
	 * @param[in] strategy 戦略の文字配列
	 * @param[in] minLen 最低限の戦略長
	 * @param[in] maxLen 最大の戦略長
	 * @return 冗長な列挙型戦略列
	 * @throw std::invalid_argument 先頭が数字の戦略の場合または、最低限の長さを満たせない場合
	 */
	std::vector<Action> toLongStrategy(const char* strategy, int minLen, int maxLen);

	/**
	 * 冗長な列挙型戦略列を、略記文字列に変換
	 * @param[in] longStrategy 冗長な列挙型戦略列
	 * @return 略記文字列
	 */
	std::string toShortStrategy(const std::vector<Action>& longStrategy);
};

} /* namespace core */
} /* namespace spd */
#endif /* STRATEGY_H_ */
