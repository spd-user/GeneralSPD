/**
 * Space.hpp
 *
 * @date 2012/09/08
 * @author katsumata
 */

#ifndef SPACE_H_
#define SPACE_H_

#include <chrono>
#include <memory>
#include <vector>
#include <utility>

#include "OriginalType.hpp"

namespace spd {

namespace param {
class Parameter;
}

namespace rule {
class SpdRule;
}

namespace core {
class Player;

/**
 * 空間を表すクラス
 */
class Space {
public:

	/**
	 * コンストラクタ
	 * @param[in] param シミュレーションパラメタ
	 */
	Space(param::Parameter& param);

	/**
	 * 1シミュレーションの実行
	 * @return 最後のシミュレーションが終了したかどうか
	 * @retval true 最後のシミュレーションが終了した場合
	 * @retval false 継続してシミュレーションを行う場合
	 */
	bool run();

	/**
	 * シミュレーションパラメタを取得
	 * @return シミュレーションパラメタ
	 */
	param::Parameter& getParameter() {
		return parameter;
	}

	/**
	 * 空間にいるプレイヤを取得
	 * @return 空間にいるプレイヤ
	 */
	AllPlayer& getPlayers() {
		return players;
	}

	/**
	 * 現在のシミュレーション回数を取得
	 * @return 現在のシミュレーション回数
	 */
	int getSimCount() const {
		return sim;
	}

	/**
	 * 現在のステップ数を取得
	 * @return 現在のステップ数
	 */
	int getStep() const {
		return step;
	}

	/**
	 * ステップ数を設定
	 * @param[in] step ステップ数
	 */
	void setStep(int step) {
		this->step = step;
	}

	/**
	 * 出力前ルールを飛ばすどうかを設定する
	 * @param[in] skip 飛ばすかどうか
	 */
	void setSkipBeforeRules(bool skip) {
		this->skipBeforeRules = skip;
	}

private:
	typedef std::vector<std::pair<std::string, bool>> OutputResultType;

	// シミュレーションパラメタ
	param::Parameter& parameter;

	// 盤面すべてのプレイヤ
	AllPlayer players;

	// シミュレーションのルール内容
	std::shared_ptr<spd::rule::SpdRule> spdRule;

	// 現在のステップ数
	int step;

	// 現在のシミュレーション回数
	int sim;

	// 出力前のルールを飛ばすかどうか
	bool skipBeforeRules;

	// シミュレーション開始時間
	const decltype(std::chrono::system_clock::now()) startTime;

	/*
	 * シミュレーションを1ステップ実行
	 */
	void execStep();

	/*
	 * 進捗の表示
	 */
	void printProgress() const;

	/**
	 * 盤面の出力を行う
	 */
	OutputResultType output();

};

} /* namespace core */
} /* namespace spd */
#endif /* SPACE_H_ */
